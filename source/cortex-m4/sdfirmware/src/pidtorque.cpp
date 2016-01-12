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
	set_Q_(1), pid_controller_(0.0,0.0,0.0), pid_controller_yaw_(0.0,0.0,0.0)
{
}

PidTorque::~PidTorque()
{
}

void PidTorque::set_target(const QuaternionF &setQ)
{
	set_Q_ = setQ;
}

Vector3f PidTorque::get_torque(const QuaternionF &in_Q, const TimeSpan& dt, float yaw_factor)
{
	Vector3f torq;
	Vector3f Zset = set_Q_.rotate(Vector3f(0.0, 0.0, 1.0));
	Vector3f Zin = in_Q.conjugate().rotate(Vector3f(0.0, 0.0, 1.0));
	QuaternionF Qtorq = QuaternionF::fromVectors(Zin, Zset);
	Vector3f error = Qtorq.axis().normalize() * Qtorq.angle() * -1.0;

	torq = pid_controller_.get_pid(error,dt.seconds_float());

#ifdef PILOT_Z_COMPENSATE
	// targetQ = attitudeQ * errQ; ==> (~attitudeQ) * attitudeQ * errQ = (~attitudeQ) * targetQ;
	// ==> errQ = (~attitudeQ) * targetQ;
	Qtorq = (~in_Q) * set_Q_;
	Vector3f error_z = Qtorq.axis().normalize();
	float angle_rad = Qtorq.angle();
	if (fabs(angle_rad) > M_PI) {
		if (angle_rad > 0) {
			angle_rad -= 2.0 * M_PI;
		} else {
			angle_rad += 2.0 * M_PI;
		}
	}
	error_z = error_z * angle_rad;
	torq.at(2) = pid_controller_yaw_.get_pid(
			error_z,
			dt.seconds_float(),
			Vector3f(0,0,yaw_factor/3.0)).at(2);
#else
	// targetQ = attitudeQ * errQ; ==> (~attitudeQ) * attitudeQ * errQ = (~attitudeQ) * targetQ;
	// ==> errQ = (~attitudeQ) * targetQ;
	Qtorq = (~in_Q) * set_Q_;
	QuaternionF swing;
	QuaternionF::decomposeTwistSwing(Qtorq, Vector3f(0,0,1), swing, twist_);
	Vector3f error_z = twist_.axis().normalize() * twist_.angle() * 1.0;
	error_z.at(0) = error_z.at(1) = 0.0;
	torq.at(2) = pid_controller_yaw_.get_d(error_z, dt.seconds_float()).at(2);
	error.at(2) = error_z.at(2);
#endif

	return torq;
}
