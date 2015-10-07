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
#include "imulowpassfilter.h"
#include <dlfcn.h>

static double s_OneHzCutoffFiveHzPass[] =
{
		0.0136, 0.0055, 0.0065, 0.0076, 0.0087, 0.0099, 0.0110, 0.0122, 0.0133, 0.0145, 0.0155, 0.0165, 0.0174,
		0.0182, 0.0189, 0.0195, 0.0199, 0.0202, 0.0204, 0.0204, 0.0202, 0.0199, 0.0195, 0.0189, 0.0182, 0.0174,
		0.0165, 0.0155, 0.0145, 0.0133, 0.0122, 0.0110, 0.0099, 0.0087, 0.0076, 0.0065, 0.0055, 0.0136
};

static double s_FiveHzCutoff200[] =
{
		0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0005, 0.0005, 0.0005, 0.0005, 0.0006,
		0.0006, 0.0006, 0.0007, 0.0007, 0.0008, 0.0008, 0.0009, 0.0009, 0.0010, 0.0010,
		0.0011, 0.0012, 0.0013, 0.0013, 0.0014, 0.0015, 0.0016, 0.0017, 0.0018, 0.0019,
		0.0020, 0.0021, 0.0022, 0.0023, 0.0025, 0.0026, 0.0027, 0.0028, 0.0030, 0.0031,
		0.0032, 0.0034, 0.0035, 0.0037, 0.0038, 0.0040, 0.0041, 0.0043, 0.0044, 0.0046,
		0.0047, 0.0049, 0.0051, 0.0052, 0.0054, 0.0056, 0.0057, 0.0059, 0.0060, 0.0062,
		0.0064, 0.0065, 0.0067, 0.0068, 0.0070, 0.0072, 0.0073, 0.0075, 0.0076, 0.0078,
		0.0079, 0.0081, 0.0082, 0.0083, 0.0085, 0.0086, 0.0087, 0.0088, 0.0090, 0.0091,
		0.0092, 0.0093, 0.0094, 0.0095, 0.0096, 0.0097, 0.0097, 0.0098, 0.0099, 0.0099,
		0.0100, 0.0101, 0.0101, 0.0101, 0.0102, 0.0102, 0.0102, 0.0103, 0.0103, 0.0103,
		0.0103, 0.0103, 0.0103, 0.0102, 0.0102, 0.0102, 0.0101, 0.0101, 0.0101, 0.0100,
		0.0099, 0.0099, 0.0098, 0.0097, 0.0097, 0.0096, 0.0095, 0.0094, 0.0093, 0.0092,
		0.0091, 0.0090, 0.0088, 0.0087, 0.0086, 0.0085, 0.0083, 0.0082, 0.0081, 0.0079,
		0.0078, 0.0076, 0.0075, 0.0073, 0.0072, 0.0070, 0.0068, 0.0067, 0.0065, 0.0064,
		0.0062, 0.0060, 0.0059, 0.0057, 0.0056, 0.0054, 0.0052, 0.0051, 0.0049, 0.0047,
		0.0046, 0.0044, 0.0043, 0.0041, 0.0040, 0.0038, 0.0037, 0.0035, 0.0034, 0.0032,
		0.0031, 0.0030, 0.0028, 0.0027, 0.0026, 0.0025, 0.0023, 0.0022, 0.0021, 0.0020,
		0.0019, 0.0018, 0.0017, 0.0016, 0.0015, 0.0014, 0.0013, 0.0013, 0.0012, 0.0011,
		0.0010, 0.0010, 0.0009, 0.0009, 0.0008, 0.0008, 0.0007, 0.0007, 0.0006, 0.0006,
		0.0006, 0.0005, 0.0005, 0.0005, 0.0005, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004
};

ImuLowPassFilter::ImuLowPassFilter() :
		accelfilt_38_(s_OneHzCutoffFiveHzPass),
		accelfilt_200_(s_FiveHzCutoff200)
{
	runtime_ = 0;
	ref_cnt_ = 1;
}

ImuLowPassFilter::~ImuLowPassFilter()
{
	assert(0 == ref_cnt_);
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
			&runtime_);
	return err;
}

int ImuLowPassFilter::ExecuteCommand(
		SdCommandParams* params)
{
	switch (params->CommandCode()) {
	case SD_COMMAND_RUN:
		runtime_->SetIoFilters(
				SD_DEVICEID_TO_FLAG(SD_DEVICEID_IMU),
				SD_IOCODE_TO_FLAG(SD_IOCODE_RECEIVE));
		accelfilt_38_.reset();
		accelfilt_200_.reset();
		gyrofilt_.Reset();
		break;
	case SD_COMMAND_EXIT:
		runtime_->DetachPlugin();
		break;
	case SD_COMMAND_GET_ACCELEROMETER:
		params->SetOutParams(SdIoData(filtered_accel3d_));
		return SD_ESTOP_DISPATCH;
	default:break;
	}
	return SD_ESUCCESS;
}

int ImuLowPassFilter::AddRef()
{
	return __sync_fetch_and_add(&ref_cnt_,1);
}

int ImuLowPassFilter::Release()
{
	int refCnt = __sync_sub_and_fetch(&ref_cnt_,1);
	if (0 == refCnt) {
		delete this;
	}
	return refCnt;
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
	filtered_accel3d_= accelfilt_200_.do_filter(ioPacket->AccelData());
	ioPacket->SetAttribute(SDIO_ATTR_ACCEL, SdIoData(filtered_accel3d_));
	//Vector3d filteredGyro=gyrofilt_.do_filter(ioPacket->GyroData());
	//ioPacket->SetAttribute(SDIO_ATTR_GYRO, SdIoData(filteredGyro));
	return SD_ESUCCESS;
}

int ImuLowPassFilter::IoDispatchThread()
{
	assert(false);
	return EINVAL;
}
