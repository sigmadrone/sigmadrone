/*
 * imureader.cpp
 *
 *  Created on: August 16, 2013
 *      Author: svassilev
 */

#include "commoninc.h"
#include "imureader.h"
#include <dlfcn.h>

ImuReader::ImuReader()
{
	m_File = 0;
	m_SensorLog = 0;
	m_RunTime = 0;
	clock_gettime(CLOCK_REALTIME, &m_LastTime);
	m_RefCnt = 1;
}

ImuReader::~ImuReader()
{
	assert(0 == m_RefCnt);
	Close();
}

int ImuReader::AttachToChain(
		void* droneContext,
		SdPluginAttachFunc attachFunc)
{
	int err = attachFunc(
			droneContext,
			this,
			SD_ALTITUDE_GROUP_DEVICE,
			0,
			0,
			&m_RunTime);
	return err;
}

void ImuReader::Close()
{
	if (0 != m_File) {
		fclose(m_File);
		m_File = 0;
	}
	if (0 != m_SensorLog) {
		fflush(m_SensorLog);
		fclose(m_SensorLog);
		m_SensorLog = 0;
	}
}

int ImuReader::ExecuteCommand(SdCommandParams* params)
{
	int err = SD_ESUCCESS;
	switch(params->CommandCode()) {
	case SD_COMMAND_RUN:
		err = Run(params->Params().asDroneConfig);
		break;
	case SD_COMMAND_RESET:
		m_RunTime->StartStopIoDispatchThread(false);
		break;
	case SD_COMMAND_EXIT:
		m_RunTime->StartStopIoDispatchThread(false);
		m_RunTime->DetachPlugin();
		break;
	default:break;
	}
	return err;
}

int ImuReader::Run(
	const SdDroneConfig* droneConfig)
{
	int err = 0;
	Close();
	remove("./sensordata.dat_bak");
	rename("./sensordata.dat","./sensordata.dat_bak");
	m_SensorLog = fopen("./sensordata.dat", "w");
	if (droneConfig->Accel.DeviceName == droneConfig->Gyro.DeviceName) {
		/*
		 * Operating in text mode
		 */
		m_File = fopen(droneConfig->Accel.DeviceName.c_str(),"r");
		if (0 == m_File) {
			fprintf(stdout,"ImuReader::Init failed to open %s, err %d\n",
				droneConfig->Accel.DeviceName.c_str(),errno);
			goto __return;
		}
		err = 0;
		fprintf(stdout,"ImuReader opened text file %s\n",
			droneConfig->Accel.DeviceName.c_str());
	} else {
		try {
			m_Sampler.reset(new sampler(
					droneConfig->Gyro.DeviceName,
					droneConfig->Accel.DeviceName,
					droneConfig->Mag.DeviceName,
					"/sys/bus/i2c/devices/4-0077/pressure0_input"
			));
			m_Sampler->gyr_.set_rate(droneConfig->Gyro.SamplingRate);
			m_Sampler->acc_.set_rate(droneConfig->Accel.SamplingRate);
			m_Sampler->gyr_.set_full_scale(droneConfig->Gyro.Scale);
			m_Sampler->acc_.set_full_scale(droneConfig->Accel.Scale);
			m_Sampler->mag_.set_full_scale(droneConfig->Mag.Scale ? droneConfig->Mag.Scale : 1300);
			m_Sampler->gyr_.set_fifo_threshold(droneConfig->Gyro.Watermark);
			m_Sampler->acc_.set_fifo_threshold(droneConfig->Accel.Watermark);
			m_Sampler->init();
		} catch (std::exception& e) {
			fprintf(stdout, "Error: %s\n", e.what());
			goto __return;
		}
	}
	m_GyroConfig = droneConfig->Gyro;
	DeltaT();

	/*
	 * Since this is a device plugin, that needs to read data,
	 * enable the polling thread
	 */
	m_RunTime->StartStopIoDispatchThread(true);

__return:
	return err;
}

double ImuReader::DeltaT()
{
	timespec now;
	double dT;
	clock_gettime(CLOCK_REALTIME, &now);
	dT = (now.tv_sec - m_LastTime.tv_sec) +
			(now.tv_nsec - m_LastTime.tv_nsec)/1000000000.0;
	m_LastTime = now;
	return dT; // sec
}

int ImuReader::AddRef()
{
	return __sync_fetch_and_add(&m_RefCnt,1);
}

int ImuReader::Release()
{
	int refCnt = __sync_sub_and_fetch(&m_RefCnt,1);
	if (0 == refCnt) {
		delete this;
	}
	return refCnt;
}

const char* ImuReader::GetName()
{
	return SD_PLUGIN_IMU_DEVICE;
}

SdDeviceId ImuReader::GetDeviceId()
{
	return SD_DEVICEID_IMU;
}

const char* ImuReader::GetVersion()
{
	return SD_PLUGIN_VERSION;
}

extern char **g_Argv;
const char* ImuReader::GetDlFileName()
{
	return g_Argv[0];
}

int ImuReader::IoCallback(
	SdIoPacket* ioPacket)
{
	return SD_ESUCCESS;
}

int ImuReader::IoDispatchThread()
{
	int ret = 0;
	Vector3d accelData;
	Vector3d magData;

	SdIoPacket* ioPacket = m_RunTime->AllocIoPacket(
			SD_IOCODE_RECEIVE, GetDeviceId(), GetName());
	if (0 == ioPacket) {
		return ENOMEM;
	}

	++m_Counter;

	if (0 != m_File) {

	} else {
		m_Sampler->update();
		ioPacket->SetAttribute(SDIO_ATTR_TIME_TO_READ_SENSORS,
				SdIoData(m_Sampler->data.dtime_));
		ioPacket->SetAttribute(SDIO_ATTR_DELTA_TIME,
				SdIoData(DeltaT()));
	}
	accelData = m_Sampler->data.acc3d_.normalize();
	magData = m_Sampler->data.mag3d_.normalize();
	ioPacket->SetAttribute(SDIO_ATTR_ACCEL,SdIoData(&accelData));
	ioPacket->SetAttribute(SDIO_ATTR_GYRO,SdIoData(&m_Sampler->data.gyr3d_));
	ioPacket->SetAttribute(SDIO_ATTR_MAG,SdIoData(&magData));
	ret = m_RunTime->DispatchIo(ioPacket,0);
	m_RunTime->FreeIoPacket(ioPacket);

	return ret;
}
