/*
 * sensorreader.h
 *
 *  Created on: May 18, 2013
 *      Author: svassilev
 */

#ifndef SENSORREADER_H_
#define SENSORREADER_H_

#include "imudevice.h"

/*
 * Reads data from all the sensor devices
 * Should be able to read from a file
 */
class SensorReader
{
public:
	SensorReader();
	~SensorReader();
	int Reset(
			const SdDroneConfig*
			);
	int ReadImuData(
			SdImuData* imuData
			);
	ImuDevice* GetDevice(
			SdImuDeviceId
			);
	int CalculateImuBias(
			Vector3d* gyroBias,
			Vector3d* accBias,
			int numSamples
			); /*0 = success*/

	inline bool IsInTextMode() { return !!m_File; }
private:
	void Close();

private:
	ImuDevice m_AccDevice;
	ImuDevice m_MagDevice;
	ImuDevice m_GyroDevice;
	FILE* m_File;
};

#endif /* SENSORREADER_H_ */
