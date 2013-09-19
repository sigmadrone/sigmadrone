/*
 * imubias.cpp
 *
 *  Created on: August 16, 2013
 *      Author: svassilev
 */

#include "commoninc.h"
#include "imubias.h"
#include <dlfcn.h>

ImuBias::ImuBias()
{
	m_Runtime = 0;
	m_RefCnt = 1;
	m_NumBiasSamples = m_CurrentBiasSamples = 0;
}

ImuBias::~ImuBias()
{
	assert(0 == m_RefCnt);
}

int ImuBias::AttachToChain(
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

int ImuBias::Start(
	CommandArgs* cmdArgs)
{
	const SdDroneConfig* droneConfig = cmdArgs->GetSdDroneConfig();
	m_NumBiasSamples = droneConfig->Gyro.NumBiasSamples;
	m_CurrentBiasSamples = 0;
	m_Runtime->SetIoFilters(
		SD_DEVICEID_TO_FLAG(SD_DEVICEID_IMU),
		SD_IOCODE_TO_FLAG(SD_IOCODE_RECEIVE));
	return SD_ESUCCESS;
}

int ImuBias::AddRef()
{
	return __sync_fetch_and_add(&m_RefCnt,1);
}

int ImuBias::Release()
{
	int refCnt = __sync_sub_and_fetch(&m_RefCnt,1);
	if (0 == refCnt) {
		delete this;
	}
	return refCnt;
}

void ImuBias::Stop(int flags)
{
	if (!!(flags&FLAG_STOP_AND_DETACH)) {
		m_Runtime->DetachPlugin();
	}
}

const char* ImuBias::GetName()
{
	return SD_PLUGIN_IMU_BIAS;
}

SdDeviceId ImuBias::GetDeviceId()
{
	return SD_DEVICEID_FILTER;
}

const char* ImuBias::GetVersion()
{
	return SD_PLUGIN_VERSION;
}

extern char **g_Argv;
const char* ImuBias::GetDlFileName()
{
	return g_Argv[0];
}

int ImuBias::IoCallback(
	SdIoPacket* ioPacket)
{
	if (!!ioPacket->accelData && !!ioPacket->gyroDataDps && !!ioPacket->magData) {
		if (m_CurrentBiasSamples < m_NumBiasSamples) {
			m_EarthG = m_EarthG + *ioPacket->accelData;
			m_GyroBias = m_GyroBias + *ioPacket->gyroDataDps;
			++m_CurrentBiasSamples;
			return SD_ESTOP_DISPATCH;
		}
		if (m_CurrentBiasSamples == m_NumBiasSamples) {
			m_EarthG = m_EarthG / m_NumBiasSamples;
			m_GyroBias = m_GyroBias / m_NumBiasSamples;
			++m_CurrentBiasSamples; // so we will not perform the div again
		}
		m_GyroData = *ioPacket->gyroDataDps - m_GyroBias;
		ioPacket->gyroDataDps = &m_GyroData;
		ioPacket->earthG = &m_EarthG;
	}
	return SD_ESUCCESS;
}

int ImuBias::IoDispatchThread()
{
	assert(false);
	return EINVAL;
}
