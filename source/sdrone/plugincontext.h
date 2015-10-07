/*
 *  Sigmadrone
 *  Copyright (c) 2013-2015 The Sigmadrone Developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Martin Stoilov <martin@sigmadrone.org>
 *  Svetoslav Vassilev <svassilev@sigmadrone.org>
 */

#ifndef PLUGINCONTEXT_H_
#define PLUGINCONTEXT_H_

#include "commoninc.h"
#include <string>
#include <pthread.h>

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
			SdDeviceId deviceTypeMask,
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
	SdIoPacket* AllocIoPacket(
			uint32_t ioCode,
			SdDeviceId deviceType,
			const char* pluginName
			);
	void FreeIoPacket(
			SdIoPacket*
			);

	const std::string& GetLowerDependencies();
	const std::string& GetUpperDependencies();
	bool IsInitialized();
	SdDeviceId GetPluginDeviceFilter();
	uint32_t GetPluginIoCodeFilter();

	void ExecuteCommandNotify(SdCommandParams* args);
	IPlugin* Plugin() { return m_plugin; }

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
	SdDeviceId m_deviceFilter;
	uint32_t m_ioFilter;
	pthread_t m_dispatchThread;
	int m_refCnt;
	volatile bool m_doDispatchIo;
	int m_logLevel;
};

#endif /* PLUGINCONTEXT_H_ */
