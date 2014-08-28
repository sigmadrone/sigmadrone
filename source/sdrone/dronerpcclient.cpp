/*
 * dronerpcclient.cpp
 *
 *  Created on: Aug 17, 2014
 *      Author: svassilev
 */

#include <time.h>
#include "commandargs.h"
#include "dronerpcclient.h"
#include "jsonrpcdispatch.h"
#include "rpcparams.h"

DroneRpcClient::DroneRpcClient() {}

DroneRpcClient::~DroneRpcClient() {}

int DroneRpcClient::ExecuteCommand(const CommandLineArgs& cmdArgs)
{
	int err = 0;
	IRpcTransport* rpcTransport=IRpcTransport::Create(
			IRpcTransport::TRANSPORT_HTTP);
	if (rpcTransport)
	{
		SdJsonRpcDispatcher rpcDispatch(rpcTransport);
		SdJsonRpcRequest req;
		SdJsonRpcReply rep;
		req.MethodName = SdCommandCodeToString(cmdArgs.GetCommand());

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
		case SD_COMMAND_SET_TARGET_ATTITUDE:
			break;
		default:break;
		}

		if (rpcDispatch.SendJsonRequest(req,
				cmdArgs.GetHostAddress(),
				cmdArgs.GetServerPort(),&rep))
		{
			printf("Successfully sent command %s, ret value %d\n",
					req.MethodName.c_str(), rep.ErrorCode);
			if (SD_JSONRPC_ERROR_SUCCESS == rep.ErrorCode)
			{
				switch (cmdArgs.GetCommand()) {
				case SD_COMMAND_PING: {
					int64_t receivedTimestamp = 0;
					rep.Results.AsIntSafe(&receivedTimestamp);
					if (receivedTimestamp == req.Params.AsInt()) {
						printf("Ping reply: %ld\n", receivedTimestamp);
					} else {
						printf("WARNING: Ping reply carries wrong time stamp: %ld, "
								"expected %ld\n", receivedTimestamp, req.Params.AsInt());
					}
					break;
				}
				default:break;
				}
			} else {
				printf("WARNING: Command errored out: %d!\n",rep.ErrorCode);
			}
		}
		else
		{
			printf("Failed to send command %s to %s, error \"%s\"",
					req.MethodName.c_str(),cmdArgs.GetHostAddress().c_str(),
					strerror(err));
		}
		delete rpcTransport;
	}
	return err;
}
