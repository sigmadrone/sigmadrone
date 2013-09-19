/*
 * plugincontext.h
 *
 *  Created on: Aug 15, 2013
 *      Author: svassilev
 */

#ifndef PLUGINCONTEXT_H_
#define PLUGINCONTEXT_H_

#include "commoninc.h"
#include <string>
#include <pthread.h>
#include "pluginapi.h"

class PluginChain;

class PluginContext: public IPluginRuntime
{
public:
	PluginContext(
			IPlugin* plugin,
			PluginChain* chain,
			SdPluginAltitude grantedAltitude,
			const std::string& attachAbove,
			const std::string& attachBelow);
	void DetachPlugin();
	int StartStopIoDispatchThread(
			bool start
			);
	void SetIoFilters(
			uint32_t deviceTypeMask,
			uint32_t ioCodeMask);
	int DispatchIo(
			IN OUT SdIoPacket* iop,
			uint32_t dispatchFlags
			);
	SdDroneType GetSdDroneType();
	SdPluginAltitude GetMyAltitude();
	SdPluginAltitude GetPluginAltitude(
			IN IPlugin*
			);
	int AddRef();
	int Release();
	void Log(
			int severity,
			const char* format,
			...
			);
	void Logv(
			int severity,
			const char* format,
			va_list args
			);

	const std::string& GetLowerDependencies();
	const std::string& GetUpperDependencies();
	bool IsInitialized();
	uint32_t GetPluginDeviceFilter();
	uint32_t GetPluginIoCodeFilter();

private:
	virtual ~PluginContext();
	static void* DispatchThread(void* arg);

private:
	friend class PluginChain;
	IPlugin* m_plugin;
	PluginChain* m_chain;
	void* m_dllHandle;
	SdPluginAltitude m_altitude;
	std::string m_lowerDependencies;
	std::string m_upperDependencies;
	uint32_t m_deviceFilter;
	uint32_t m_ioFilter;
	pthread_t m_dispatchThread;
	int m_refCnt;
	bool m_doDispatchIo;
};

#endif /* PLUGINCONTEXT_H_ */
