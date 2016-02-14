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
#include "tripilot.h"

TriPilot::TriPilot()
	: pf_(0.24, 1.0)
	, pid_(0.0, 0.0, 0.0, 80)
	, pidz_(0.0, 0.0, 0.0, 80)
{
	min_thrust_ = 0.0;
	max_thrust_ = 1.0;
	target_thrust_ = 0.0;
	max_integral_torque_ = 0.25;

	Vector3f thrust_dir(0, 0, -1);
	propellers_.push_back(Propeller(Vector3f(-1, -1,  0), thrust_dir, Propeller::CW));
	propellers_.push_back(Propeller(Vector3f(-1,  1,  0), thrust_dir, Propeller::CCW));
	propellers_.push_back(Propeller(Vector3f( 1,  1,  0), thrust_dir, Propeller::CW));
	propellers_.push_back(Propeller(Vector3f( 1,  -1,  0), thrust_dir, Propeller::CCW));
}

TriPilot::~TriPilot()
{
}

void TriPilot::set_pid_coefficents(const DroneState& state)
{
	pid_.set_kp_ki_kd(state.kp_, state.ki_, state.kd_);
	pid_.set_derivative_filter(state.pid_filter_freq_);
	pidz_.set_kp_ki_kd(state.yaw_kp_, state.yaw_ki_, state.yaw_kd_);
	pidz_.set_derivative_filter(state.pid_filter_freq_);
}

Vector3f TriPilot::get_torque(const DroneState& state)
{
	Vector3f torq_xy, torq_z, torq;
	QuaternionF twist, swing;
	QuaternionF::decomposeTwistSwing(~state.attitude_, Vector3f(0,0,1), swing, twist);

	// targetSwing = attitudeSwing * errorSwing; ==> ~attitudeSwing * targetSwing = ~attitudeSwing * attitudeSwing * errorSwing;
	QuaternionF errorSwing = (~swing) * (~target_swing_);
	QuaternionF errorTwist = (~twist) * (~target_twist_);
	Vector3f error_xy = errorSwing.axis().normalize() * errorSwing.angle() * -1.0f;
	Vector3f error_z = errorTwist.axis().normalize() * errorTwist.angle() * -1.0f;

	float ki = pid_.get_ki();
	Vector3f integral_error = pid_.get_integral_error();
	if (integral_error.length() * ki > max_integral_torque_ * target_thrust_) {
		integral_error = integral_error.normalize() * (max_integral_torque_ * target_thrust_ / ki);
		pid_.set_integral_error(integral_error);
	}
	torq_xy += pid_.get_p(error_xy);
	torq_xy += pid_.get_d_median(error_xy, state.dt_.seconds_float());
	torq_xy += pid_.get_i(error_xy, 1.50f * state.dt_.seconds_float(), state.dt_.seconds_float() / 10.0f);
	torq_z += pidz_.get_p(error_z);
	torq_z += pidz_.get_d_median(error_z, state.dt_.seconds_float());
	if (torq_z.length() > target_thrust_ * 0.6)
		torq_z = torq_z.normalize() * target_thrust_ * 0.6;
	torq = torq_xy + torq_z;
	if (target_thrust_ < 0.1)
		torq *= std::pow(target_thrust_ / 0.1, 2.0f);
//	std::cout << "Twist error: " << errorTwist.toString(4) << "( " << torq.transpose().to_string(4) << " )" << target_twist_.toString(4)
//			<< ", target_yawv_ rad/s: " << target_yawv_.transpose() << std::endl;
	return torq;
}

void TriPilot::update_state(DroneState& state)
{
	set_target_thrust(0.7 * state.base_throttle_);
	set_pid_coefficents(state);

	target_yawv_ = Vector3f(0.0f, 0.0f, 10.0f * state.yaw_throttle_factor_ * state.yaw_);
	if (target_yawv_.length() > M_PI * target_thrust_)
		target_yawv_ = target_yawv_.normalize() * M_PI * target_thrust_;
	target_twist_ *= QuaternionF::fromAngularVelocity(target_yawv_, state.dt_.seconds_float());
	target_swing_ = QuaternionF::fromAngularVelocity(Vector3f(-state.roll_, -state.pitch_, 0), 1.0);
	Vector3f torque_bias(state.roll_bias_, state.pitch_bias_, state.yaw_bias_);
	if (target_thrust_ < 0.1) {
		pid_.set_integral_error(Vector3f(0));
		target_twist_ = QuaternionF(state.attitude_.w, 0, 0, state.attitude_.z).normalize();
	}
	torque_correction_ = get_torque(state);
	Vector4f motors = Vector4f(
		torque_correction_.dot(propellers_.at(0).torque_dir()),
		torque_correction_.dot(propellers_.at(1).torque_dir()),
		torque_correction_.dot(propellers_.at(2).torque_dir()),
		torque_correction_.dot(propellers_.at(3).torque_dir()));
	motors += target_thrust_;
	Vector4f torq_rpm = Vector4f(
		pf_.nvelocity(motors[0]),
		pf_.nvelocity(motors[1]),
		pf_.nvelocity(motors[2]),
		pf_.nvelocity(motors[3])
	);
	motors_ = state.motors_ = clip_motors(torq_rpm);
	return;
}


Vector4f TriPilot::clip_motors(const Vector4f& motors)
{
	Vector4f ret;

	for (size_t i = 0; i < ret.data.size(); i++)
		ret[i] = std::max(min_thrust_, std::min(motors[i], max_thrust_));
	return ret;
}

const Vector4f& TriPilot::motors() const
{
	return motors_;
}

void TriPilot::set_min_thrust(float min)
{
	min_thrust_ = fmax(0.0, min);
}

void TriPilot::set_max_thrust(float max)
{
	max_thrust_ = fmin(1.0, max);
}

void TriPilot::set_target_thrust(float thr)
{
	target_thrust_ = fmax(fmin(max_thrust_, thr), min_thrust_);
}
