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
#include "pluginregistry.h"
#include "commandargs.h"

namespace RpcParams {

bool ParseJsonDroneConfig(
		const IJsonValue* jsonRpcParams,
		SdDroneConfig*);
bool ParseJsonImuConfig(
		const IJsonObject* jsonImuObj,
		SdImuDeviceConfig*);
bool ParseJsonThrust(
		const SdJsonValue& jsonRpcParams,
		SdThrustValues*);
QuaternionD ParseJsonQuaternion(
		const SdJsonValue& jsonRpcParams);
SdJsonValue BuildJsonQuaternion(
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

struct PluginBuilder{
	PluginBuilder(const PluginInfo& pl);
	SdJsonValue Get(SdPluginAltitude altitude = SD_ALTITUDE_INVALID);
private:
	const PluginInfo& plugin_;
};

struct PluginParser {
	PluginParser(const SdJsonValue& jval);
	PluginInfo Get();
private:
	const SdJsonValue& value_;
};

template <int N>
SdJsonValue BuildJsonVector(
	const MatrixMN<double,N,1>& vec)
{
	SdJsonArray jarr;
	for (int i = 0; i < N; ++i) {
		jarr.AddElement(SdJsonValue(vec.at(i,0)));
	}
	return SdJsonValue(jarr);
}

inline SdJsonValue BuildJsonVector3d(const Vector3d& v) { return BuildJsonVector(v); }
inline SdJsonValue BuildJsonVector4d(const Vector4d& v) { return BuildJsonVector(v); }

template <int N>
MatrixMN<double,N,1> ParseJsonVector(
		const SdJsonValue& jval)
{
	MatrixMN<double,N,1> v;
	if (jval.GetType() == SD_JSONVALUE_ARRAY && jval.Array().ElementCount() == N) {
		for (int i = 0; i < N; ++i) {
			v.at(i,0) = jval.Array().Element(i).AsDouble();
		}
	}
	return v;
}

inline Vector3d ParseJsonVector3d(const SdJsonValue& jval) { return ParseJsonVector<3>(jval);}
inline Vector4d ParseJsonVector4d(const SdJsonValue& jval) { return ParseJsonVector<4>(jval);}

};

#endif /* RPCPARAMS_H_ */
