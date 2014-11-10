/* * quadrotorpilot.cpp
 *
 *  Created on: May 27, 2013
 *      Author: svassilev
 */
#include "pidpilotsd.h"
#include <dlfcn.h>

PidPilot::PidPilot()
{
	m_minThrust = 0.0;
	m_maxThrust = 0.0;
	m_runtime = 0;
	m_refCnt = 1;
	m_targetThrust = 0.0;

	Vector3d TrustDir(0, 0, 1);
	m_M0 = Vector3d::cross(Vector3d( 1.0, -1.0, 0.0), TrustDir).normalize();
	m_M1 = Vector3d::cross(Vector3d( 1.0,  1.0, 0.0), TrustDir).normalize();
	m_M2 = Vector3d::cross(Vector3d(-1.0,  1.0, 0.0), TrustDir).normalize();
	m_M3 = Vector3d::cross(Vector3d(-1.0, -1.0, 0.0), TrustDir).normalize();

}

PidPilot::~PidPilot()
{
	assert(0 == m_refCnt);
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
		&m_runtime);
	return err;
}

int PidPilot::ExecuteCommand(
		SdCommandParams* params)
{
	int err = SD_ESUCCESS;
	switch (params->CommandCode()) {
	case SD_COMMAND_RUN:
		err = Start(&params->Params().asDroneConfig());
		break;
	case SD_COMMAND_RESET:
		Stop(false);
		break;
	case SD_COMMAND_EXIT:
		Stop(true);
		break;
	case SD_COMMAND_SET_THRUST:
		SetMinRev(params->Params().asThrust().MinThrust());
		SetMaxRev(params->Params().asThrust().MaxThrust());
		m_targetThrust = fmax(fmin(m_maxThrust,params->Params().asThrust().Thrust()),m_minThrust);
		break;
	default:break;
	}
	return err;
}

int PidPilot::Start(const SdDroneConfig* config)
{
	assert(config);
	assert(m_runtime);

	m_config = config->Quad;
	m_pid.Reset(m_config.Kp * 1000.0, m_config.Ki * 1000.0, m_config.Kd * 1000.0);

	m_runtime->StartStopIoDispatchThread(true);

	m_runtime->SetIoFilters(
			SD_DEVICEID_TO_FLAG(SD_DEVICEID_IMU),
			SD_IOCODE_TO_FLAG(SD_IOCODE_RECEIVE));

	return 0;
}

int PidPilot::AddRef()
{
	return __sync_fetch_and_add(&m_refCnt,1);
}

int PidPilot::Release()
{
	int refCnt = __sync_sub_and_fetch(&m_refCnt,1);
	if (0 == refCnt) {
		delete this;
	}
	return refCnt;
}

void PidPilot::Stop(bool detach)
{
	m_runtime->StartStopIoDispatchThread(false);
	m_minThrust = m_maxThrust = 0;
	m_motors = Vector4d();
	if (detach) {
		m_runtime->DetachPlugin();
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


		/*
		 *  Set the motor values in the IO structures so it can be used by the
		 *  rest of the chain
		 */
		ioPacket->SetAttribute(SDIO_ATTR_MOTORS,SdIoData(m_motors));
	} else {
		assert(false);
	}
	return err;
}

int PidPilot::IssueCommandToServo(const Vector4d& motors)
{
	/**
	 * Issue commands to the servos. Since the servos are controlled by
	 * different device, we must issue new SdIoPacket "down" to the servo
	 * device.
	 */
	int err = ENOMEM;
	SdServoIoData servoData;
	SdIoPacket* ioPacket = m_runtime->AllocIoPacket(
			SD_IOCODE_SEND,
			SD_DEVICEID_SERVO,
			SD_PLUGIN_SERVO_PCA9685);
	if (0 != ioPacket) {
		servoData.numChannels = 4;
		for (size_t i = 0; i < ARRAYSIZE(m_config.Motor); i++) {
			servoData.channels[i] = m_config.Motor[i];
			servoData.value[i] = motors.at(i,0);
		}
		ioPacket->SetIoData(SdIoData(servoData),true);
		err = m_runtime->DispatchIo(ioPacket,SD_FLAG_DISPATCH_DOWN);
		m_runtime->FreeIoPacket(ioPacket);
	} else {
		m_runtime->Log(SD_LOG_LEVEL_ERROR,
				"Quadrotor pilot failed to alloc iopacket!\n");
	}
	return err;
}

