/*
 *  Sigmadrone
 *  Copyright (c) 2013-2015 The Sigmadrone Developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Martin Stoilov <martin@sigmadrone.org>
 *  Svetoslav Vassilev <svassilev@sigmadrone.org>
 */
#include <stdio.h>
#include "pidtorque.h"

PidTorque::PidTorque() :
	m_setQ(1), m_oldQ(1), m_pidController(0.0,0.0,0.0)
{
	Reset(0.0, 0.0, 0.0);
}

PidTorque::~PidTorque()
{

}

void PidTorque::Reset(double Kp, double Ki, double Kd)
{
	m_Kp = Kp / 1000.0 * 22.5 / 100.0 / (3.1415/2);
	m_Ki = Ki / 1000.0 * 22.5 / 100.0 / (3.1415/2);
	m_Kd = Kd / 1000.0 * 22.5 / 100.0 / (3.1415/2);
	m_pidController.reset(m_Kp,0,m_Kd,0);
	m_integralError = Vector3d();
	m_lastError = Vector3d();
}

void PidTorque::SetTarget(const QuaternionD &setQ)
{
	m_setQ = setQ;
}


Vector3d PidTorque::GetTorque(const QuaternionD &inQ, const Vector3d& Omega, double dT)
{
//	Vector3d Ti = Vector3d(0.0, 0.018, 0.0);
	Vector3d torq;
	Vector3d differentialError;
	Vector3d Zset = m_setQ.rotate(Vector3d(0.0, 0.0, 1.0));
	Vector3d Zin = (~inQ).rotate(Vector3d(0.0, 0.0, 1.0));
	QuaternionD Qtorq = QuaternionD::fromVectors(Zin, Zset);
	Vector3d error = Qtorq.axis().normalize() * Qtorq.angle() * -1.0;
//	Vector3d error = QuaternionD::angularVelocity(~inQ, m_setQ, 1.0) * -1.0;

	double err = error.length();
	m_integralError = m_integralError + error * dT;
	m_integralError = m_integralError - m_integralError * 0.000001;
	if (m_integralError.length() * m_Ki > 30.0/1000.0)
		m_integralError = m_integralError * 0.85;
	differentialError = (error - m_lastError)/dT;
//	differentialError = Omega * M_PI / 180.0f * -1.0f;
//	torq = error.normalize() * pow(err, 1.05) * m_Kp +
//			differentialError * m_Kd +
//			m_integralError.normalize() * pow(m_integralError.length(), 1.3) * m_Ki * 0.0;


	torq = error.normalize() * pow(err, 1.00) * m_Kp +
			differentialError * m_Kd +
			m_integralError.normalize() * pow(m_integralError.length(), 1.3) * m_Ki * 0.0;

	torq = m_pidController.get_pid(error,dT);

//	torq = error.normalize() * err * m_Kp;
//	torq = m_integralError.normalize() * pow(m_integralError.length(), 1.3) * m_Ki;
//	torq = differentialError * m_Kd;

	m_lastError = error;
	return torq;
}
