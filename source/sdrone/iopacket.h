/*
 * iopacket.h
 *
 *  Created on: Nov 5, 2013
 *      Author: svassilev
 */

#ifndef IOPACKET_H_
#define IOPACKET_H_

#include "commoninc.h"

class IoPacket: public SdIoPacket {
public:
	IoPacket(
			SdPluginAltitude startingAltitude,
			uint32_t ioCode,
			SdDeviceId deviceType,
			const char* pluginName);
	virtual ~IoPacket();
	virtual uint32_t IoCode(
			);
	virtual uint32_t Flags(
			);
	virtual const char* PluginName(
			);
	virtual SdDeviceId DeviceId(
			);
	virtual SdPluginAltitude StartingAltitude(
			);
	virtual const SdIoData& GetIoData(
			bool input
			);
	virtual void SetIoData(
			IN const SdIoData&,
			IN bool input
			);
	virtual const QuaternionD& Attitude(
			);
	const Vector3d& GyroData(
			);
	virtual const Vector3d& AccelData(
			);
	const Vector3d& MagData(
			);
	virtual const QuaternionD& TargetAttitude(
			);
	double DeltaTime(
			);
	virtual SdIoData GetAttribute(
			IN uint32_t attributeType
			);
	virtual int SetAttribute(
			IN uint32_t attributeType,
			IN const SdIoData& attribute
			);
private:
	static int SetAttrVector3d(const SdIoData& data, Vector3d* v);
	static int SetAttrVector4d(const SdIoData& data, Vector4d* v);
	static int SetAttrQ(const SdIoData& data, QuaternionD* q);
	static int SetAttrDouble(const SdIoData& data, double* d);
private:
	uint32_t m_flags;
	SdPluginAltitude m_startAltitude;
	uint32_t m_ioCode;
	const char* m_pluginName;
	SdDeviceId m_deviceId;
	SdIoData m_inData;
	SdIoData m_outData;
	QuaternionD m_attitudeQ;
	QuaternionD m_targetQ;
	Vector3d m_gyroDataDps;
	Vector3d m_accelData;
	Vector3d m_magData;
	Vector3d m_earthG;
	Vector4d m_motors;
	Vector3d m_errAxisPid;
	Vector3d m_errAxis;
	Vector3d m_errAxisP;
	Vector3d m_errAxisI;
	Vector3d m_errAxisD;
	Vector3d m_velocityFromAcc;
	Vector3d m_errOmega;
	double m_errAngle;
	double m_droneAltitude;
	double m_deltaTime; /*elapsed time since the last sample*/
	double m_timeToReadSensors;

};

#endif /* IOPACKET_H_ */