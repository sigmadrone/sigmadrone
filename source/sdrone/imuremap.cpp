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

int ImuRemap::ExecuteCommand(
		SdCommandParams* params)
{
	int err = SD_ESUCCESS;
	switch (params->CommandCode()) {
	case SD_COMMAND_RUN:
		err = Start(&params->Params().asDroneConfig());
		break;
	case SD_COMMAND_EXIT:
		m_Runtime->DetachPlugin();
		break;
	default:break;
	}
	return err;
}

int ImuRemap::Start(const SdDroneConfig* droneConfig)
{
	m_AccelMap = droneConfig->Accel.CoordinateMap;
	m_GyroMap = droneConfig->Gyro.CoordinateMap;
	m_MagMap = droneConfig->Mag.CoordinateMap;
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
	Vector3d accelData = m_AccelMap * ioPacket->AccelData();
	Vector3d gyroData = m_GyroMap * ioPacket->GyroData();
	Vector3d magData = m_MagMap * ioPacket->MagData();
	ioPacket->SetAttribute(SDIO_ATTR_ACCEL,SdIoData(accelData));
	ioPacket->SetAttribute(SDIO_ATTR_GYRO,SdIoData(gyroData));
	ioPacket->SetAttribute(SDIO_ATTR_MAG,SdIoData(magData));
	return SD_ESUCCESS;
}

int ImuRemap::IoDispatchThread()
{
	assert(false);
	return EINVAL;
}
