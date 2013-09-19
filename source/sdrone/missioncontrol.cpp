/*
 * missioncontrol.cpp
 *
 *  Created on: May 26, 2013
 *      Author: svassilev
 */
#include "missioncontrol.h"

MissionControl::MissionControl()
{
	m_Thrust = 0.0;
	m_HoverThrust = 0.0;
	m_DesiredAttitude = QuaternionD(1,0,0,0);
}

MissionControl::~MissionControl()
{
}

int MissionControl::Reset(const SdDroneConfig* droneConfig)
{
	return 0;
}

const QuaternionD& MissionControl::GetDesiredAttitude()
{
	return m_DesiredAttitude;
}

double MissionControl::GetDesiredThrust()
{
	return m_Thrust;
}

void MissionControl::SetDesiredAttitude(const QuaternionD* att)
{
	m_DesiredAttitude = *att;
}

void MissionControl::IncThrust(double increment)
{
	m_Thrust += increment;
	m_Thrust = fmin(m_Thrust,1.0);
	m_Thrust = fmax(m_Thrust,0.0);
}

void MissionControl::ResetThrust(double thrust)
{
	m_Thrust = thrust;
	m_Thrust = fmin(m_Thrust,1.0);
	m_Thrust = fmax(m_Thrust,0.0);
}
