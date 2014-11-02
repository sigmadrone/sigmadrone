/*
 * thrustcorrectionplugin.cpp
 *
 *  Created on: Oct 23, 2014
 *      Author: svassilev
 */

#include "thrustcorrectionplugin.h"
#include <iostream>

ThrustCorrectionPlugin::ThrustCorrectionPlugin() : m_runtime(0), m_refCnt(1)  {}

int ThrustCorrectionPlugin::AttachToChain(
		void* droneContext,
		SdPluginAttachFunc attachFunc)
{
	int err = attachFunc(
			droneContext,
			this,
			SD_ALTITUDE_GROUP_UPPER_FILTER, // has to be below the pilot
			0,
			0,
			&m_runtime);
	return err;
}

int ThrustCorrectionPlugin::ExecuteCommand(
		SdCommandParams* params)
{
	int err = SD_ESUCCESS;
	switch (params->CommandCode()) {
	case SD_COMMAND_RUN:
		m_runtime->SetIoFilters(
				SD_DEVICEID_TO_FLAG(SD_DEVICEID_SERVO),
				SD_IOCODE_TO_FLAG(SD_IOCODE_SEND));
		break;
	case SD_COMMAND_EXIT:
		m_runtime->DetachPlugin();
		break;
	case SD_COMMAND_SET_CORRECTION_THRUST:
		assert(params->Params().dataType() == SdIoData::TYPE_VECTOR4D);
		m_thrustCorrection = params->Params().asVector4d();
		std::cout << "Correction thrust:\n" << m_thrustCorrection;
		break;
	case SD_COMMAND_GET_CORRECTION_THRUST:
		params->SetOutParams(SdIoData(m_thrustCorrection));
		break;
	default:break;
	}
	return err;
}

int ThrustCorrectionPlugin::AddRef()
{
	return __sync_fetch_and_add(&m_refCnt,1);
}

int ThrustCorrectionPlugin::Release()
{
	int refCnt = __sync_sub_and_fetch(&m_refCnt,1);
	if (0 == refCnt) {
		delete this;
	}
	return refCnt;
}

const char* ThrustCorrectionPlugin::GetName() {
	return SD_PLUGIN_THRUST_CORRECT;
}

SdDeviceId ThrustCorrectionPlugin::GetDeviceId() {
	return SD_DEVICEID_FILTER;
}

const char* ThrustCorrectionPlugin::GetVersion() {
	return SD_PLUGIN_VERSION;
}

extern char **g_Argv;
const char* ThrustCorrectionPlugin::GetDlFileName() {
	return g_Argv[0];
}


int ThrustCorrectionPlugin::IoCallback(SdIoPacket* ioPacket)
{
	const SdIoData& ioData = ioPacket->GetIoData(true);
	if (ioData.dataType()!= SdIoData::TYPE_SERVO) {
		return EINVAL;
	}
	SdServoIoData servoData = ioData.asServoData();
	assert(servoData.numChannels <= ARRAYSIZE(servoData.channels));
	for (uint32_t i = 0; i < 4; i++) {
		assert ((size_t)servoData.channels[i] < ARRAYSIZE(servoData.value));
		servoData.value[servoData.channels[i]] += m_thrustCorrection.at(i,0);
	}
	ioPacket->SetIoData(servoData,true);
	return SD_ESUCCESS;
}

int ThrustCorrectionPlugin::IoDispatchThread() {
	return SD_ESTOP_DISPATCH;
}

ThrustCorrectionPlugin::~ThrustCorrectionPlugin() {}

