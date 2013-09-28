/*
 * imufilterplugin.cpp
 *
 *  Created on: Aug 19, 2013
 *      Author: svassilev
 */

#include "imufilterplugin.h"

ImuFilterPlugin::ImuFilterPlugin()
{
	m_RefCnt = 1;
	m_Runtime = 0;
	m_SetEarthG = false;
}

ImuFilterPlugin::~ImuFilterPlugin()
{
	assert(0 == m_RefCnt);
}

int ImuFilterPlugin::AttachToChain(
		void* droneContext,
		SdPluginAttachFunc attachFunc)
{
	int err = attachFunc(
			droneContext,
			this,
			SD_ALTITUDE_GROUP_FILTER,
			0,
			0,
			&m_Runtime);
	return err;
}

int ImuFilterPlugin::Start(CommandArgs* cmdArgs)
{
	m_ImuFilter.Reset();
	m_SetEarthG = true;
	m_Runtime->SetIoFilters(
			SD_DEVICEID_TO_FLAG(SD_DEVICEID_IMU),
			SD_IOCODE_TO_FLAG(SD_IOCODE_RECEIVE));
	return 0;
}

int ImuFilterPlugin::AddRef()
{
	return __sync_fetch_and_add(&m_RefCnt,1);
}

int ImuFilterPlugin::Release()
{
	int refCnt = __sync_sub_and_fetch(&m_RefCnt,1);
	if (0 == refCnt) {
		delete this;
	}
	return refCnt;
}

void ImuFilterPlugin::Stop(int flags)
{
	if (!!(flags&FLAG_STOP_AND_DETACH)) {
		m_Runtime->DetachPlugin();
	}
}

const char* ImuFilterPlugin::GetName()
{
	return SD_PLUGIN_IMU_FILTER;
}

SdDeviceId ImuFilterPlugin::GetDeviceId()
{
	return SD_DEVICEID_FILTER;
}

const char* ImuFilterPlugin::GetVersion()
{
	return SD_PLUGIN_VERSION;
}

extern char **g_Argv;
const char* ImuFilterPlugin::GetDlFileName()
{
	return g_Argv[0];
}


int ImuFilterPlugin::IoCallback(SdIoPacket* ioPacket)
{
	assert(SD_DEVICEID_IMU==ioPacket->deviceId);
	assert(SD_IOCODE_RECEIVE == ioPacket->ioCode);
	if (0 == ioPacket->accelData || 0 == ioPacket->gyroDataDps || 0 == ioPacket->magData) {
		return EINVAL;
	}
	if (m_SetEarthG) {
		m_ImuFilter.SetEarthG(*ioPacket->earthG);
		m_SetEarthG = false;
	}
	ioPacket->attitudeQ = &(m_ImuFilter.Update(
			*ioPacket->gyroDataDps,
			*ioPacket->accelData,
			*ioPacket->magData,
			ioPacket->deltaTime));
	return SD_ESUCCESS;
}
