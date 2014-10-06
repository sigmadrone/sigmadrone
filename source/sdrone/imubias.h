/*
 * imubias.h
 *
 *  Created on: August 16, 2013
 *      Author: svassilev
 */

#ifndef IMUBIAS_H
#define IMUBIAS_H

#include "commoninc.h"

/*
 * Reads data from all the sensor devices
 * Should be able to read from a file
 */
class ImuBias: public IPlugin
{
public:
	ImuBias();
	int AttachToChain(void*,SdPluginAttachFunc attachPlugin);

public:
	int AddRef();
	int Release();
	int ExecuteCommand(SdCommandParams* commandArgs);
	const char* GetName();
	SdDeviceId GetDeviceId();
	const char* GetVersion();
	const char* GetDlFileName();
	int IoCallback(SdIoPacket* ioPacket);
	int IoDispatchThread();

private:
	~ImuBias();
	int Start(const SdDroneConfig* droneConfig);

private:
	Vector3d m_EarthG;
	Vector3d m_GyroBias;
	uint32_t m_CurrentBiasSamples;
	double m_ElapsedTime;
	double m_TotalBiasCalcTime;
	IPluginRuntime* m_Runtime;
	int m_RefCnt;
};

#endif /* IMUBIAS_H */
