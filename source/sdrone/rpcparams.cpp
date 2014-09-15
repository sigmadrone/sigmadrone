/*
 * rpcparams.cpp
 *
 *  Created on: Aug 27, 2014
 *      Author: svassilev
 */

#include "rpcparams.h"
#include "jsonrpcparser.h"

namespace RpcParams {

bool ParseJsonImuConfig(
	const IJsonObject* obj,
	SdImuDeviceConfig* imu)
{
	if (obj) {
		obj->GetMember("DeviceName")->AsStringSafe(&imu->DeviceName);
		obj->GetMember("SamplingRate")->AsIntSafe(&imu->SamplingRate);
		obj->GetMember("Scale")->AsIntSafe(&imu->Scale);
		obj->GetMember("MaxReading")->AsIntSafe(&imu->MaxReading);
		obj->GetMember("Watermark")->AsIntSafe(&imu->Watermark);
		obj->GetMember("NumBiasSamples")->AsIntSafe(&imu->NumBiasSamples);
		const IJsonArray* arr;
		if (0 != (arr = obj->GetMember("CoordinateMap")->AsArray())) {
			if (arr->ElementCount() == 9) {
				for (uint32_t i = 0; i < 9; i++) {
					imu->CoordinateMap.at(i/3,i%3)=arr->GetElement(i)->AsInt();
				}
			}
		}
	}
	return !!obj;
}

bool ParseJsonDroneConfig(
		const IJsonValue* jsonRpcParams,
		SdDroneConfig* droneCfg)
{
	const IJsonObject* config = 0;
	const IJsonObject* obj = 0;
	const IJsonArray* arr = 0;
	const IJsonObject* params = 0;

	if (jsonRpcParams->GetType() != SD_JSONVALUE_OBJECT) {
		return false;
	}
	params = jsonRpcParams->AsObject();

	if (0 == (config = params->GetMember("GlobalConfig")->AsObject())) {
		return false;
	}

	if (config->IsMemberPresent("Gyro")) {
		ParseJsonImuConfig(config->GetMember("Gyro")->AsObject(),
				&droneCfg->Gyro);
	}
	if (config->IsMemberPresent("Accelerometer")) {
		ParseJsonImuConfig(config->GetMember("Accelerometer")->AsObject(),
				&droneCfg->Accel);
	}
	if (config->IsMemberPresent("Magnetometer")) {
		ParseJsonImuConfig(config->GetMember("Magnetometer")->AsObject(),
				&droneCfg->Mag);
	}

	if (0 != (obj = config->GetMember("Servo")->AsObject())) {
		obj->GetMember("DeviceName")->AsStringSafe(&droneCfg->Servo.DeviceName);
		obj->GetMember("SamplingRate")->AsIntSafe(&droneCfg->Servo.Rate);
		obj->GetMember("ChannelMask")->AsUintSafe(&droneCfg->Servo.ChannelMask);
		obj->GetMember("BitCount")->AsUintSafe(&droneCfg->Servo.BitCount);
	}

	if (0 != (obj = config->GetMember("QuadPilot")->AsObject())) {
		obj->GetMember("Kp")->AsDoubleSafe(&droneCfg->Quad.Kp);
		obj->GetMember("Ki")->AsDoubleSafe(&droneCfg->Quad.Ki);
		obj->GetMember("Kd")->AsDoubleSafe(&droneCfg->Quad.Kd);
		if (0 != (arr = obj->GetMember("MotorToServoMap")->AsArray()) &&
			arr->ElementCount() == 4) {
			for (size_t i = 0; i < 4; i++) {
				droneCfg->Quad.Motor[i] = (uint32_t)arr->GetElement(i)->AsInt();
			}
		}

		if (0 != (arr = obj->GetMember("ImuCoordinateAngles")->AsArray()) &&
			arr->ElementCount() == 3) {
			droneCfg->Quad.ImuAngleAxisZ = arr->GetElement(2)->AsInt();
		}
	}

	if (0 != (obj = config->GetMember("ControlChannel")->AsObject())) {
		obj->GetMember("ServerAddress")->AsStringSafe(&droneCfg->ServerAddress);
		obj->GetMember("ServerPort")->AsIntSafe(&droneCfg->ServerPort);
	}
	if (config->GetMember("LogLevel")->GetType() == SD_JSONVALUE_STRING) {
		droneCfg->LogLevel = SdStringToLogLevel(
				config->GetMember("LogLevel")->AsString().c_str());
	}

	config->GetMember("LogFileName")->AsStringSafe(&droneCfg->LogFileName);
	config->GetMember("LogRotationMatrix")->AsBoolSafe(
			(bool*)&droneCfg->LogRotationMatrix);
	config->GetMember("LogRate")->AsDoubleSafe(&droneCfg->LogRate);
	return true;
}

bool ParseJsonTargetQuaternion(
		const IJsonValue* args, QuaternionD* targetQ)
{
	bool ret = false;
	if (args->GetType() == SD_JSONVALUE_ARRAY) {
		const IJsonArray* jarr = args->AsArray();
		if (jarr->ElementCount() == 4) {
			targetQ->w = jarr->GetElement(0)->AsDouble();
			targetQ->x = jarr->GetElement(1)->AsDouble();
			targetQ->y = jarr->GetElement(2)->AsDouble();
			targetQ->z = jarr->GetElement(3)->AsDouble();
			ret = true;
		}
	}
	return ret;
}

bool ParseJsonThrust(
		const IJsonValue* rpcParams,
		SdThrustValues* thrustVals)
{
	if (rpcParams->GetType() != SD_JSONVALUE_OBJECT) {
		return false;
	}
	bool ret = false;
	const IJsonObject* jobj = rpcParams->AsObject();
	double thr, min, max;
	if (jobj->GetMember("Thrust")->AsDoubleSafe(&thr)) {
		ret = true;
	} else {
		thr = thrustVals->Thrust();
	}
	if (jobj->GetMember("MaxThrust")->AsDoubleSafe(&max)) {
		ret = true;
	} else {
		max = thrustVals->MaxThrust();
	}
	if (jobj->GetMember("MinThrust")->AsDoubleSafe(&min)) {
		ret = true;
	} else {
		min = thrustVals->MinThrust();
	}
	*thrustVals = SdThrustValues(thr,min,max);
	return ret;
}

bool BuildJsonTargetQuaternion(
	SdJsonValue* jsonRpcParams,
	const QuaternionD& q)
{
	bool ret = true;
	SdJsonArray jarr;
	ret = ret && jarr.AddElement(SdJsonValue(q.w));
	ret = ret && jarr.AddElement(SdJsonValue(q.x));
	ret = ret && jarr.AddElement(SdJsonValue(q.y));
	ret = ret && jarr.AddElement(SdJsonValue(q.z));
	jsonRpcParams->SetValueAsArray(&jarr);
	return ret;
}

bool BuildJsonDroneConfig(
		SdJsonValue* jsonRpcParams,
		const SdDroneConfig& droneCfg,
		const SdJsonArray* pluginsConfig)
{
	bool ret = true;
	SdJsonObject tmpObj;
	SdJsonArray tmpArr;
	SdJsonObject jsonDroneConfig;
	SdJsonObject paramsAsObj;

	ret = ret && BuildJsonImuConfig(&tmpObj,droneCfg.Gyro);
	ret = ret && jsonDroneConfig.AddMember("Gyro",SdJsonValue(tmpObj));
	ret = ret && BuildJsonImuConfig(&tmpObj,droneCfg.Accel);
	ret = ret && jsonDroneConfig.AddMember("Accelerometer",SdJsonValue(tmpObj));
	ret = ret && BuildJsonImuConfig(&tmpObj,droneCfg.Mag);
	ret = ret && jsonDroneConfig.AddMember("Magnetometer",SdJsonValue(tmpObj));

	tmpObj.Reset();
	ret = ret && tmpObj.AddMember("DeviceName",SdJsonValue(droneCfg.Servo.DeviceName));
	ret = ret && tmpObj.AddMember("SamplingRage",SdJsonValue(droneCfg.Servo.Rate));
	ret = ret && tmpObj.AddMember("ChannelMask",SdJsonValue(droneCfg.Servo.ChannelMask));
	ret = ret && tmpObj.AddMember("BitCount",SdJsonValue(droneCfg.Servo.BitCount));
	ret = ret && jsonDroneConfig.AddMember("Servo",SdJsonValue(tmpObj));

	tmpObj.Reset();
	tmpArr.Reset();
	for (size_t i = 0; i < ARRAYSIZE(droneCfg.Quad.Motor); i++) {
		ret = ret && tmpArr.AddElement(SdJsonValue(droneCfg.Quad.Motor[i]));
	}
	ret = ret && tmpObj.AddMember("MotorToServoMap",SdJsonValue(tmpArr));
	tmpArr.Reset();
	ret = ret && tmpArr.AddElement(SdJsonValue((int32_t)0));
	ret = ret && tmpArr.AddElement(SdJsonValue((int32_t)0));
	ret = ret && tmpArr.AddElement(SdJsonValue(droneCfg.Quad.ImuAngleAxisZ));
	ret = ret && tmpObj.AddMember("ImuCoordinateAngles",SdJsonValue(tmpArr));
	ret = ret && tmpObj.AddMember("Kp",SdJsonValue(droneCfg.Quad.Kp));
	ret = ret && tmpObj.AddMember("Ki",SdJsonValue(droneCfg.Quad.Ki));
	ret = ret && tmpObj.AddMember("Kd",SdJsonValue(droneCfg.Quad.Kd));
	ret = ret && jsonDroneConfig.AddMember("QuadPilot",SdJsonValue(tmpObj));

	tmpObj.Reset();
	ret = ret && tmpObj.AddMember("ServerAddress",SdJsonValue(
			droneCfg.ServerAddress));
	ret = ret && tmpObj.AddMember("ServerPort",SdJsonValue(droneCfg.ServerPort));
	ret = ret && jsonDroneConfig.AddMember("ControlChannel",SdJsonValue(tmpObj));

	ret = ret && jsonDroneConfig.AddMember("LogLevel",
			SdLogLevelToString(droneCfg.LogLevel));
	ret = ret && jsonDroneConfig.AddMember("LogRotationMatrix",
			SdJsonValue((bool)droneCfg.LogRotationMatrix));
	ret = ret && jsonDroneConfig.AddMember("LogFileName",SdJsonValue(
			droneCfg.LogFileName));
	ret = ret && jsonDroneConfig.AddMember("LogRate",SdJsonValue(
			droneCfg.LogRate));

	ret = ret && paramsAsObj.AddMember("GlobalConfig",SdJsonValue(jsonDroneConfig));
	if (!!pluginsConfig && pluginsConfig->ElementCount()) {
		ret = ret && paramsAsObj.AddMember("Plugins", SdJsonValue(*pluginsConfig));
	}
	jsonRpcParams->SetValueAsObject(&paramsAsObj);
	return ret;
}

bool BuildJsonImuConfig(
		SdJsonObject* jobj,
		const SdImuDeviceConfig& imuCfg)
{
	bool ret = true;
	jobj->Reset();
	ret = ret && jobj->AddMember("DeviceName",SdJsonValue(imuCfg.DeviceName));
	ret = ret && jobj->AddMember("SamplingRage",SdJsonValue(imuCfg.SamplingRate));
	ret = ret && jobj->AddMember("Scale",SdJsonValue(imuCfg.Scale));
	ret = ret && jobj->AddMember("MaxReading",SdJsonValue(imuCfg.MaxReading));
	ret = ret && jobj->AddMember("Watermark",SdJsonValue(imuCfg.Watermark));
	ret = ret && jobj->AddMember("NumBiasSamples",SdJsonValue(imuCfg.NumBiasSamples));
	SdJsonArray jarr;
	for (size_t i = 0; i < 3; i++) {
		for (size_t j = 0; j < 3; j++) {
			ret = ret && jarr.AddElement(SdJsonValue(imuCfg.CoordinateMap.at(i,j)));
		}
	}
	ret = ret && jobj->AddMember("CoordinateMap",SdJsonValue(jarr));
	return ret;
}

bool BuildJsonThrustParams(
		SdJsonValue* rpcParams,
		double thrust,
		double minThrust,
		double maxThrust)
{
	bool ret = true;
	SdJsonObject jsonThrustParams;
	ret = ret && jsonThrustParams.AddMember("Thrust",SdJsonValue(thrust));
	ret = ret && jsonThrustParams.AddMember("MinThrust",SdJsonValue(minThrust));
	ret = ret && jsonThrustParams.AddMember("MaxThrust",SdJsonValue(maxThrust));
	rpcParams->SetValueAsObject(&jsonThrustParams);
	return ret;
}

bool BuildJsonPingParams(
		SdJsonValue* jsonPingParams,
		double timestamp)
{
	*jsonPingParams = timestamp;
	return jsonPingParams->SetValueAsDouble(timestamp);
}

bool BuildJsonDroneConfigFromCmdLineArgs(
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
	SdJsonArray jarrPluginConfig;

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
		if (jobjFromFile->GetMember("Plugins")->GetType() == SD_JSONVALUE_ARRAY) {
			jarrPluginConfig = *(SdJsonArray*)jobjFromFile->GetMember("Plugins")->AsArray();
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
		SdImuDeviceConfig imuConfig, imuConfig1;
		if (jobjDroneConfig.GetMember("Gyro")->GetType() == SD_JSONVALUE_OBJECT) {
			jobjGyroConfig = *(SdJsonObject*)jobjDroneConfig.GetMember("Gyro")->AsObject();
			ParseJsonImuConfig(&jobjGyroConfig,&imuConfig);
		}
		if (jobjDroneConfig.GetMember("Accelerometer")->GetType()==SD_JSONVALUE_OBJECT) {
			jobjAccConfig = *(SdJsonObject*)jobjDroneConfig.GetMember("Accelerometer")->AsObject();
		}
		if (jobjDroneConfig.GetMember("Magnetometer")->GetType()==SD_JSONVALUE_OBJECT) {
			jobjMagConfig = *(SdJsonObject*)jobjDroneConfig.GetMember("Magnetometer")->AsObject();
		}
		if (cmdArgs.GetMaxDps(&intVal)) {
			jobjGyroConfig.AddMember("Scale",SdJsonValue(intVal));
		}
		if (cmdArgs.GetImuSamplingRage(&intVal)) {
			jobjGyroConfig.AddMember("SamplingRate",SdJsonValue(intVal));
			jobjAccConfig.AddMember("SamplingRate",SdJsonValue(intVal));
			jobjMagConfig.AddMember("SamplingRate",SdJsonValue(intVal));
		}
		std::string infile = cmdArgs.GetImuInputFile();
		if (infile.length() > 0) {
			jobjGyroConfig.AddMember("DeviceName",SdJsonValue(infile));
			jobjAccConfig.AddMember("DeviceName",SdJsonValue(infile));
			jobjMagConfig.AddMember("DeviceName",SdJsonValue(infile));
		}
		if (jobjGyroConfig.GetMemberCount() > 0) {
			jobjDroneConfig.AddMember("Gyro",SdJsonValue(jobjGyroConfig));
			ParseJsonImuConfig(&jobjGyroConfig,&imuConfig1);
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
		jobjDroneConfig.AddMember("LogRate",SdJsonValue(doubleVal));
	}
	jobjDroneConfig.AddMember("LogRotationMatrix",SdJsonValue(cmdArgs.LogRotMatrix()));
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

	SdJsonObject configObj;
	configObj.AddMember("GlobalConfig",SdJsonValue(jobjDroneConfig));
	if (jarrPluginConfig.ElementCount() > 0) {
		configObj.AddMember("Plugins",SdJsonValue(jarrPluginConfig));
	}
	jsonArgs->SetValueAsObject(&configObj);
	return true;
}

bool BuildJsonThrustParamsFromCmdLineArgs(
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


}; // namespace
