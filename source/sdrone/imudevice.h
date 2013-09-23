/*
 * imudevice.h
 *
 * Represents Inertial Measurement Unit device
 *
 *  Created on: May 18, 2013
 *      Author: svassilev
 */

#ifndef IMUDEVICE_H_
#define IMUDEVICE_H_

#include "commoninc.h"

#define IMUDEVICE_DEFAULT_RATE 200
#define IMUDEVICE_DEFAULT_GYRO_SCALE 575
#define IMUDEVICE_DEFAULT_ACCEL_SCALE 4

class ImuDevice
{
public:
	ImuDevice(
		);
	~ImuDevice(
		);
	int Open(
		const char* deviceName,
		SdImuDeviceId deviceId
		);
	void Close(
		);
	int ResetFifo(
		);
	int SetRate(
		int rate
		);
	int GetRate(
		int* rate
		);
	int SetScale(
		int scale
		);
	int GetScale(
		int* scale
		);
	int Enable(
		int enable
		);
	bool IsEnabled(
		);
	int ReadData(
		short3d_t* data,
		size_t size
		);
	int SetWatermark(
		int watermark
		);

	inline SdImuDeviceId GetDeviceId() { return m_DeviceId; }

	/*
	 * Since we are polling the device queue, the throttle controls
	 * how often to read the data from the device. When ReadData is called
	 * the actual data is read every m_Throttle-th invocation
	 */
	inline void SetReadDataThrottle(int throttle) {
		m_ReadDataThrottle = (throttle>=1) ? throttle : m_ReadDataThrottle;
	}

	static const int s_InvalidFileId = -1;

private:
	int m_Fid;
	int m_Counter;
	int m_ReadDataThrottle;
	short3d_t m_LastData;
	SdImuDeviceId m_DeviceId;
	char* m_DeviceName;
};

#endif /* IMUDEVICE_H_ */
