/*
 * plugincontext.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: svassilev
 */

#include "plugincontext.h"
#include <dlfcn.h>
#include "pluginchain.h"
#include "iopacket.h"

static void* s_myModuleHandle = (void*)-1;
extern char **g_Argv;
extern _CommandArgs g_CmdArgs;

PluginContext::PluginContext(
		IPlugin* plugin,
		PluginChain* chain,
		SdPluginAltitude grantedAltitude,
		const std::string& attachAbove,
		const std::string& attachBelow) :

		m_plugin(plugin),
		m_chain(chain),
		m_altitude(grantedAltitude),
		m_lowerDependencies(attachAbove),
		m_upperDependencies(attachBelow),
		m_deviceFilter(0),
		m_ioFilter(0),
		m_dispatchThread(0),
		m_doDispatchIo(false),
		m_logLevel(SD_LOG_LEVEL_INFO)
{
	m_refCnt = 1;
	if (0 == strcmp(m_plugin->GetDlFileName(),g_Argv[0])) {
		m_dllHandle = s_myModuleHandle;
	} else {
		m_dllHandle = dlopen(m_plugin->GetDlFileName(),RTLD_LAZY);
	}
	if (!m_dllHandle) {
		const char* err = dlerror();
		printf("ERR: Failed to open dll %s for plugin %s, %s\n",
				m_plugin->GetDlFileName(), m_plugin->GetName(),
				!!err ? err : strerror(errno));
	}
	m_plugin->AddRef();
}

PluginContext::~PluginContext()
{
	assert(0 == m_refCnt);
	StartStopIoDispatchThread(false);
	m_plugin->Release();
	if (0 != m_dllHandle && s_myModuleHandle != m_dllHandle) {
		dlclose(m_dllHandle);
	}
}

void PluginContext::DetachPlugin()
{
	AddRef();
	m_chain->PluginDetach(this);
	StartStopIoDispatchThread(false);
	Release();
}

int PluginContext::StartStopIoDispatchThread(bool start) {
	int err = 0;
	if (start) {
		if (0 != m_dispatchThread) {
			printf("WARN: IO dispatch thread for %s already started\n",
					m_plugin->GetName());
			err = EINVAL;
			goto __return;
		}
		m_doDispatchIo = true;
		err = pthread_create(&m_dispatchThread,0,DispatchThread,this);
		if (0 != err) {
			m_doDispatchIo = false;
			printf("ERR: failed to create thread for %s %s\n",
					m_plugin->GetName(),strerror(err));
		}
	} else {
		m_doDispatchIo = false;
		if (0 != m_dispatchThread) {
			if (pthread_self() != m_dispatchThread) {
				printf("Waiting %s IO dispatch thread to terminate\n",
						m_plugin->GetName());
				pthread_join(m_dispatchThread,0);
			} else {
				printf("WARN: %s is attempting to stop IO dispatch thread "
						"from the same thread context\n",m_plugin->GetName());
			}
		}
	}

	__return:
	return err;
}

int PluginContext::DispatchIo(
		SdIoPacket* iop,
		uint32_t dispatchFlags)
{
	assert(iop);
	return m_chain->DispatchIo(this,iop,dispatchFlags);
}

void* PluginContext::DispatchThread(void* arg)
{
	PluginContext* ctx = reinterpret_cast<PluginContext*>(arg);
	int err = SD_ESUCCESS;
	printf("Entered %s IO dispatch thread\n",ctx->m_plugin->GetName());
	ctx->AddRef();
	while (ctx->m_doDispatchIo && SD_ESUCCESS == err) {
		err = ctx->m_plugin->IoDispatchThread();
	}
	ctx->m_dispatchThread = 0;
	printf("Exit %s IO dispatch thread\n",ctx->m_plugin->GetName());
	ctx->Release();
	return 0;
}


void PluginContext::SetIoFilters(
		SdDeviceId deviceTypeMask,
		uint32_t ioCodeMask)
{
	m_deviceFilter = deviceTypeMask;
	m_ioFilter = ioCodeMask;
}

SdDroneType PluginContext::GetSdDroneType()
{
	return SD_DRONE_TYPE_QUADROTOR;
}

SdPluginAltitude PluginContext::GetMyAltitude()
{
	return m_altitude;
}

const std::string& PluginContext::GetLowerDependencies(void)
{
	return m_lowerDependencies;
}

const std::string& PluginContext::GetUpperDependencies()
{
	return m_upperDependencies;
}

bool PluginContext::IsInitialized()
{
	return !!m_dllHandle;
}

int PluginContext::AddRef()
{
	return __sync_fetch_and_add(&m_refCnt,1);
}

int PluginContext::Release()
{
	int refCnt = __sync_sub_and_fetch(&m_refCnt,1);
	if (0 == refCnt) {
		delete this;
	}
	return refCnt;
}

void PluginContext::Log(
		int logLevel,
		const char* format,
		...)
{
	va_list args;
	va_start (args, format);
	Logv(logLevel,format,args);
	va_end (args);
}

void PluginContext::Logv(
	int logLevel,
	const char* format,
	va_list args)
{
	if (logLevel > m_logLevel) {
		return;
	}
	vfprintf(stdout,format,args);
}

SdDeviceId PluginContext::GetPluginDeviceFilter()
{
	return m_deviceFilter;
}

uint32_t PluginContext::GetPluginIoCodeFilter()
{
	return m_ioFilter;
}

SdIoPacket* PluginContext::AllocIoPacket(
		uint32_t ioCode,
		SdDeviceId deviceType,
		const char* pluginName)
{
	SdIoPacket* ioPacket = new IoPacket(m_altitude,ioCode,
			deviceType,pluginName);
	if (0 == ioPacket) {
		printf("ERROR: failed to alloc packet,  ioCode %d, "
				"plugin %s\n", ioCode, pluginName);
	}
	return ioPacket;
}

void PluginContext::FreeIoPacket(
		SdIoPacket* iop)
{
	delete static_cast<IoPacket*>(iop);
}
