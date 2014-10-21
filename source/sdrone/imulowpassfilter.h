/*
 * imubprefilter.h
 *
 *  Created on: October 5, 2013
 *      Author: svassilev
 */

#ifndef IMULPF_H
#define IMULPF_H

#include "commoninc.h"
#include "firfilt.h"

/*
 * Reads data from all the sensor devices
 * Should be able to read from a file
 */
class ImuLowPassFilter: public IPlugin
{
public:
	ImuLowPassFilter();
	int AttachToChain(void*,SdPluginAttachFunc attachPlugin);

public:
	int AddRef();
	int Release();
	int ExecuteCommand(SdCommandParams* commandArgs);
	void Stop(int flags);
	const char* GetName();
	SdDeviceId GetDeviceId();
	const char* GetVersion();
	const char* GetDlFileName();
	int IoCallback(SdIoPacket* ioPacket);
	int IoDispatchThread();

private:
	~ImuLowPassFilter();

private:
	LpPreFilter3d m_GyroFilt;
	FirFilter<double,38,3> m_AccelFilt;
	IPluginRuntime* m_Runtime;
	Vector3d m_filteredAccel3d;
	int m_RefCnt;
};

#endif /* IMULPF_H */
