/* * quadrotorpilot.cpp
 *
 *  Created on: May 27, 2013
 *      Author: svassilev
 */
#include "pidpilotsd.h"
#include <dlfcn.h>

static const double s_InitialStep = 0.005;

PidPilot::PidPilot()
{
	m_MinRev = 0.0;
	m_MaxRev = 0.0;
	m_Counter = 0;
	m_Skip = 0;
	m_ErrorAngle = 0;
	m_Runtime = 0;
	m_RefCnt = 1;
	m_targetThrust = 0.0;

	Vector3d TrustDir(0, 0, 1);
	m_M0 = Vector3d::cross(Vector3d( 1.0, -1.0, 0.0), TrustDir).normalize();
	m_M1 = Vector3d::cross(Vector3d( 1.0,  1.0, 0.0), TrustDir).normalize();
	m_M2 = Vector3d::cross(Vector3d(-1.0, -1.0, 0.0), TrustDir).normalize();
	m_M3 = Vector3d::cross(Vector3d(-1.0,  1.0, 0.0), TrustDir).normalize();

}

PidPilot::~PidPilot()
{
	assert(0 == m_RefCnt);
}

int PidPilot::AttachToChain(
	void* droneContext,
	SdPluginAttachFunc attachFunc)
{
	static const char* attachAbove[] = {SD_PLUGIN_NAVIGATOR,0};
	int err = attachFunc(
		droneContext,
		this,
		SD_ALTITUDE_GROUP_APP+SD_ALTITUDE_FIRST/2,
		attachAbove,
		0,
		&m_Runtime);
	return err;
}

int PidPilot::ExecuteCommand(
		SdCommandParams* params)
{
	int err = SD_ESUCCESS;
	switch (params->CommandCode()) {
	case SD_COMMAND_RUN:
		err = Start(params->Params().asDroneConfig);
		break;
	case SD_COMMAND_RESET:
		Stop(false);
		break;
	case SD_COMMAND_EXIT:
		Stop(true);
		break;
	case SD_COMMAND_SET_THRUST:
		SetMinRev(params->Params().asThrust->MinThrust());
		SetMaxRev(params->Params().asThrust->MaxThrust());
		m_targetThrust = fmax(fmin(m_MaxRev,params->Params().asThrust->Thrust()),m_MinRev);
		break;
	default:break;
	}
	return err;
}

int PidPilot::Start(const SdDroneConfig* config)
{
	assert(config);
	assert(m_Runtime);
	m_Config = config->Quad;
	m_RotZQ = QuaternionD::fromAxisRot(
		Vector3d(0,0,1),DEG2RAD(-m_Config.ImuAngleAxisZ));

	m_Runtime->SetIoFilters(
			SD_DEVICEID_TO_FLAG(SD_DEVICEID_IMU),
			SD_IOCODE_TO_FLAG(SD_IOCODE_RECEIVE));

	return 0;
}

int PidPilot::AddRef()
{
	return __sync_fetch_and_add(&m_RefCnt,1);
}

int PidPilot::Release()
{
	int refCnt = __sync_sub_and_fetch(&m_RefCnt,1);
	if (0 == refCnt) {
		delete this;
	}
	return refCnt;
}

void PidPilot::Stop(bool detach)
{
	m_MinRev = m_MaxRev = 0;
	CalcThrustFromErrAxis(Vector3d(0,0,0),0);
	IssueCommandToServo();
	if (detach) {
		m_Runtime->DetachPlugin();
	}
}

const char* PidPilot::GetName()
{
	return SD_PLUGIN_PIDPILOT;
}

SdDeviceId PidPilot::GetDeviceId()
{
	return SD_DEVICEID_FILTER;
}

const char* PidPilot::GetVersion()
{
	return SD_PLUGIN_VERSION;
}

extern char **g_Argv;
const char* PidPilot::GetDlFileName()
{
	return g_Argv[0];
}

int PidPilot::IoCallback(
	SdIoPacket* ioPacket)
{
	int err = 0;
	if (SD_IOCODE_RECEIVE == ioPacket->IoCode()) {

		/*
		 * Check whether the IO carries all the necessary info for the pilot
		 */
		UpdateState(ioPacket);

		if (0 == (m_Counter++%(m_Skip+1)))
		{
			//
			// Issue commands to the servos. Since the servos are controlled by
			// different device, we must issue new SdIoPacket "down" to the servo
			// device. We do not want to stop the current packet as there may
			// be another plugin on top that performs different functions, aka
			// logging, etc.
			//
			err = IssueCommandToServo();
		}
		/*
		 *  Set the motor values in the IO structures so it can be used by the
		 *  rest of the chain
		 */
		ioPacket->SetAttribute(SDIO_ATTR_MOTORS,SdIoData(&m_Motors));
	} else {
		assert(false);
	}
	return err;
}

