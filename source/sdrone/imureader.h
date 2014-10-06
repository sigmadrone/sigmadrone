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
	void CloseSensorLog();
	~ImuReader();

private:
	IPluginRuntime* m_RunTime;
	int m_refCnt;
	FILE* m_sensorLog;
	timespec m_lastTime;
	uint32_t m_counter;
	boost::scoped_ptr<file_sampler> m_fileSampler;
	boost::scoped_ptr<sampler> m_sampler;
};

#endif /* IMUREADER_H_ */
