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

#ifndef _PIDTORQUE_H_
#define _PIDTORQUE_H_

class pidtorque
{
public:
	pidtorque();
	~pidtorque();

	void set_target(const QuaternionD &setQ);
	Vector3d get_xy_torque(const QuaternionD &inQ, const Vector3d& Omega, double dT);
	Vector3d get_z_torque(const QuaternionD &inQ, const Vector3d& Omega, double dT);
	void reset(double Kp, double Ki, double Kd);

public:
	QuaternionD target_q_;
	QuaternionD old_q_;
	Vector3d error_;
	Vector3d integral_error_;
	timespec previous_time_;
	double Kp_, Ki_, Kd_;
};




#endif