int PidPilot::IssueCommandToServo()
{
	//
	// Since the servos are controlled by different device, we must issue new
	// SdIoPacket "down" to the servo device.
	//
	int err = ENOMEM;
	SdServoIoData servoData;
	SdIoPacket* ioPacket = m_Runtime->AllocIoPacket(
			SD_IOCODE_SEND,
			SD_DEVICEID_SERVO,
			SD_PLUGIN_SERVO_PCA9685);
	if (0 != ioPacket) {
		servoData.numChannels = 4;
		for (size_t i = 0; i < ARRAYSIZE(m_Config.Motor); i++) {
			servoData.channels[i] = m_Config.Motor[i];
			servoData.value[i] = m_Motors.at(i,0);
		}
		ioPacket->SetIoData(SdIoData(&servoData),true);
		err = m_Runtime->DispatchIo(ioPacket,SD_FLAG_DISPATCH_DOWN);
		m_Runtime->FreeIoPacket(ioPacket);
	} else {
		m_Runtime->Log(SD_LOG_LEVEL_ERROR,
				"Quadrotor pilot failed to alloc iopacket!\n");
	}
	return err;
}


int PidPilot::UpdateState(
	SdIoPacket* ioPacket)
{
	QuaternionD attitudeQ = ioPacket->Attitude();
	const QuaternionD targetQ = ioPacket->TargetAttitude();
	int retVal = 0;
	Vector3d _currentOmega = *(ioPacket->GetAttribute(SDIO_ATTR_GYRO).asVector3d);

	Vector3d TorqueRPM;

	m_pid.SetTarget(targetQ);
	m_TorqueCorrection = m_pid.GetTorque(attitudeQ, _currentOmega * M_PI / 180.0f,
			ioPacket->DeltaTime());

	QuaternionD errQ = QuaternionD::fromAngularVelocity(m_pid.m_lastError, 1.0);
	m_ErrorAngle = errQ.angle();
	m_ErrorAxis = errQ.axis();

	//  From the motor trust measurement:
	//  0.6 --> 450g * 22.5cm
	//  0.6 --> (450/1000) * (22.5/100)
	//  0.6 --> 0.10125 kg.m
	TorqueRPM = m_TorqueCorrection * 0.6 / (101.25/1000.0) / 2.0;

	m_Motors = Vector4d(
			m_targetThrust + Vector3d::dot(TorqueRPM, m_M0),
			m_targetThrust + Vector3d::dot(TorqueRPM, m_M1),
			m_targetThrust + Vector3d::dot(TorqueRPM, m_M2),
			m_targetThrust + Vector3d::dot(TorqueRPM, m_M3));

#if 0
	ioPacket->SetAttribute(SDIO_ATTR_ERR_PID,SdIoData(&m_ErrorAxisPid));
	ioPacket->SetAttribute(SDIO_ATTR_ERR_P,SdIoData(&m_ErrorP));
	ioPacket->SetAttribute(SDIO_ATTR_ERR_I,SdIoData(&m_ErrorI));
	ioPacket->SetAttribute(SDIO_ATTR_ERR_D,SdIoData(&m_ErrorD));
	ioPacket->SetAttribute(SDIO_ATTR_ERR_ANGLE,SdIoData(angleDeg));
	ioPacket->SetAttribute(SDIO_ATTR_ERR_OMEGA,SdIoData(&omegaErrPid));
#endif
	return retVal;
}

void PidPilot::CalcThrustFromErrAxis(
		const Vector3d& err,
		double _targetThrust)
{
	//
	// Control the motors in pairs 1-3 and 2-4.
	// We use the following equations:
	// 		torgX = l * k * (m4^2 - m2^2);
	// 		torgY = l * k * (m3^2 - m1^2);
	// 		torgZ = b * (-m1^2 + m2^2 - m3^2 + m4^2);
	// 		thrust = k * (m1^2 + m2^2 + m3^2 + m4^2);
	// Since the rotation axis is exactly the same as the desired torque,
	// we need to find w (motor rot velocity) deltas, that satisfy
	// the equations above.
	//
	double targetThrust = fmax(_targetThrust,0.01);
	double deltaX = err.at(0,0)/(4 * targetThrust);
	double deltaY = err.at(1,0)/(4 * targetThrust);
	double deltaZ = err.at(2,0)/(8 * targetThrust);
	double m1 = targetThrust;
	double m2 = targetThrust;
	double m3 = targetThrust;
	double m4 = targetThrust;

	m2 -= deltaX;
	m4 += deltaX;
	m1 -= deltaY;
	m3 += deltaY;

	m1 -= deltaZ;
	m3 -= deltaZ;
	m2 += deltaZ;
	m4 += deltaZ;

	SetAndScaleMotors(m1,m2,m3,m4);
}

void PidPilot::SetAndScaleMotors(
		double m1,
		double m2,
		double m3,
		double m4)
{
	Vector4d mv(m1,m2,m3,m4);
	double minVal = mv.minValue();
	if (minVal < m_MinRev) {
		mv = mv + (m_MinRev - minVal);
	}
	double maxVal = mv.maxValue();
	if (maxVal > m_MaxRev) {
		mv = mv - (maxVal - m_MaxRev);
	}
	maxVal = mv.maxValue();
	minVal = mv.minValue();
	if (minVal < m_MinRev || maxVal > m_MaxRev) {
		double scale = (m_MaxRev-m_MinRev)/(maxVal-minVal);
		mv = mv - minVal;
		mv = mv * scale;
		mv = mv + m_MinRev;
	}
	m_Motors = mv;
}

const Vector4d* PidPilot::GetMotors() const
{
	return &m_Motors;
}

void PidPilot::SetMinRev(double minRev)
{
	m_MinRev = fmax(0,minRev);
}

void PidPilot::SetMaxRev(double maxRev)
{
	m_MaxRev = fmin(1,maxRev);
}
