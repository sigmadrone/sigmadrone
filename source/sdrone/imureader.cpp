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
	m_RunTime = 0;
	memset(&m_ImuData,0,sizeof(m_ImuData));
	memset(&m_GyroConfig,0,sizeof(m_GyroConfig));
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
}

int ImuReader::Start(
	CommandArgs* cmdArgs)
{
	int err = EINVAL;
	Close();
	const SdDroneConfig* droneConfig = cmdArgs->GetSdDroneConfig();
	if (droneConfig->Accel.DeviceName == droneConfig->Gyro.DeviceName) {
		/*
		 * Operating in text mode
		 */
		m_File = fopen(droneConfig->Accel.DeviceName,"r");
		if (0 == m_File) {
			fprintf(stdout,"ImuReader::Init failed to open %s, err %d\n",
				droneConfig->Accel.DeviceName,errno);
			goto __return;
		}
		err = 0;
		fprintf(stdout,"ImuReader opened text file %s\n",
			droneConfig->Accel.DeviceName);
	} else {
		int readValue = 0;
		if (0 != (err = m_GyroDevice.Open(
			droneConfig->Gyro.DeviceName,SD_IMU_DEVICE_GYRO))) {
			goto __return;
		}
		if (0 != (err = m_AccDevice.Open(
			droneConfig->Accel.DeviceName,SD_IMU_DEVICE_ACCEL))) {
			goto __return;
		}
		if (0 != (err = m_MagDevice.Open(
			droneConfig->Mag.DeviceName,SD_IMU_DEVICE_MAG))) {
			goto __return;
		}
		m_AccDevice.SetRate(droneConfig->Accel.Rate);
		m_GyroDevice.SetRate(droneConfig->Gyro.Rate);
		m_AccDevice.SetScale(droneConfig->Accel.Scale);
		m_AccDevice.GetScale(&readValue);
		if (readValue != droneConfig->Accel.Scale) {
			printf("ERROR: read wrong value for accel scale %d, expected %d\n",
					readValue, droneConfig->Accel.Scale);
		}
		m_GyroDevice.SetScale(droneConfig->Gyro.Scale);
		m_GyroDevice.GetScale(&readValue);
		if (readValue != droneConfig->Gyro.Scale) {
			printf("ERROR: read wrong value for gyro scale %d, expected %d\n",
					readValue, droneConfig->Gyro.Scale);
		}
		m_GyroDevice.Enable(0);
		m_AccDevice.Enable(0);
		m_GyroDevice.ResetFifo();
		m_AccDevice.ResetFifo();
		m_MagDevice.SetReadDataThrottle(15);
		m_GyroDevice.Enable(1);
		m_AccDevice.Enable(1);
		m_MagDevice.Enable(1);
	}

	m_GyroConfig = droneConfig->Gyro;

	clock_gettime(CLOCK_REALTIME, &m_LastTime);

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

void ImuReader::Stop(int flags)
{
	m_RunTime->StartStopIoDispatchThread(false);
	if (!!(flags&FLAG_STOP_AND_DETACH)) {
		m_RunTime->DetachPlugin();
	}
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
	assert(false);
	return EINVAL;
}

int ImuReader::IoDispatchThread()
{
	int ret = 0;
	SdIoPacket ioPacket;
	SdIoPacket::Init(&ioPacket,SD_IOCODE_RECEIVE, GetDeviceId(), GetName());
	ioPacket.inData.dataType = SdIoData::TYPE_IMU;
	ioPacket.inData.asImuData = &m_ImuData;
	ioPacket.startPluginAltitude = m_RunTime->GetMyAltitude();

	if (0 != m_File) {
		char buffer[256];
		if (fgets(buffer, sizeof(buffer), m_File)) {
			sscanf(buffer, "%hd %hd %hd %hd %hd %hd %hd %hd %hd",
					&m_ImuData.acc[0].x, &m_ImuData.acc[0].y, &m_ImuData.acc[0].z,
					&m_ImuData.gyro[0].x, &m_ImuData.gyro[0].y, &m_ImuData.gyro[0].z,
					&m_ImuData.mag[0].x, &m_ImuData.mag[0].y, &m_ImuData.mag[0].z);
			m_ImuData.gyro_samples = 1;
			m_ImuData.acc_samples = 1;
			m_ImuData.mag_samples = 1;
			ioPacket.deltaTime = 0.005; //TODO
		} else {
			ret = EIO;
		}
	} else {
		ret = m_GyroDevice.ReadData(m_ImuData.gyro, sizeof(m_ImuData.gyro));
		if (ret < 0) {
			goto __return;
		}
		m_ImuData.gyro_samples = ret;
		ret = m_AccDevice.ReadData(m_ImuData.acc, sizeof(m_ImuData.acc));
		if (ret < 0) {
			goto __return;
		}
		m_ImuData.acc_samples = ret;
		ret = m_MagDevice.ReadData(m_ImuData.mag, sizeof(m_ImuData.mag));
		if (ret < 0) {
			goto __return;
		}
		m_ImuData.mag_samples = ret;
		ioPacket.deltaTime = DeltaT();
	}

	m_GyroData.clear();
	for (unsigned int j = 0; j < m_ImuData.gyro_samples; j++) {
		m_GyroData = m_GyroData + Vector3d(
				m_ImuData.gyro[j].x, m_ImuData.gyro[j].y, m_ImuData.gyro[j].z);
	}
	m_GyroData = m_GyroData / m_ImuData.gyro_samples * m_GyroConfig.Scale /
			m_GyroConfig.MaxReading;

	m_AccelData.clear();
	for (unsigned int j = 0; j < m_ImuData.acc_samples; j++) {
		m_AccelData = m_AccelData + Vector3d(
				m_ImuData.acc[j].x, m_ImuData.acc[j].y, m_ImuData.acc[j].z);
	}
	m_AccelData = m_AccelData / m_ImuData.acc_samples;
	m_AccelData = m_AccelData.normalize();

	m_MagData.clear();
	for (unsigned int j = 0; j < m_ImuData.mag_samples; j++) {
		m_MagData = m_MagData + Vector3d(
				m_ImuData.mag[j].x, m_ImuData.mag[j].y, m_ImuData.mag[j].z);
	}
	m_MagData = m_MagData / m_ImuData.mag_samples;

	ioPacket.gyroDataDps = &m_GyroData;
	ioPacket.accelData = &m_AccelData;
	ioPacket.magData = &m_MagData;
	ret = m_RunTime->DispatchIo(&ioPacket,0);

	__return:

	return ret;
}
