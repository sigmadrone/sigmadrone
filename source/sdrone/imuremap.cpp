/*
 * ImuRemap.cpp
 *
 *  Created on: August 16, 2013
 *      Author: svassilev
 */

#include "commoninc.h"
#include "imuremap.h"
#include <dlfcn.h>

ImuRemap::ImuRemap()
{
	m_Runtime = 0;
	m_RefCnt = 1;
}

ImuRemap::~ImuRemap()
{
	assert(0 == m_RefCnt);
}

int ImuRemap::AttachToChain(
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

int ImuRemap::Start(
	CommandArgs* cmdArgs)
{
	const SdDroneConfig* droneConfig = cmdArgs->GetDroneConfig();
	m_AccelMap = droneConfig->AccMap;
	m_GyroMap = droneConfig->GyroMap;
	m_MagMap = droneConfig->MagMap;
	m_Runtime->SetIoFilters(
		SD_DEVICEID_TO_FLAG(SD_DEVICEID_IMU),
		SD_IOCODE_TO_FLAG(SD_IOCODE_RECEIVE));
	return SD_ESUCCESS;
}

int ImuRemap::AddRef()
{
	return __sync_fetch_and_add(&m_RefCnt,1);
}

int ImuRemap::Release()
{
	int refCnt = __sync_sub_and_fetch(&m_RefCnt,1);
	if (0 == refCnt) {
		delete this;
	}
	return refCnt;
}

void ImuRemap::Stop(int flags)
{
	if (!!(flags&FLAG_STOP_AND_DETACH)) {
		m_Runtime->DetachPlugin();
	}
}

const char* ImuRemap::GetName()
{
	return SD_PLUGIN_IMU_REMAP;
}

SdDeviceId ImuRemap::GetDeviceId()
{
	return SD_DEVICEID_FILTER;
}

const char* ImuRemap::GetVersion()
{
	return SD_PLUGIN_VERSION;
}

extern char **g_Argv;
const char* ImuRemap::GetDlFileName()
{
	return g_Argv[0];
}


int ImuRemap::IoCallback(
	SdIoPacket* ioPacket)
{
	if (!!ioPacket->accelData && !!ioPacket->gyroDataDps && !!ioPacket->magData)
	{
		m_MagData = m_MagMap * (*ioPacket->magData);
		m_AccelData = m_AccelMap * (*ioPacket->accelData);
		m_GyroData = m_GyroMap * (*ioPacket->gyroDataDps);
		ioPacket->magData = &m_MagData;
		ioPacket->gyroDataDps = &m_GyroData;
		ioPacket->accelData = &m_AccelData;
	}
	return SD_ESUCCESS;
}

int ImuRemap::IoDispatchThread()
{
	assert(false);
	return EINVAL;
}
