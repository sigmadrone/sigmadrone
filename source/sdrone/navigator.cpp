/*
 * navigator.cpp
 *
 *  Created on: Aug 18, 2013
 *      Author: svassilev
 */

#include "navigator.h"
#include <dlfcn.h>

Navigator::Navigator()
{
	m_RefCnt = 1;
	m_TargetAttitude = QuaternionD(1,0,0,0);
	m_Runtime = 0;
}

Navigator::~Navigator()
{
	assert(0 == m_RefCnt);
}

int Navigator::AttachToChain(
		void* droneContext,
		SdPluginAttachFunc attachFunc)
{
	assert(!m_Runtime);
	int err = attachFunc(
			droneContext,
			this,
			SD_ALTITUDE_LAST_IN_GROUP(SD_ALTITUDE_GROUP_UPPER_FILTER),
			0,
			0,
			&m_Runtime);
	return err;
}

int Navigator::Start(CommandArgs* cmdArgs)
{
	assert(!!m_Runtime);
	m_Runtime->SetIoFilters(
			SD_DEVICEID_TO_FLAG(SD_DEVICEID_IMU) |
			SD_DEVICEID_TO_FLAG(SD_DEVICEID_BAR) |
			SD_DEVICEID_TO_FLAG(SD_DEVICEID_GPS) |
			SD_DEVICEID_TO_FLAG(SD_DEVICEID_COMMAND),
			SD_IOCODE_TO_FLAG(SD_IOCODE_RECEIVE)|
			SD_IOCODE_TO_FLAG(SD_IOCODE_COMMAND));
	return 0;
}

int Navigator::AddRef()
{
	return __sync_fetch_and_add(&m_RefCnt,1);
}

int Navigator::Release()
{
	int refCnt = __sync_sub_and_fetch(&m_RefCnt,1);
	if (0 == refCnt) {
		delete this;
	}
	return refCnt;
}

void Navigator::Stop(int flags)
{
	assert(!!m_Runtime);
	if (!!(flags&FLAG_STOP_AND_DETACH)) {
		m_Runtime->DetachPlugin();
	}
}

const char* Navigator::GetName()
{
	return SD_PLUGIN_NAVIGATOR;
}

SdDeviceId Navigator::GetDeviceId()
{
	return SD_DEVICEID_FILTER;
}

const char* Navigator::GetVersion()
{
	return SD_PLUGIN_VERSION;
}

extern char **g_Argv;
const char* Navigator::GetDlFileName()
{
	return g_Argv[0];
}

int Navigator::IoCallback(SdIoPacket* ioPacket)
{
	assert(!!m_Runtime);
	int ret = EINVAL;
	switch (ioPacket->DeviceId())
	{
	case SD_DEVICEID_IMU:
		if (SD_IOCODE_RECEIVE == ioPacket->IoCode()) {
			//
			// Set up the target quaternion
			//
			ioPacket->SetAttribute(SDIO_ATTR_TARGET_Q,SdIoData(&m_TargetAttitude));
		} else if (SD_IOCODE_COMMAND == ioPacket->IoCode()) {
			// TODO: the target attitude can be controlled here
		}
		ret = SD_ESUCCESS;
		break;
	case SD_DEVICEID_GPS: // TODO
	case SD_DEVICEID_BAR: // TODO
	case SD_DEVICEID_COMMAND:
		ret = SD_ESUCCESS;
		break;
	default:;
	}
	return ret;
}