int PidPilot::IoDispatchThread()
{
	Vector4d motors = m_motors;
	while (motors.at(0,0) != m_motors.at(0,0) ||
			motors.at(1,0) != m_motors.at(1,0) ||
			motors.at(2,0) != m_motors.at(2,0) ||
			motors.at(3,0) != m_motors.at(3,0))  {
		motors = m_motors;
	}
	return IssueCommandToServo(motors);
}

int PidPilot::UpdateState(
	SdIoPacket* ioPacket)
{
	QuaternionD attitudeQ = ioPacket->Attitude();
	const QuaternionD targetQ = ioPacket->TargetAttitude();
	int retVal = 0;
	Vector3d _currentOmega = ioPacket->GetAttribute(SDIO_ATTR_GYRO).asVector3d();

	Vector3d torqueRPM;

	m_pid.SetTarget(targetQ);
	m_torqueCorrection = m_pid.GetTorque(attitudeQ, _currentOmega * M_PI / 180.0f,
			ioPacket->DeltaTime());

#if 0
	QuaternionD errQ = QuaternionD::fromAngularVelocity(m_pid.m_lastError, 1.0);
	m_ErrorAngle = errQ.angle();
	m_ErrorAxis = errQ.axis();
#endif

	//  From the motor trust measurement:
	//  0.6 --> 450g * 22.5cm
	//  0.6 --> (450/1000) * (22.5/100)
	//  0.6 --> 0.10125 kg.m
	torqueRPM = m_torqueCorrection * 0.6 / (101.25/1000.0) / 2.0;

	m_motors = Vector4d(
			m_targetThrust + Vector3d::dot(torqueRPM, m_M0),
			m_targetThrust + Vector3d::dot(torqueRPM, m_M1),
			m_targetThrust + Vector3d::dot(torqueRPM, m_M2),
			m_targetThrust + Vector3d::dot(torqueRPM, m_M3));

	SetAndScaleMotors(
			m_motors.at(0,0),
			m_motors.at(1,0),
			m_motors.at(2,0),
			m_motors.at(3,0));

	ioPacket->SetAttribute(SDIO_ATTR_ERR_PID,SdIoData(torqueRPM));

	return retVal;
}

void PidPilot::SetAndScaleMotors(
		double m1,
		double m2,
		double m3,
		double m4)
{
	Vector4d mv(m1,m2,m3,m4);
	double minVal = mv.minValue();
	if (minVal < m_minThrust) {
		mv = mv + (m_minThrust - minVal);
	}
	double maxVal = mv.maxValue();
	if (maxVal > m_maxThrust) {
		mv = mv - (maxVal - m_maxThrust);
	}
	maxVal = mv.maxValue();
	minVal = mv.minValue();
	if (minVal < m_minThrust || maxVal > m_maxThrust) {
		double scale = (m_maxThrust-m_minThrust)/(maxVal-minVal);
		mv = mv - minVal;
		mv = mv * scale;
		mv = mv + m_minThrust;
	}
	m_motors = mv;
}

const Vector4d* PidPilot::GetMotors() const
{
	return &m_motors;
}

void PidPilot::SetMinRev(double minRev)
{
	m_minThrust = fmax(0,minRev);
}

void PidPilot::SetMaxRev(double maxRev)
{
	m_maxThrust = fmin(1,maxRev);
}
