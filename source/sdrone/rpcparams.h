/*
 * rpcparams.h
 *
 *  Created on: Aug 27, 2014
 *      Author: svassilev
 */

#ifndef RPCPARAMS_H_
#define RPCPARAMS_H_

#include "jsonreadwrite.h"
#include "sdroneapi.h"
#include "commandargs.h"

namespace RpcParams {

bool ParseJsonDroneConfig(
		const IJsonValue* jsonRpcParams,
		SdDroneConfig*);
bool ParseJsonImuConfig(
		const IJsonObject* jsonImuObj,
		SdImuDeviceConfig*);
bool ParseJsonThrust(
		const IJsonValue* jsonRpcParams,
		SdThrustValues*);
bool ParseJsonTargetQuaternion(
		const IJsonValue* jsonRpcParams,
		QuaternionD*);
bool BuildJsonTargetQuaternion(
		SdJsonValue* jsonRpcParams,
		const QuaternionD&);
bool BuildJsonDroneConfig(
		SdJsonValue* jsonRpcParams,
		const SdDroneConfig&,
		const SdJsonArray* pluginsConfig);
bool BuildJsonImuConfig(
		SdJsonObject* jsonImuConfig,
		const SdImuDeviceConfig&);
bool BuildJsonThrustParams(
		SdJsonValue* jsonFlightParams,
		double thrust,
		double minThrust,
		double maxThrust);
bool BuildJsonPingParams(
		SdJsonValue* jsonPing,
		double timestamp);
bool BuildJsonDroneConfigFromCmdLineArgs(
		SdJsonValue* jsonArgs,
		const CommandLineArgs& cmdArgs);
bool BuildJsonThrustParamsFromCmdLineArgs(
		SdJsonValue* jsonArgs,
		const CommandLineArgs&);
};

#endif /* RPCPARAMS_H_ */