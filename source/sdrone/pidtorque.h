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

#include <sys/time.h>
#include "matrix.h"
#include "pidcontroller.h"

#ifndef _PIDTORQUE_H_
#define _PIDTORQUE_H_

class PidTorque
{
public:
	PidTorque();
	~PidTorque();

	void SetTarget(const QuaternionD &setQ);
	Vector3d GetTorque(const QuaternionD &inQ, const Vector3d& Omega, double dT);
	void Reset(double Kp, double Ki, double Kd);

public:
	QuaternionD m_setQ;
	QuaternionD m_oldQ;
	Vector3d m_lastError;
	Vector3d m_integralError;
	timespec m_lastTime;
	double m_Kp, m_Ki, m_Kd;
	PidController3d m_pidController;
};




#endif

