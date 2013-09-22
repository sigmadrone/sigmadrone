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

/*
 * Aggregate class representing an instance of a drone.
 * Main responsibilities:
 * 	- factory for the rest of the objects
 * 	- Run/Reset/Diagnose
 */
class Drone
{
public:
	static Drone* Create();
	~Drone();
	int ExecuteCommand(_CommandArgs*);
	int Reset(const SdDroneConfig*,bool detachPlugins);
	int Run(_CommandArgs*);
	bool IsRunning(void);
	void PrintState(FILE* out);
	static void PrintConfig(const SdDroneConfig* config);
	static Drone* Only() { return s_Only; }

	inline const SdDroneConfig& GetCurrentConfig() { return m_config; }
	inline void SetConfig(const SdDroneConfig* cfg) { m_config = *cfg; }

	//static const double s_DefaultMinThrust = 0.3;
	//static const double s_DefaultMaxThrust = 0.99;
private:
	Drone();
	void Cleanup();
	void InitInternalPlugins();

	static int PluginAttach(
		IN void* droneContext,
		IN IPlugin* plugin,
		IN SdPluginAltitude desiredAltitude,
		IN const char* const attachAbove[],
		IN const char* const attachBelow[],
		OUT struct IPluginRuntime** pluginRuntime
		);

private:
	SdDroneConfig m_config;
	PluginChain m_pluginChain;
	bool m_isRunning;
	bool m_pluginsStarted;
	static Drone* s_Only;
};

#endif /* DRONE_H_ */
