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
	m_sensorLog = 0;
	m_RunTime = 0;
	clock_gettime(CLOCK_REALTIME, &m_lastTime);
	m_refCnt = 1;
	m_counter = 0;
}

ImuReader::~ImuReader()
{
	assert(0 == m_refCnt);
	CloseSensorLog();
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

void ImuReader::CloseSensorLog()
{
	if (0 != m_sensorLog) {
		fflush(m_sensorLog);
		fclose(m_sensorLog);
		m_sensorLog = 0;
	}
}

int ImuReader::ExecuteCommand(SdCommandParams* params)
{
	int err = SD_ESUCCESS;
	switch(params->CommandCode()) {
	case SD_COMMAND_RUN:
		err = Run(&params->Params().asDroneConfig());
		break;
	case SD_COMMAND_RESET:
		m_RunTime->StartStopIoDispatchThread(false);
		break;
	case SD_COMMAND_EXIT:
		m_RunTime->StartStopIoDispatchThread(false);
		m_RunTime->DetachPlugin();
		break;
	case SD_COMMAND_GET_ACCELEROMETER:
		if (m_fileSampler) {
			params->SetOutParams(SdIoData(m_fileSampler->data.acc3d_));
		} else if (m_sampler) {
			params->SetOutParams(SdIoData(m_sampler->data.acc3d_));
		} else {
			params->SetOutParams(SdIoData(Vector3d(0,0,1)));
		}
		break;
	default:break;
	}
	return err;
}

int ImuReader::Run(
	const SdDroneConfig* droneConfig)
{
	int err = 0;
	CloseSensorLog();
	if (droneConfig->Accel.DeviceName == droneConfig->Gyro.DeviceName) {
		/*
		 * Operating in text mode
		 */
		m_fileSampler.reset(new file_sampler(droneConfig->Accel.DeviceName.c_str(), true));
		try {
			m_fileSampler.reset(new file_sampler(
					droneConfig->Accel.DeviceName.c_str(),
					true));
		} catch (std::exception& e) {
			fprintf(stdout, "Error: %s\n", e.what());
			err = EINVAL;
			goto __return;
		}
		err = 0;
		fprintf(stdout,"ImuReader opened text file %s\n",
			droneConfig->Accel.DeviceName.c_str());
	} else {
		try {
			m_sampler.reset(new sampler(
					droneConfig->Gyro.DeviceName,
					droneConfig->Accel.DeviceName,
					droneConfig->Mag.DeviceName,
					"/sys/bus/i2c/devices/4-0077/pressure0_input"
			));
			m_sampler->gyr_.set_adjustment(1.2);
			m_sampler->gyr_.set_rate(droneConfig->Gyro.SamplingRate);
			m_sampler->acc_.set_rate(droneConfig->Accel.SamplingRate);
			m_sampler->gyr_.set_full_scale(droneConfig->Gyro.Scale);
			m_sampler->acc_.set_full_scale(droneConfig->Accel.Scale);
			m_sampler->mag_.set_full_scale(droneConfig->Mag.Scale ? droneConfig->Mag.Scale : 1300);
			m_sampler->gyr_.set_fifo_threshold(droneConfig->Gyro.Watermark);
			m_sampler->acc_.set_fifo_threshold(droneConfig->Accel.Watermark);
			m_sampler->init();
			remove("./sensordata.dat_bak");
			rename("./sensordata.dat","./sensordata.dat_bak");
			m_sensorLog = fopen("./sensordata.dat", "w");
		} catch (std::exception& e) {
			fprintf(stdout, "Error: %s\n", e.what());
			goto __return;
		}
	}
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
	dT = (now.tv_sec - m_lastTime.tv_sec) +
			(now.tv_nsec - m_lastTime.tv_nsec)/1000000000.0;
	m_lastTime = now;
	return dT; // sec
}

int ImuReader::AddRef()
{
	return __sync_fetch_and_add(&m_refCnt,1);
}

int ImuReader::Release()
{
	int refCnt = __sync_sub_and_fetch(&m_refCnt,1);
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
	SdImuData imuData;
	double pressure;

	SdIoPacket* ioPacket = m_RunTime->AllocIoPacket(
			SD_IOCODE_RECEIVE, GetDeviceId(), GetName());
	if (0 == ioPacket) {
		return ENOMEM;
	}

	++m_counter;

	if (m_fileSampler) {
		if (!m_fileSampler->update()) {
			ret = EIO;
			goto __return;
		}
		imuData.mag3d_upd = true;
		imuData.gyro3d_upd = true;
		imuData.acc3d_upd = true;
		imuData.acc3d = m_fileSampler->data.acc3d_;
		imuData.gyro3d = m_fileSampler->data.gyr3d_;
		imuData.mag3d = m_fileSampler->data.mag3d_;
		pressure = m_fileSampler->data.bar1d_;
		ioPacket->SetAttribute(SDIO_ATTR_TIME_TO_READ_SENSORS,
				SdIoData(m_fileSampler->data.dtime_));
		ioPacket->SetAttribute(SDIO_ATTR_DELTA_TIME,
				SdIoData(m_fileSampler->data.dtime_));
	} else {
		m_sampler->update();
		imuData.mag3d_upd = m_sampler->data.mag3d_upd_;
		imuData.gyro3d_upd = m_sampler->data.gyr3d_upd_;
		imuData.acc3d_upd = m_sampler->data.acc3d_upd_;
		imuData.acc3d = m_sampler->data.acc3d_;
		imuData.gyro3d = m_sampler->data.gyr3d_;
		imuData.mag3d = m_sampler->data.mag3d_;
		pressure = m_sampler->data.bar1d_;
		ioPacket->SetAttribute(SDIO_ATTR_TIME_TO_READ_SENSORS,
				SdIoData(m_sampler->data.dtime_));
		ioPacket->SetAttribute(SDIO_ATTR_DELTA_TIME,
				SdIoData(DeltaT()));
	}

	ioPacket->SetIoData(SdIoData(imuData),true);

	if (m_sensorLog) {
		fprintf(m_sensorLog, "%10.2lf %10.2lf %10.2lf    %10.2lf %10.2lf %10.2lf    "
				"%10.2lf %10.2lf %10.2lf    %10.2lf    %10.6lf\n",
				imuData.acc3d.at(0, 0), imuData.acc3d.at(1, 0), imuData.acc3d.at(2, 0),
				imuData.gyro3d.at(0, 0), imuData.gyro3d.at(1, 0), imuData.gyro3d.at(2, 0),
				imuData.mag3d.at(0, 0), imuData.mag3d.at(1, 0), imuData.mag3d.at(2, 0),
				pressure,
				ioPacket->GetAttribute(SDIO_ATTR_TIME_TO_READ_SENSORS).asDouble());
		fflush(m_sensorLog);
	}

	imuData.acc3d = imuData.acc3d.normalize();
	imuData.mag3d = imuData.mag3d.normalize();
	ioPacket->SetAttribute(SDIO_ATTR_ACCEL,SdIoData(imuData.acc3d));
	ioPacket->SetAttribute(SDIO_ATTR_GYRO,SdIoData(imuData.gyro3d));
	ioPacket->SetAttribute(SDIO_ATTR_MAG,SdIoData(imuData.mag3d));
	ret = m_RunTime->DispatchIo(ioPacket,0);

	__return:
	m_RunTime->FreeIoPacket(ioPacket);

	return ret;
}
