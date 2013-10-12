/*
 * ImuLowPassFilter.cpp
 *
 *  Created on: August 16, 2013
 *      Author: svassilev
 */

#include "commoninc.h"
#include "imulowpassfilter.h"
#include <dlfcn.h>

static double s_OneHzCutoffFiveHzPass[] =
{
		0.0136, 0.0055, 0.0065, 0.0076, 0.0087, 0.0099, 0.0110, 0.0122, 0.0133, 0.0145, 0.0155, 0.0165, 0.0174,
		0.0182, 0.0189, 0.0195, 0.0199, 0.0202, 0.0204, 0.0204, 0.0202, 0.0199, 0.0195, 0.0189, 0.0182, 0.0174,
		0.0165, 0.0155, 0.0145, 0.0133, 0.0122, 0.0110, 0.0099, 0.0087, 0.0076, 0.0065, 0.0055, 0.0136
};

ImuLowPassFilter::ImuLowPassFilter() :
		m_AccelFilt(s_OneHzCutoffFiveHzPass)
{
	m_Runtime = 0;
	m_RefCnt = 1;
}

ImuLowPassFilter::~ImuLowPassFilter()
{
	assert(0 == m_RefCnt);
}

int ImuLowPassFilter::AttachToChain(
		void* droneContext,
		SdPluginAttachFunc attachFunc)
{
	int err = attachFunc(
			droneContext,
			this,
			SD_ALTITUDE_GROUP_LOWER_FILTER,
			0,
			0,
			&m_Runtime);
	return err;
}

int ImuLowPassFilter::Start(
	CommandArgs* cmdArgs)
{
	m_Runtime->SetIoFilters(
		SD_DEVICEID_TO_FLAG(SD_DEVICEID_IMU),
		SD_IOCODE_TO_FLAG(SD_IOCODE_RECEIVE));
	m_AccelFilt.Reset();
	m_GyroFilt.Reset();
	return SD_ESUCCESS;
}

int ImuLowPassFilter::AddRef()
{
	return __sync_fetch_and_add(&m_RefCnt,1);
}

int ImuLowPassFilter::Release()
{
	int refCnt = __sync_sub_and_fetch(&m_RefCnt,1);
	if (0 == refCnt) {
		delete this;
	}
	return refCnt;
}

void ImuLowPassFilter::Stop(int flags)
{
	if (!!(flags&FLAG_STOP_AND_DETACH)) {
		m_Runtime->DetachPlugin();
	}
}

const char* ImuLowPassFilter::GetName()
{
	return SD_PLUGIN_IMU_LOWPASSFILTER;
}

SdDeviceId ImuLowPassFilter::GetDeviceId()
{
	return SD_DEVICEID_FILTER;
}

const char* ImuLowPassFilter::GetVersion()
{
	return SD_PLUGIN_VERSION;
}

extern char **g_Argv;
const char* ImuLowPassFilter::GetDlFileName()
{
	return g_Argv[0];
}

int ImuLowPassFilter::IoCallback(
	SdIoPacket* ioPacket)
{
	if (!!ioPacket->accelData && !!ioPacket->gyroDataDps && !!ioPacket->magData) {
		//m_AccelData = m_AccelFilt.DoFilter(*ioPacket->accelData);
		double arr[3] = {ioPacket->accelData->at(0,0),ioPacket->accelData->at(1,0),
				ioPacket->accelData->at(2,0)};
		m_AccelFilt.DoFilter(arr);
		m_AccelData.at(0,0) = m_AccelFilt.GetOutput()[0];
		m_AccelData.at(1,0) = m_AccelFilt.GetOutput()[1];
		m_AccelData.at(2,0) = m_AccelFilt.GetOutput()[2];
		m_AccelData = m_AccelData.normalize();
		ioPacket->accelData = &m_AccelData;
	}
	return SD_ESUCCESS;
}

int ImuLowPassFilter::IoDispatchThread()
{
	assert(false);
	return EINVAL;
}
