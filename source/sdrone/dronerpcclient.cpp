/*
 * dronerpcclient.cpp
 *
 *  Created on: Aug 17, 2014
 *      Author: svassilev
 */

#include "commandargs.h"
#include "dronerpcclient.h"
#include "jsonrpcdispatch.h"
#include "jsonrpcparser.h"
#include <time.h>

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
			DroneRpcClient::BuildJsonDroneConfigFromCmdLineArgs(&req.Params, cmdArgs);
			break;
		case SD_COMMAND_SET_THRUST:
			DroneRpcClient::BuildJsonThrustParamsFromCmdLineArgs(&req.Params, cmdArgs);
			break;
		case SD_COMMAND_PING:
			req.Params = (double)clock();
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
					double receivedTimestamp = 0.0;
					rep.Results.AsDoubleSafe(&receivedTimestamp);
					if (receivedTimestamp == req.Params.AsDouble()) {
						printf("Ping reply: %f\n", receivedTimestamp);
					} else {
						printf("WARNING: Ping reply carries wrong time stamp: %f, "
								"expected %f\n", receivedTimestamp, req.Params.AsDouble());
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

bool DroneRpcClient::BuildJsonDroneConfigFromCmdLineArgs(
		SdJsonValue* jsonArgs,
		const CommandLineArgs& cmdArgs)
{
	/*
	 * Here we want to parse the json schema from the file name provided on the
	 * command line and add on top of it any settings that came from the rest of
	 * command line arguments, thus effectively any manually entered cmd line args
	 * will trump over the settings from the json config file.
	 */
	SdJsonObject jobjDroneConfig;

	const std::string& configFile = cmdArgs.GetConfigFile();
	if (configFile.length() > 0) {
		SdJsonParser jsonParser;
		if (!jsonParser.ParseFile(configFile.c_str())) {
			printf("Failed to parse config file %s\n",configFile.c_str());
			return false;
		}
		const IJsonObject* jobjFromFile = jsonParser.RootObj();
		if (jobjFromFile->GetMember("GlobalConfig")->GetType() == SD_JSONVALUE_OBJECT) {
			jobjDroneConfig = *(SdJsonObject*)jobjFromFile->GetMember("GlobalConfig")->AsObject();
		}
	}

	int32_t intVal;
	double doubleVal;
	if (cmdArgs.GetMaxDps(&intVal) ||
		cmdArgs.GetImuSamplingRage(&intVal) ||
		cmdArgs.GetImuInputFile().length() > 0)
	{
		SdJsonObject jobjGyroConfig;
		SdJsonObject jobjAccConfig;
		SdJsonObject jobjMagConfig;
		if (jobjDroneConfig.GetMember("Gyro")->GetType() == SD_JSONVALUE_OBJECT) {
			jobjGyroConfig = *(SdJsonObject*)jobjDroneConfig.GetMember("Gyro")->AsObject();
		}
		if (jobjDroneConfig.GetMember("Accelerometer")->GetType()==SD_JSONVALUE_OBJECT) {
			jobjGyroConfig = *(SdJsonObject*)jobjDroneConfig.GetMember("Accelerometer")->AsObject();
		}
		if (jobjDroneConfig.GetMember("Magnetometer")->GetType()==SD_JSONVALUE_OBJECT) {
			jobjGyroConfig = *(SdJsonObject*)jobjDroneConfig.GetMember("Magnetometer")->AsObject();
		}
		if (cmdArgs.GetMaxDps(&intVal)) {
			jobjGyroConfig.AddMember("Scale",SdJsonValue(intVal));
		}
		if (cmdArgs.GetImuSamplingRage(&intVal)) {
			jobjGyroConfig.AddMember("SamplingRage",SdJsonValue(intVal));
			jobjAccConfig.AddMember("SamplingRage",SdJsonValue(intVal));
			jobjMagConfig.AddMember("SamplingRage",SdJsonValue(intVal));
		}
		std::string infile = cmdArgs.GetImuInputFile();
		if (infile.length() > 0) {
			jobjGyroConfig.AddMember("DeviceName",SdJsonValue(infile));
			jobjAccConfig.AddMember("DeviceName",SdJsonValue(infile));
			jobjMagConfig.AddMember("DeviceName",SdJsonValue(infile));
		}
		if (jobjGyroConfig.GetMemberCount() > 0) {
			jobjDroneConfig.AddMember("Gyro",SdJsonValue(jobjGyroConfig));
		}
		if (jobjAccConfig.GetMemberCount() > 0) {
			jobjDroneConfig.AddMember("Accelerometer",SdJsonValue(jobjAccConfig));
		}
		if (jobjMagConfig.GetMemberCount() > 0) {
			jobjDroneConfig.AddMember("Magnetometer",SdJsonValue(jobjMagConfig));
		}
	}

	std::string outfile = cmdArgs.GetServoOutputFile();
	if (outfile.length() > 0) {
		SdJsonObject jobjServoConfig;
		if (jobjDroneConfig.GetMember("Servo")->GetType() == SD_JSONVALUE_OBJECT) {
			jobjServoConfig = *(SdJsonObject*)jobjDroneConfig.GetMember("Servo")->AsObject();
		}
		jobjServoConfig.AddMember("DeviceName",
				SdJsonValue(outfile));
		jobjDroneConfig.AddMember("Servo", SdJsonValue(jobjServoConfig));
	}
	if (cmdArgs.GetLogRate(&doubleVal)) {
		jobjDroneConfig.AddMember("LogRate",doubleVal);
	}
	jobjDroneConfig.AddMember("LogRotationMatrix",cmdArgs.LogRotMatrix());
	if (cmdArgs.GetImuToRotorAxisAngle(&intVal) ||
		cmdArgs.GetKp(&doubleVal) ||
		cmdArgs.GetKi(&doubleVal) ||
		cmdArgs.GetKd(&doubleVal)) {
		SdJsonObject jobjQuadPilot;
		if (jobjDroneConfig.GetMember("QuadPilot")->GetType() == SD_JSONVALUE_OBJECT) {
			jobjQuadPilot = *(SdJsonObject*)jobjDroneConfig.GetMember("QuadPilot")->AsObject();
		}
		if (cmdArgs.GetImuToRotorAxisAngle(&intVal)) {
			SdJsonArray jarr;
			jarr.AddElement(SdJsonValue((uint32_t)0));
			jarr.AddElement(SdJsonValue((uint32_t)0));
			jarr.AddElement(SdJsonValue((uint32_t)intVal));
			jobjQuadPilot.AddMember("ImuCoordinateAngles",jarr);
		}
		if (cmdArgs.GetKp(&doubleVal)) {
			jobjQuadPilot.AddMember("Kp",SdJsonValue(doubleVal));
		}
		if (cmdArgs.GetKi(&doubleVal)) {
			jobjQuadPilot.AddMember("Ki",SdJsonValue(doubleVal));
		}
		if (cmdArgs.GetKd(&doubleVal)) {
			jobjQuadPilot.AddMember("Kd",SdJsonValue(doubleVal));
		}
		jobjDroneConfig.AddMember("QuadPilot",jobjQuadPilot);
	}

	jsonArgs->SetValueAsObject(&jobjDroneConfig);
	return true;
}

bool DroneRpcClient::BuildJsonThrustParamsFromCmdLineArgs(
		SdJsonValue* jsonThrustParams,
		const CommandLineArgs& cmdArgs)
{
	bool ret = true;
	double thrust;
	SdJsonObject jobj;
	if (cmdArgs.GetThrust(&thrust)) {
		ret = ret && jobj.AddMember("Thrust",SdJsonValue(thrust));
	}
	if (cmdArgs.GetMinThrust(&thrust)) {
		ret = ret && jobj.AddMember("MinThrust",SdJsonValue(thrust));
	}
	if (cmdArgs.GetMaxThrust(&thrust)) {
		ret = ret && jobj.AddMember("MaxThrust",SdJsonValue(thrust));
	}
	if (jobj.GetMemberCount()) {
		ret = jsonThrustParams->SetValueAsObject(&jobj);
	}
	return ret;
}
