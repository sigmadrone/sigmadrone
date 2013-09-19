/* * pidpilot.cpp
 *
 *  Created on: Aug 07, 2013
 *      Author: Martin Stoilov
 */
#include "pidpilot.h"
#include "servodevice.h"

static const double s_InitialStep = 0.005;

PidPilot::PidPilot() :
			m_PidCtl(0,0,0,0),
			m_MotorFilt()
{
	static const double filtCoeff[] = {
			0.0207,0.0656,0.1664,0.2473,0.2473,0.1664,0.0656,0.0207
	};
	m_MotorFilt.InitCoeff(filtCoeff);
	m_Servo = 0;
	m_MinRev = 0.0;
	m_MaxRev = 1.0;
	m_Counter = 0;
	m_Dt = 1.0/200;
}

PidPilot::~PidPilot() {}

int PidPilot::Reset(
		const SdDroneConfig* config,
		ServoDevice* servo)
{
	assert(config);
	assert(servo);
	m_Config = config->Quad;
	m_Servo = servo;
	m_PidCtl.Reset(m_Config.Kp,m_Config.Ki,m_Config.Kd,1.0/config->Gyro.Rate);
	m_Dt = 1.0/config->Gyro.Rate;
	m_Counter = 0;
	m_Motors.clear();
	m_PrevErrQ = QuaternionD(1, 0, 0, 0);
	m_TorqueCorrection.clear();
	m_Omega.clear();
	m_AngAccel.clear();
	m_Step = s_InitialStep;
	m_RotZQ = QuaternionD::fromAxisRot(
		Vector3d(0,0,1),-m_Config.ImuAngleAxisZ);
	return 0;
}


int PidPilot::Update(
		const QuaternionD& attitudeQ,
		const QuaternionD& targetQ,
		const Vector3d& _currentOmega,
		double _targetThrust)
{
	int retVal = 0;

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
	if (minVal < m_MinRev) {
		mv = mv + (m_MinRev - minVal);
	}
	double maxVal = mv.maxValue();
	if (maxVal > m_MaxRev) {
		mv = mv - (maxVal - m_MaxRev);
	}
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
