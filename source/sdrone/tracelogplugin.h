/*
 * tracelogplugin.h
 *
 *  Created on: September 20, 2013
 *      Author: svassilev
 */

#ifndef TRACELOG_H
#define TRACELOG_H

#include "commoninc.h"

/*
 * Creates a trace log, attaches to the SD_DEVICE_ID_IMU chain
 */
class TraceLogPlugin: public IPlugin
{
public:
	TraceLogPlugin();
	int AttachToChain(void*,SdPluginAttachFunc attachPlugin);

public:
	int AddRef();
	int Release();
	int Start(const CommandArgs*);
	void Stop(int flags);
	const char* GetName();
	SdDeviceId GetDeviceId();
	const char* GetVersion();
	const char* GetDlFileName();
	int IoCallback(SdIoPacket* ioPacket);
	int IoDispatchThread();

private:
	~TraceLogPlugin();

private:
	uint32_t m_iteration;
	uint32_t m_logLevel;
	int m_logRotMatrix;
	double m_logPeriod;
	double m_timeAfterLastLog;
	IPluginRuntime* m_runtime;
	int m_refCnt;
};

#endif /* TRACELOG_H */
