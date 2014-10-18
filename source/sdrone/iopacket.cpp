/*
 * IoPacket.cpp
 *
 *  Created on: Nov 5, 2013
 *      Author: svassilev
 */

#include "iopacket.h"

IoPacket::IoPacket(
		SdPluginAltitude startingAltitude,
		uint32_t ioCode,
		SdDeviceId deviceType,
		const char* pluginName) :
		m_flags(0),
		m_startAltitude(startingAltitude),
		m_ioCode(ioCode),
		m_pluginName(pluginName),
		m_deviceId(deviceType),
		m_errAngle(0),
		m_droneAltitude(0),
		m_deltaTime(0),
		m_timeToReadSensors(0)
{
}

IoPacket::~IoPacket()
{
}

uint32_t IoPacket::IoCode() {
	return m_ioCode;
}

uint32_t IoPacket::Flags() {
	return m_flags;
}

const char* IoPacket::PluginName() {
	return m_pluginName;
}

SdDeviceId IoPacket::DeviceId() {
	return m_deviceId;
}

SdPluginAltitude IoPacket::StartingAltitude() {
	return m_startAltitude;
}

const SdIoData& IoPacket::GetIoData(bool input) {
	return input ? m_inData : m_outData;
}

void IoPacket::SetIoData(const SdIoData& data, bool input) {
	if (input) { // TODO: clone the data if necessary
		m_inData = data;
	} else {
		m_outData = data;
	}
}

const QuaternionD& IoPacket::Attitude() {
	return m_attitudeQ;
}

const Vector3d& IoPacket::GyroData() {
	return m_gyroDataDps;
}

const Vector3d& IoPacket::AccelData() {
	return m_accelData;
}

const Vector3d& IoPacket::MagData() {
	return m_magData;
}

const QuaternionD& IoPacket::TargetAttitude() {
	return m_targetQ;
}

double IoPacket::DeltaTime() {
	return m_deltaTime;
}

SdIoData IoPacket::GetAttribute(
		uint32_t attributeType)
{
	switch (attributeType) {
	case SDIO_ATTR_ATTITUDE_Q:
		return SdIoData(m_attitudeQ);
	case SDIO_ATTR_TARGET_Q:
		return SdIoData(m_targetQ);
	case SDIO_ATTR_GYRO:
		return SdIoData(m_gyroDataDps);
	case SDIO_ATTR_ACCEL:
		return SdIoData(m_accelData);
	case SDIO_ATTR_MAG:
		return SdIoData(m_magData);
	case SDIO_ATTR_EARTH_G:
		return SdIoData(m_earthG);
	case SDIO_ATTR_MOTORS:
		return SdIoData(m_motors);
	case SDIO_ATTR_ERR_PID:
		return SdIoData(m_errAxisPid);
	case SDIO_ATTR_ERR_P:
		return SdIoData(m_errAxisP);
	case SDIO_ATTR_ERR_I:
		return SdIoData(m_errAxisI);
	case SDIO_ATTR_ERR_D:
		return SdIoData(m_errAxisD);
	case SDIO_ATTR_ERR_ANGLE:
		return SdIoData(m_errAngle);
	case SDIO_ATTR_DELTA_TIME:
		return SdIoData(m_deltaTime);
	case SDIO_ATTR_CORR_VELOCITY:
		return SdIoData(m_velocityFromAcc);
	case SDIO_ATTR_TIME_TO_READ_SENSORS:
		return SdIoData(m_timeToReadSensors);
	case SDIO_ATTR_ERR_OMEGA:
		return SdIoData(m_errOmega);
	case SDIO_ATTR_ALTITUDE:
	case SDIO_ATTR_TEMPERATURE:
	case SDIO_ATTR_COORDINATES:
	default:
		break;
	}
	return SdIoData();
}

int IoPacket::SetAttribute(
		uint32_t attributeType,
		const SdIoData& attr)
{
	switch (attributeType) {
	case SDIO_ATTR_ATTITUDE_Q:
		return SetAttrQ(attr, &m_attitudeQ);
	case SDIO_ATTR_TARGET_Q:
		return SetAttrQ(attr, &m_targetQ);
	case SDIO_ATTR_GYRO:
		return SetAttrVector3d(attr, &m_gyroDataDps);
	case SDIO_ATTR_ACCEL:
		return SetAttrVector3d(attr, &m_accelData);
	case SDIO_ATTR_MAG:
		return SetAttrVector3d(attr, &m_magData);
	case SDIO_ATTR_EARTH_G:
		return SetAttrVector3d(attr, &m_earthG);
	case SDIO_ATTR_MOTORS:
		return SetAttrVector4d(attr, &m_motors);
	case SDIO_ATTR_ERR_PID:
		return SetAttrVector3d(attr, &m_errAxisPid);
	case SDIO_ATTR_ERR_P:
		return SetAttrVector3d(attr, &m_errAxisP);
	case SDIO_ATTR_ERR_I:
		return SetAttrVector3d(attr, &m_errAxisI);
	case SDIO_ATTR_ERR_D:
		return SetAttrVector3d(attr, &m_errAxisD);
	case SDIO_ATTR_ERR_ANGLE:
		return SetAttrDouble(attr, &m_errAngle);
	case SDIO_ATTR_DELTA_TIME:
		return SetAttrDouble(attr, &m_deltaTime);
	case SDIO_ATTR_CORR_VELOCITY:
		return SetAttrVector3d(attr, &m_velocityFromAcc);
	case SDIO_ATTR_TIME_TO_READ_SENSORS:
		return SetAttrDouble(attr, &m_timeToReadSensors);
	case SDIO_ATTR_ERR_OMEGA:
		return SetAttrVector3d(attr, &m_errOmega);
	case SDIO_ATTR_ALTITUDE:
	case SDIO_ATTR_TEMPERATURE:
	case SDIO_ATTR_COORDINATES:
	default:
		break;
	}
	return SD_INVALID_ATTRIBUTE;
}

int IoPacket::SetAttrVector3d(
		const SdIoData& data,
		Vector3d* v)
{
	if (data.dataType()!= SdIoData::TYPE_VECTOR3D) {
		return EINVAL;
	}
	*v = data.asVector3d();
	return SD_ESUCCESS;
}

int IoPacket::SetAttrVector4d(
		const SdIoData& data,
		Vector4d* v)
{
	if (data.dataType() != SdIoData::TYPE_VECTOR4D) {
		return EINVAL;
	}
	*v = data.asVector4d();
	return SD_ESUCCESS;
}

int IoPacket::SetAttrQ(
		const SdIoData& data,
		QuaternionD* q)
{
	if (data.dataType()!= SdIoData::TYPE_QUATERNION) {
		return EINVAL;
	}
	*q = data.asQuaternion();
	return SD_ESUCCESS;
}

int IoPacket::SetAttrDouble(
		const SdIoData& data,
		double* d)
{
	if (data.dataType()!= SdIoData::TYPE_DOUBLE) {
		return EINVAL;
	}
	*d = data.asDouble();
	return SD_ESUCCESS;
}
