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

#include "servoplugin.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <math.h>
#include "pca9685controller.h"

ServoDevicePlugin::ServoDevicePlugin()
	: ref_cnt_(1)
	, servoctrl_(new servocontroller(16))
	, runtime_(0)
{
}

ServoDevicePlugin::~ServoDevicePlugin()
{
	assert(0 == ref_cnt_);
}

int ServoDevicePlugin::ExecuteCommand(
		SdCommandParams* params)
{
	int err = SD_ESUCCESS;
	switch (params->CommandCode()) {
	case SD_COMMAND_RUN:
		err = Start(&params->Params().asDroneConfig());
		break;
	case SD_COMMAND_RESET:
		Stop(false);
		break;
	case SD_COMMAND_EXIT:
		Stop(true);
		break;
	case SD_COMMAND_GET_MOTORS: {
		Vector4d outData;
		for (size_t i = 0, j = 0; i < servoctrl_->channelcount() && j < 4; ++i) {
			if (servoctrl_->motor(i).valid()) {
				outData.at(j++,0) = servoctrl_->motor(i).offset();
			}
		}
		params->SetOutParams(SdIoData(outData));
		break;
	}
	case SD_COMMAND_SET_MOTORS: {
		if (params->Params().dataType()== SdIoData::TYPE_VECTOR4D) {
			try {
				int j = 0;
				const Vector4d& inData = params->Params().asVector4d();
				for (size_t i = 0; i < servoctrl_->channelcount() && j < 4; ++i) {
					if (servoctrl_->motor(i).valid()) {
						servoctrl_->motor(i).offset(inData.at(j++,0));
					}
				}
				servoctrl_->update();
			} catch (std::exception& e) {
				printf("ServoDevicePlugin::SetMotors error: %s\n", e.what());
				err = EINVAL;
			}
		}
		break;
	}
	default:break;
	}
	return err;
}

int ServoDevicePlugin::Start(const SdDroneConfig* droneConfig)
{
	const SdServoConfig* config = &droneConfig->Servo;
	int err = 0;

	try {
		if (std::string::npos == config->DeviceName.find("/dev/")){
			fprintf(stdout,"ServoDevice is operating in text mode.\n");
			servoctrl_.reset(new servocontroller(16));
		} else  {
			servoctrl_.reset(new pca9685controller(16));
		}
		servoctrl_->setrate(config->Rate);
		for (size_t i = 0; i < servoctrl_->channelcount(); i++) {
			servoctrl_->motor(i) = servomotor(1.0, 1.25, 1.1, 2.2);
		}
		servoctrl_->enable();
		servoctrl_->armmotors();
		printf("ServoDevice - all channels are armed and ready!\n");
	} catch (std::exception& e) {
		fprintf(stdout, "ServoDevicePlugin::Start error: \n\t%s\n", e.what());
		err = EINVAL;
		goto __return;
	}

	/*
	 * Tell the runtime we are interested in writes towards our
	 * own device
	 */
	runtime_->SetIoFilters(SD_DEVICEID_TO_FLAG(SD_DEVICEID_SERVO),
			SD_IOCODE_TO_FLAG(SD_IOCODE_SEND));

	__return:
	return err;
}

void ServoDevicePlugin::Stop(bool detach)
{
	if (servoctrl_) {
		try {

			// disarm the motors
			servoctrl_->disarmmotors();
			servoctrl_->disable();
		}  catch (std::exception& e) {
			printf("ServoDevicePlugin::Stop error: %s\n", e.what());
		}
	}

	if (detach) {
		runtime_->DetachPlugin();
	}
}

int ServoDevicePlugin::AttachToChain(
		void* droneContext,
		SdPluginAttachFunc attachFunc)
{
	int err = attachFunc(
			droneContext,
			this,
			SD_ALTITUDE_GROUP_DEVICE,
			0,
			0,
			&runtime_);
	return err;
}

int ServoDevicePlugin::AddRef()
{
	return __sync_fetch_and_add(&ref_cnt_,1);
}

int ServoDevicePlugin::Release()
{
	int refCnt = __sync_sub_and_fetch(&ref_cnt_,1);
	if (0 == refCnt) {
		delete this;
	}
	return refCnt;
}

const char* ServoDevicePlugin::GetName()
{
	return SD_PLUGIN_SERVO_PCA9685;
}

SdDeviceId ServoDevicePlugin::GetDeviceId()
{
	return SD_DEVICEID_SERVO;
}

const char* ServoDevicePlugin::GetVersion()
{
	return SD_PLUGIN_VERSION;
}

extern char **g_Argv;
const char* ServoDevicePlugin::GetDlFileName()
{
	return g_Argv[0];
}

int ServoDevicePlugin::IoCallback(
	SdIoPacket* ioPacket)
{
	const SdIoData& ioData = ioPacket->GetIoData(true);
	if (ioData.dataType()!= SdIoData::TYPE_SERVO) {
		return EINVAL;
	}
	const SdServoIoData& servoData = ioData.asServoData();
	assert(servoData.numChannels <= sizeof(servoData.channels));
	try {
		for (uint32_t i = 0; i < servoData.numChannels; i++) {
			if (servoData.channels[i] < 16) {
				servoctrl_->motor(servoData.channels[i]).offset(servoData.value[i]);
			}
		}
		servoctrl_->update();
	} catch (std::exception& e) {
		printf("ServoDevicePlugin::IoCallback error: %s\n", e.what());
		printf("ServoDevicePlugin - disabling power output\n");
		servoctrl_->disable();
		return SD_ESTOP_DISPATCH;
	}

	return SD_ESUCCESS;
}

int ServoDevicePlugin::IoDispatchThread()
{
	assert(false);
	return EINVAL;
}
