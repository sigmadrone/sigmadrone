/*
 * imureader.h
 *
 *  Created on: August 16, 2013
 *      Author: svassilev
 */

#ifndef IMUREADER_H_
#define IMUREADER_H_

#include <boost/scoped_ptr.hpp>
#include "commoninc.h"
#include "sampler.h"
#include "filesampler.h"

/*
 * Reads data from all the sensor devices
 * Should be able to read from a file
 */
class ImuReader: public IPlugin
{
public:
	ImuReader();
	int AttachToChain(void*,SdPluginAttachFunc attachPlugin);

public:
	int AddRef();
	int Release();
	int Run(const SdDroneConfig* config);
	const char* GetName();
	SdDeviceId GetDeviceId();
	const char* GetVersion();
	const char* GetDlFileName();
	int IoCallback(SdIoPacket* ioPacket);
	int IoDispatchThread();
	int ExecuteCommand(SdCommandParams*);

private:
	double DeltaT();
	void Close();
	~ImuReader();

private:
	SdImuDeviceConfig m_GyroConfig;
	IPluginRuntime* m_RunTime;
	int m_RefCnt;
	FILE* m_SensorLog;
	timespec m_LastTime;
	uint32_t m_Counter;
	boost::scoped_ptr<file_sampler> m_FileSampler;
	boost::scoped_ptr<sampler> m_Sampler;
};

#endif /* IMUREADER_H_ */
