/*
 *  Sigmadrone
 *  Copyright (c) 2013-2017 The Sigmadrone Developers
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

#include "targetattitude.h"

TargetAttitude::TargetAttitude() :
	pitch_avg_(0.94),
	roll_avg_(0.94)
{
}

void TargetAttitude::update_state(DroneState& state)
{
	float target_thrust = state.base_throttle_ * 0.5f;
	float max_w = 2.0f * M_PI * target_thrust;
	float roll = roll_avg_.do_filter(state.roll_);
	float pitch = pitch_avg_.do_filter(state.pitch_);
	Vector3f target_yawv = Vector3f(0.0f, 0.0f, 18.0f * state.yaw_throttle_factor_ * state.yaw_);
	if (target_yawv.length() > max_w) {
		target_yawv = target_yawv.normalize() * max_w;
	}

	swing_ = QuaternionF::fromAngularVelocity(Vector3f(-roll, -pitch, 0), 1.0);
	twist_ *= QuaternionF::fromAngularVelocity(target_yawv, state.dt_.seconds_float());
	if (target_thrust < 0.1) {
		twist_ = QuaternionF(state.attitude_.w, 0, 0, state.attitude_.z).normalize();
	}
	state.target_swing_ = swing_;
	state.target_twist_ = twist_;
}

QuaternionF TargetAttitude::get() const
{
	return 	swing_ * twist_;
}

const QuaternionF& TargetAttitude::target_twist() const
{
	return twist_;
}

const QuaternionF& TargetAttitude::target_swing() const
{
	return swing_;
}
