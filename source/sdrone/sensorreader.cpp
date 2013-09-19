/*
 * sensorreader.cpp
 *
 *  Created on: May 25, 2013
 *      Author: svassilev
 */

#include "commoninc.h"
#include "sensorreader.h"

SensorReader::SensorReader()
{
	m_File = 0;
}

SensorReader::~SensorReader() {

}

void SensorReader::Close()
{
	if (0 != m_File) {
		fclose(m_File);
		m_File = 0;
	}
}

int SensorReader::Reset(
	const SdDroneConfig* droneConfig)
{
	int err = EINVAL;
	Close();
	if (droneConfig->Accel.DeviceName == droneConfig->Gyro.DeviceName) {
		/*
		 * Operating in text mode
		 */
		m_File = fopen(droneConfig->Accel.DeviceName,"r");
		if (0 == m_File) {
			fprintf(stdout,"SensorReader::Init failed to open %s, err %d\n",
				droneConfig->Accel.DeviceName,errno);
			goto __return;
		}
		err = 0;
		fprintf(stdout,"SensorReader opened text file %s\n",
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

	__return:

	return err;
}

int SensorReader::ReadImuData(SdImuData* imuData)
{
	int ret = 0;
	if (0 != m_File) {
		char buffer[256];
		if (fgets(buffer, sizeof(buffer), m_File)) {
			sscanf(buffer, "%hd %hd %hd %hd %hd %hd %hd %hd %hd",
				&imuData->acc[0].x, &imuData->acc[0].y, &imuData->acc[0].z,
				&imuData->gyro[0].x, &imuData->gyro[0].y, &imuData->gyro[0].z,
				&imuData->mag[0].x, &imuData->mag[0].y, &imuData->mag[0].z);
			imuData->gyro_samples = 1;
			imuData->acc_samples = 1;
			imuData->mag_samples = 1;
		} else {
			ret = EIO;
		}
	} else {
		ret = m_GyroDevice.ReadData(imuData->gyro, sizeof(imuData->gyro));
		if (ret < 0) {
			goto __return;
		}
		imuData->gyro_samples = ret;
		ret = m_AccDevice.ReadData(imuData->acc, sizeof(imuData->acc));
		if (ret < 0) {
			goto __return;
		}
		imuData->acc_samples = ret;
		ret = m_MagDevice.ReadData(imuData->mag, sizeof(imuData->mag));
		if (ret < 0) {
			goto __return;
		}
		imuData->mag_samples = ret;
	}

//	tmp = imuData->acc.x;
//	imuData->acc.x = -imuData->acc.y;
//	imuData->acc.y = tmp;
//	imuData->acc.z = -imuData->acc.z;

//	tmp = imuData->mag.x;
//	imuData->mag.x = -imuData->mag.y;
//	imuData->mag.y = tmp;
//	imuData->mag.z = -imuData->mag.z;

	__return:
	return 0;
}

ImuDevice* SensorReader::GetDevice(SdImuDeviceId imuDeviceId)
{
	switch(imuDeviceId) {
	case SD_IMU_DEVICE_ACCEL:return &m_AccDevice;
	case SD_IMU_DEVICE_GYRO:return &m_GyroDevice;
	case SD_IMU_DEVICE_MAG:return &m_MagDevice;
	case SD_IMU_DEVICE_UNKNOWN: return 0;
	}
	return 0;
}

int SensorReader::CalculateImuBias(
		Vector3d* gyroBias,
		Vector3d* accBias,
		int numSamples)
{
	int err = 0;
	SdImuData imuData;
	int nGyro = 0;

	printf("Calculating gyro and acccel bias for %d samples\n",
			numSamples);

	gyroBias->clear();
	accBias->clear();
	for (nGyro = 0; nGyro < numSamples; ) {
		if (0 != (err = ReadImuData(&imuData))) {
			goto __return;
		}
		for (unsigned int j = 0; j < imuData.gyro_samples; j++)
			*gyroBias = *gyroBias + Vector3d(imuData.gyro[j].x, imuData.gyro[j].y, imuData.gyro[j].z);
		nGyro += imuData.gyro_samples;
		for (unsigned int j = 0; j < imuData.acc_samples; j++)
			*accBias = *accBias + Vector3d(imuData.acc[j].x, imuData.acc[j].y, imuData.acc[j].z);
	}
	*gyroBias = *gyroBias / nGyro;
	*accBias = accBias->normalize();

	printf("IMU gravity vector: %4.3lf %4.3lf %4.3lf\n",
			accBias->at(0,0),accBias->at(1,0),accBias->at(2,0));
	printf("IMU gyro (samples: %d)  bias: %4.3lf %4.3lf %4.3lf\n",
			nGyro, gyroBias->at(0,0),gyroBias->at(1,0),gyroBias->at(2,0));

	__return:
	return err;
}
