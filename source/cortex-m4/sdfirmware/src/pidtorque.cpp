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
	set_Q_(1), pid_roll_(0.0,0.0,0.0,80.0), pid_pitch_(0.0,0.0,0.0,80), pid_yaw_(0.0,0.0,0.0)
{
}

PidTorque::~PidTorque()
{
}

void PidTorque::set_target(const QuaternionF &setQ)
{
	set_Q_ = setQ;
}

Vector3f PidTorque::get_torque(const DroneState& state)
{
	Vector3f torq;
	const QuaternionF &in_Q = state.attitude_;
	const TimeSpan& dt = state.dt_;
	Vector3f Zset = set_Q_.rotate(Vector3f(0.0, 0.0, 1.0));
	Vector3f Zin = in_Q.conjugate().rotate(Vector3f(0.0, 0.0, 1.0));
	QuaternionF Qtorq = QuaternionF::fromVectors(Zin, Zset);
	Vector3f error = Qtorq.axis().normalize() * Qtorq.angle() * -1.0;

	torq[0] = pid_roll_.get_p(error[0]) + pid_roll_.get_d_median(error[0],dt.seconds_float());
	torq[0] += pid_roll_.get_i(
			(state.base_throttle_ > 0.05) ? torq[0] : 0,
			dt.seconds_float(),
			dt.seconds_float()/20.0f);

	torq[1] = pid_pitch_.get_p(error[1]) + pid_pitch_.get_d_median(error[1],dt.seconds_float());
	torq[1] += pid_pitch_.get_i(
			(state.base_throttle_ > 0.05) ? torq[1] : 0,
			dt.seconds_float(),
			dt.seconds_float()/20.0f);

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
	float torq_yaw = pid_yaw_.get_pd(error_z.at(2), dt.seconds_float());
	float yaw_factor = state.yaw_throttle_factor_ / 3.0;
	torq_yaw = std::max(-yaw_factor, std::min(torq_yaw, yaw_factor));
	torq.at(2) = torq_yaw;

	return torq;
}
