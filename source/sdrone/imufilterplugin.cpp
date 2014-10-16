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
	m_earthG = Vector3d(0,0,1.0);
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

int ImuFilterPlugin::ExecuteCommand(
		SdCommandParams* params)
{
	switch (params->CommandCode()) {
	case SD_COMMAND_RUN:
		m_attitude.reset_attitude();
		m_Runtime->SetIoFilters(
				SD_DEVICEID_TO_FLAG(SD_DEVICEID_IMU),
				SD_IOCODE_TO_FLAG(SD_IOCODE_RECEIVE));
		break;
	case SD_COMMAND_EXIT:
		m_Runtime->DetachPlugin();
		break;
	case SD_COMMAND_SET_EARTH_G_VECTOR:
		assert(params->Params().dataType == SdIoData::TYPE_VECTOR3D);
		m_earthG = *(params->Params().asVector3d);
		m_attitude.set_earth_g(m_earthG);
		break;
	case SD_COMMAND_GET_EARTH_G_VECTOR:
		params->SetOutParams(SdIoData(&m_earthG));
		break;
	default:break;
	}
	return SD_ESUCCESS;
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
	if (SD_DEVICEID_IMU == ioPacket->DeviceId() &&
			SD_IOCODE_RECEIVE == ioPacket->IoCode())
	{
		const SdIoData& ioData = ioPacket->GetIoData(true);
		if (ioData.asImuData->gyro3d_upd)
			m_attitude.track_gyroscope(DEG2RAD(ioPacket->GyroData()), ioPacket->DeltaTime());
		if (ioData.asImuData->acc3d_upd)
			m_attitude.track_accelerometer(ioPacket->AccelData());
		if (ioData.asImuData->mag3d_upd)
			m_attitude.track_magnetometer(ioPacket->MagData());
		const QuaternionD& attQ = m_attitude.get_attitude();
		ioPacket->SetAttribute(SDIO_ATTR_ATTITUDE_Q,SdIoData(&attQ));
		ioPacket->SetAttribute(SDIO_ATTR_EARTH_G,SdIoData(&m_earthG));
	}
	return SD_ESUCCESS;
}
