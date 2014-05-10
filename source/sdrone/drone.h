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
#include "dronedefs.h"
#include "pluginchain.h"
#include "commandargs.h"
#include "filelock.h"

class SdJsonRpcDispatcher;
struct SdJsonRpcReply;
struct SdJsonRpcRequest;
struct IRpcTransport;

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

	int Run(const _CommandArgs& args);

	inline bool IsRunning(void) { return m_isRunning; }

private:
	Drone();
	~Drone();
	void InitInternalPlugins();
	int OnExit();
	int OnFly();
	int OnReset();

//	inline void SetConfig(const SdDroneConfig* cfg) { m_config = *cfg; }

	static void OnRpcCommandFly(
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
	static void OnRpcCommandGetState(
				void* Context,
				const SdJsonRpcRequest*,
				SdJsonRpcReply*);
	static void PrintConfig(const SdDroneConfig* config);

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
	_CommandArgs m_commandArgs;
	SdJsonRpcDispatcher* m_rpcDispatch;
	IRpcTransport* m_rpcTransport;
	PluginChain m_pluginChain;
	FileLock m_globalLock;
	bool m_isRunning;
	static Drone* s_Only;
};

#endif /* DRONE_H_ */
