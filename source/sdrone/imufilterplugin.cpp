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
	assert(SD_DEVICEID_IMU == ioPacket->DeviceId());
	assert(SD_IOCODE_RECEIVE == ioPacket->IoCode());

	if (m_SetEarthG) {
		SdIoData earthG = ioPacket->GetAttribute(SDIO_ATTR_EARTH_G);
		if (earthG.dataType == SdIoData::TYPE_VECTOR3D) {
			m_ImuFilter.SetEarthG(*earthG.asVector3d);
			m_SetEarthG = false;
		} else {
			m_Runtime->Log(SD_LOG_LEVEL_ERROR,
					"ImuFilterPlugin: earth G was expected!\n");
			return EINVAL;
		}
	}

	const QuaternionD& attQ = m_ImuFilter.Update(
			ioPacket->GyroData(),
			ioPacket->AccelData(),
			ioPacket->MagData(),
			ioPacket->DeltaTime());
	ioPacket->SetAttribute(SDIO_ATTR_ATTITUDE_Q,SdIoData(&attQ));
	ioPacket->SetAttribute(SDIO_ATTR_CORR_VELOCITY,
			SdIoData(&(m_ImuFilter.GetVg())));
	return SD_ESUCCESS;
}
