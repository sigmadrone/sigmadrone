/*
 * drone.h
 *
 *  Created on: May 10, 2013
 *      Author: svassilev
 */

#ifndef DRONE_H_
#define DRONE_H_

#include <pthread.h>
#include "axesdata.h"
#include "pluginchain.h"
#include "commandargs.h"
#include "filelock.h"
#include "droneconfig.h"
#include "pluginregistry.h"

class SdJsonRpcDispatcher;
struct SdJsonRpcReply;
struct SdJsonRpcRequest;
struct IRpcTransport;
class SdJsonArray;
class SdJsonObject;
class SdJsonValue;

/*
 * Aggregate class representing an instance of a drone.
 * Main responsibilities:
 * 	- Serves the RPC commands
 * 	- Indirectly manages plugins by invoking PluginChain methods
 */
class Drone
{
public:
	static Drone* Create();
	static void Destroy();
	static Drone* Only() { return s_Only; }

	int Run(CommandLineArgs& args);

	inline bool IsRunning(void) { return m_isRunning; }

private:
	Drone();
	~Drone();
	void InitPlugins(const SdJsonValue& cmdLineArgs);
	int OnExit();
	int OnRun(const IJsonValue* rpcParams);
	int OnReset();
	int OnSetThrust(const SdThrustValues&);
	int OnSetConfig(const SdDroneConfig&, const IJsonValue* rpcParams);
	int OnSetTargetQuaternion(const QuaternionD&);
	void LoadUnloadPlugins();

	static void OnRpcCommandRun(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandExit(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandReset(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandSetConfig(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandGetConfig(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandPing(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandSetThrust(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandSetSafeThrust(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandGetThrust(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandSetTargetAttitude(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandGetAttitude(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandSetTargetAltitude(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandGetAltitude(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandGetRpcSpec(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandGetRpcList(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandUnloadPlugin(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandLoadPlugin(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandQueryPlugins(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandSetGVector(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandGetGVector(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandIsRunning(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandGetMotors(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandSetMotors(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	static void OnRpcCommandGetAccelerometer(
			void* Context,
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);

	static void fatal_error_signal(int sig);

	static int PluginAttach(
		IN void* droneContext,
		IN IPlugin* plugin,
		IN SdPluginAltitude desiredAltitude,
		IN const char* const attachAbove[],
		IN const char* const attachBelow[],
		OUT struct IPluginRuntime** pluginRuntime
		);

private:
	SdJsonRpcDispatcher* m_rpcDispatch;
	IRpcTransport* m_rpcTransport;
	PluginChain m_pluginChain;
	FileLock m_globalLock;
	DroneConfig m_droneConfig;
	SdThrustValues m_thrustValues;
	PluginRegistry m_pluginReg;
	bool m_isRunning;
	bool m_safeThrust;
	static Drone* s_Only;
};

#endif /* DRONE_H_ */
