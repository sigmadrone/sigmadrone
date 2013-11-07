/*
 * imubias.cpp
 *
 *  Created on: August 16, 2013
 *      Author: svassilev
 */

#include "commoninc.h"
#include "imubias.h"
#include <dlfcn.h>

ImuBias::ImuBias()
{
	m_Runtime = 0;
	m_RefCnt = 1;
	m_CurrentBiasSamples = 0;
	m_ElapsedTime = m_TotalBiasCalcTime = 0;
}

ImuBias::~ImuBias()
{
	assert(0 == m_RefCnt);
}

int ImuBias::AttachToChain(
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

int ImuBias::Start(
	CommandArgs* cmdArgs)
{
	int biasSamples = cmdArgs->GetDroneConfig()->Gyro.NumBiasSamples;
	m_CurrentBiasSamples = 0;
	m_ElapsedTime = 0;
	m_TotalBiasCalcTime = (double)biasSamples/cmdArgs->GetDroneConfig()->Gyro.SamplingRate;
	m_Runtime->Log(SD_LOG_LEVEL_INFO,
			"--> Will calculate bias for %d samples (%2.3lf sec), stay still...\n",
			 biasSamples, m_TotalBiasCalcTime);
	m_Runtime->SetIoFilters(
		SD_DEVICEID_TO_FLAG(SD_DEVICEID_IMU),
		SD_IOCODE_TO_FLAG(SD_IOCODE_RECEIVE));
	return SD_ESUCCESS;
}

int ImuBias::AddRef()
{
	return __sync_fetch_and_add(&m_RefCnt,1);
}

int ImuBias::Release()
{
	int refCnt = __sync_sub_and_fetch(&m_RefCnt,1);
	if (0 == refCnt) {
		delete this;
	}
	return refCnt;
}

void ImuBias::Stop(int flags)
{
	if (!!(flags&FLAG_STOP_AND_DETACH)) {
		m_Runtime->DetachPlugin();
	}
}

const char* ImuBias::GetName()
{
	return SD_PLUGIN_IMU_BIAS;
}

SdDeviceId ImuBias::GetDeviceId()
{
	return SD_DEVICEID_FILTER;
}

const char* ImuBias::GetVersion()
{
	return SD_PLUGIN_VERSION;
}

extern char **g_Argv;
const char* ImuBias::GetDlFileName()
{
	return g_Argv[0];
}

int ImuBias::IoCallback(
	SdIoPacket* ioPacket)
{
	if (m_ElapsedTime < m_TotalBiasCalcTime) {

		m_EarthG = m_EarthG + ioPacket->AccelData();
		m_GyroBias = m_GyroBias + ioPacket->GyroData();
		m_CurrentBiasSamples++;
		m_ElapsedTime += ioPacket->DeltaTime();

		if (m_ElapsedTime < m_TotalBiasCalcTime) {
			return SD_ESTOP_DISPATCH;
		}

		m_EarthG = m_EarthG / m_CurrentBiasSamples;
		m_GyroBias = m_GyroBias / m_CurrentBiasSamples;

		m_Runtime->Log(SD_LOG_LEVEL_INFO,"\n--> Done calculating bias!\n");
		m_Runtime->Log(SD_LOG_LEVEL_INFO,"--> Earth G:   %1.3lf %1.3lf %1.3lf\n",
				m_EarthG.at(0,0),
				m_EarthG.at(1,0),
				m_EarthG.at(2,0));
		m_Runtime->Log(SD_LOG_LEVEL_INFO,"--> Gyro Bias: %4.3lf %4.3lf %4.3lf\n\n",
				m_GyroBias.at(0,0), m_GyroBias.at(1,0), m_GyroBias.at(2,0));

		/*
		 * TODO: in order for us to be able to level the drone, earth G must
		 * contain the averaged reading when the drone frame is leveled,i.e
		 * parallel to the earth surface. So I guess accounting for the actual
		 * earth G must be done only if we know that the drone is leveled and
		 * perhaps should be done only once as it will depend on the angle
		 * between the sensor and the drone coordinate systems. For now we will
		 * just hard-code it
		 */
		if (fabs(m_EarthG.at(2,0)) < 0.97) {
			m_EarthG = Vector3d(0,0,-1);
		}
	}
	Vector3d gyroNoBias = ioPacket->GyroData()-m_GyroBias;
	ioPacket->SetAttribute(SDIO_ATTR_GYRO,SdIoData(&gyroNoBias));
	ioPacket->SetAttribute(SDIO_ATTR_EARTH_G,SdIoData(&m_EarthG));

	return SD_ESUCCESS;
}

int ImuBias::IoDispatchThread()
{
	assert(false);
	return EINVAL;
}
