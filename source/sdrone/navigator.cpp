/*
 *  Sigmadrone
 *  Copyright (c) 2013-2015 The Sigmadrone Developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Martin Stoilov <martin@sigmadrone.org>
 *  Svetoslav Vassilev <svassilev@sigmadrone.org>
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

int Navigator::ExecuteCommand(
		SdCommandParams* params)
{
	int err = SD_ESUCCESS;
	switch (params->CommandCode()) {
	case SD_COMMAND_RUN:
		m_Runtime->SetIoFilters(
				SD_DEVICEID_TO_FLAG(SD_DEVICEID_IMU) |
				SD_DEVICEID_TO_FLAG(SD_DEVICEID_BAR) |
				SD_DEVICEID_TO_FLAG(SD_DEVICEID_GPS),
				SD_IOCODE_TO_FLAG(SD_IOCODE_RECEIVE));
		break;
	case SD_COMMAND_RESET:
		break;
	case SD_COMMAND_EXIT:
		m_Runtime->DetachPlugin();
		break;
	case SD_COMMAND_SET_ATTITUDE:
		m_TargetAttitude = params->Params().asQuaternion();
		break;
	default:break;
	}
	return err;
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
	int ret = SD_ESUCCESS;
	switch (ioPacket->DeviceId())
	{
	case SD_DEVICEID_IMU:
		if (SD_IOCODE_RECEIVE == ioPacket->IoCode()) {
			//
			// Set up the target quaternion
			//
			ioPacket->SetAttribute(SDIO_ATTR_TARGET_Q,SdIoData(m_TargetAttitude));
		}
		break;
	case SD_DEVICEID_GPS: // TODO
	case SD_DEVICEID_BAR: // TODO
	default:break;
	}
	return ret;
}
