/*
 * dronerpcclient.cpp
 *
 *  Created on: Aug 17, 2014
 *      Author: svassilev
 */

#include <time.h>
#include <iostream>
#include "dronerpcclient.h"
#include "rpcparams.h"
#include "jsonrpcparser.h"

DroneRpcClient::DroneRpcClient() {
	m_rpc_transport.reset(IRpcTransport::Create(
			IRpcTransport::TRANSPORT_HTTP));
}

DroneRpcClient::~DroneRpcClient() {}

int DroneRpcClient::ExecuteCommand(const CommandLineArgs& cmdArgs)
{
	if (!m_rpc_transport.get()) {
		return ENOMEM;
	}
	if (cmdArgs.GetCommand() == SD_COMMAND_NONE) {
		printf("Unrecognized or no command specified!\n");
		return EINVAL;
	}

	SdJsonRpcRequest req;
	SdJsonRpcReply rep;
	req.MethodName = SdCommandCodeToString(cmdArgs.GetCommand());
	if (cmdArgs.GetRpcParamsList().size() > 0) {
		FillRequestParamsFromJsonSpec(cmdArgs,req.Params);
	} else if (cmdArgs.GetRpcParamsAsJsonStream().length() > 0) {
		FillRequestParamsFromJsonStream(cmdArgs,req.Params);
	} else {
		switch (cmdArgs.GetCommand()) {
		case SD_COMMAND_RUN:
		case SD_COMMAND_SET_CONFIG:
			RpcParams::BuildJsonDroneConfigFromCmdLineArgs(&req.Params, cmdArgs);
			break;
		case SD_COMMAND_SET_THRUST:
			RpcParams::BuildJsonThrustParamsFromCmdLineArgs(&req.Params, cmdArgs);
			break;
		case SD_COMMAND_PING: {
			timespec ts = {0};
			clock_gettime(CLOCK_MONOTONIC,&ts);
			int64_t clockValue = (int64_t)ts.tv_sec*1000*1000*1000 + ts.tv_nsec;
			req.Params = SdJsonValue(clockValue);
		}
		break;
		case SD_COMMAND_SET_ATTITUDE:
			RpcParams::BuildJsonTargetQuaternion(&req.Params,cmdArgs.GetTargetAttitude());
			break;
		case SD_COMMAND_GET_RPC_SPEC:
			req.Params = SdJsonValue(cmdArgs.GetApiName());
			break;
		default:break;
		}
	}

	if (MakeRpcCall(cmdArgs.GetHostAddress(),
			cmdArgs.GetServerPort(),
			req,
			rep))
	{
		if (SD_JSONRPC_ERROR_SUCCESS == rep.ErrorCode) {
			switch (cmdArgs.GetCommand()) {
			case SD_COMMAND_PING: {
				int64_t receivedTimestamp = 0;
				rep.Results.AsIntSafe(&receivedTimestamp);
				if (receivedTimestamp == req.Params.AsInt()) {
					timespec ts = {0};
					clock_gettime(CLOCK_MONOTONIC,&ts);
					int64_t clockValue = (int64_t)ts.tv_sec*1000*1000*1000 + ts.tv_nsec;
					cout << "Reply from " << cmdArgs.GetHostAddress() << ":" << cmdArgs.GetServerPort();
					cout << " timestamp=" << receivedTimestamp <<" RTT=";
					cout << (double)(clockValue - receivedTimestamp)/1000/1000 << " ms\n";
				} else {
					printf("WARNING: Ping reply carries wrong time stamp: %lu, "
							"expected %lu\n", receivedTimestamp, req.Params.AsInt());
				}
				break;
			}
			default:
			case SD_COMMAND_GET_CONFIG:
			case SD_COMMAND_GET_THRUST:
			case SD_COMMAND_GET_ALTITUDE:
			case SD_COMMAND_GET_ATTITUDE:
			case SD_COMMAND_GET_RPC_SPEC:
			case SD_COMMAND_GET_RPC_LIST:
				cout << SdJsonValueToText(rep.Results) << "\n";
				break;
			}
		}
	}
	return errno;
}

bool DroneRpcClient::MakeRpcCall(
		const std::string& serverAddress,
		int serverPort,
		const SdJsonRpcRequest& req,
		SdJsonRpcReply& rep)
{
	SdJsonRpcDispatcher rpcDispatch(m_rpc_transport.get());
	bool retVal=rpcDispatch.SendJsonRequest(req,serverAddress,serverPort,&rep);
	if (retVal) {
		if (SD_JSONRPC_ERROR_SUCCESS != rep.ErrorCode) {
			printf("WARNING: Command errored out: %d %s!\n",rep.ErrorCode,
					SdIsJsonRpcError(rep.ErrorCode) ? SdJsonRpcErrorToString(rep.ErrorCode) :
					strerror(rep.ErrorCode));
		}
	} else {
		printf("Failed to send command %s to %s, error \"%s\"",
				req.MethodName.c_str(),serverAddress.c_str(),
				strerror(errno));
	}
	return retVal;
}

bool DroneRpcClient::FillRequestParamsFromJsonSpec(
		const CommandLineArgs& cmdArgs,
		SdJsonValue& requestParams)
{
	// Get the spec by issuing rpc
	SdJsonRpcRequest req(SdCommandCodeToString(SD_COMMAND_GET_RPC_SPEC),
			SdJsonValue(SdCommandCodeToString(cmdArgs.GetCommand())));
	SdJsonRpcReply rep;
	if (!MakeRpcCall(cmdArgs.GetHostAddress(),cmdArgs.GetServerPort(),req,rep)) {
		return false;
	}
	if (rep.Results.GetType() != SD_JSONVALUE_OBJECT) {
		cout << "Received unexpected spec for command" <<
				SdCommandCodeToString(SD_COMMAND_GET_RPC_SPEC) << std::endl;
		cout << SdJsonValueToText(rep.Results) << std::endl;
		return false;
	}

	// From the method spec and the command line args, construct a SdJsonValue
	// representing an actual JSON RPC request params
	SdJsonValue paramsSpec = rep.Results.AsObject()->GetMember("params");
	requestParams = SdJsonValueFromSpec(paramsSpec,cmdArgs.GetRpcParamsList()).Get();
	return true;
}

bool DroneRpcClient::FillRequestParamsFromJsonStream(
	const CommandLineArgs& cmdArgs,
	SdJsonValue& rpcParams)
{
	SdJsonRpcParser parser;
	std::string jsonStream = cmdArgs.GetRpcParamsAsJsonStream();
	if (!parser.ParseBuffer(jsonStream.c_str(), jsonStream.length(), 0)) {
		cout << "Failed to parse json buffer from command line\n    " << jsonStream <<
				std::endl;
		return false;
	}
	rpcParams = parser.RootNode();
	cout << "FillRequestParamsFromJsonStream: \n" <<
			SdJsonValueToText(rpcParams) << std::endl;
	return true;
}
