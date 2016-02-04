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
	: pid_(0.0, 0.0, 0.0, 80)
{
	min_thrust_ = 0.0;
	max_thrust_ = 1.0;
	target_thrust_ = 0.0;
	max_integral_torque_ = 0.3;
	leak_rate_ = 0.015;

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
}

Vector3f TriPilot::get_torque(const DroneState& state)
{
	//  From the motor trust measurement:
	//  0.6 --> 450g * 22.5cm
	//  0.6 --> (450/1000) * (22.5/100)
	//  0.6 --> 0.10125 kg.m
	static const float rpm_coeff = 0.6 / (101.25/1000.0) / 2.0;

	Vector3f torq;
	QuaternionF twist, swing;
	QuaternionF::decomposeTwistSwing(~state.attitude_, Vector3f(0,0,1), swing, twist);

	// targetSwing = attitudeSwing * errorSwing; ==> ~attitudeSwing * targetSwing = ~attitudeSwing * attitudeSwing * errorSwing;
	QuaternionF errorSwing = (~swing) * state.target_swing_;
	/*
	 * RcValueConverter::reset_twist_quaternion():
	 * QuaternionF swing;
	 * QuaternionF::decomposeTwistSwing(~current_q, Vector3f(0,0,1), swing, target_twist_);
	 *
	 * RcValueConverter::update()
	 * yaw_ = ((yaw - 0.5) * MAX_EULER_FROM_RC * scale_factor_ * 2);
	 *
	 * then we can have:
	 * QuaternionF errorTwist = (~twist) * state.target_twist_;
	 */
	QuaternionF errorTwist = (~twist) * (~state.target_twist_);
	Vector3f error_xy = errorSwing.axis().normalize() * errorSwing.angle() * -1.0;
	Vector3f error_z = errorTwist.axis().normalize() * errorTwist.angle() * -1.0;
	Vector3f error = error_xy + error_z;
	Vector3f torq_p, torq_i, torq_d;

#ifndef USE_INTEGRAL
	float ki = pid_.get_ki() * rpm_coeff;
	Vector3f integral_error = pid_.get_integral_error();
	if (integral_error.length() * ki > max_integral_torque_ * target_thrust_) {
		integral_error = integral_error.normalize() * (max_integral_torque_ * target_thrust_ / ki);
		pid_.set_integral_error(integral_error);
	}

	torq_p = pid_.get_p(error) * rpm_coeff;
	torq_d = pid_.get_d(error, state.dt_.seconds_float()) * rpm_coeff;
	torq_i = pid_.get_i((target_thrust_ > 0.05) ? error_xy : Vector3f(0.0f), 4.0f/5.0f * state.dt_.seconds_float(), state.dt_.seconds_float()/8.0f) * rpm_coeff;
	torq = torq_p + torq_d + torq_i;
	if (torq.length() > target_thrust_ * 0.45)
		torq = torq.normalize() * target_thrust_ * 0.45;

#else
	torq = pid_.get_pid(error, state.dt_.seconds_float());
#endif

	return torq;
}

void TriPilot::update_state(DroneState& state)
{
	set_target_thrust(0.7 * state.base_throttle_);
	set_pid_coefficents(state);
	torque_correction_ = get_torque(state);

	Vector3f torque_bias(state.roll_bias_, state.pitch_bias_, state.yaw_bias_);
	torque_correction_ += torque_bias;
	Vector3f torque_yaw(0.0, 0.0, state.yaw_throttle_factor_ * state.yaw_ * target_thrust_);
	torque_correction_ += torque_yaw;

	Vector4f motors = Vector4f(
			torque_correction_.dot(propellers_.at(0).torque_dir()),
			torque_correction_.dot(propellers_.at(1).torque_dir()),
			torque_correction_.dot(propellers_.at(2).torque_dir()),
			torque_correction_.dot(propellers_.at(3).torque_dir()));
	motors += target_thrust_;
	motors_ = state.motors_ = set_and_scale_motors(motors);

//	std::cout << motors.transpose() << std::endl;
//	Vector3f integral_error = pid_.get_integral_error();
//	std::cout << integral_error.transpose().to_string(5) << "(" << state.motors_.transpose() << ")" << "dt: " << state.dt_.seconds_float() << std::endl;
	return;
}

Vector4f TriPilot::set_and_scale_motors(const Vector4f& motors)
{
	Vector4f mv(motors);
	float min_val = mv.min();
	if (min_val < min_thrust_) {
		mv = mv + (min_thrust_ - min_val);
	}
	float max_val = mv.max();
	if (max_val > max_thrust_) {
		mv = mv - (max_val - max_thrust_);
	}
	max_val = mv.max();
	min_val = mv.min();
	if (min_val < min_thrust_ || max_val > max_thrust_) {
		float scale = (max_thrust_ - min_thrust_) / (max_val - min_val);
		mv = mv - min_val;
		mv = mv * scale;
		mv = mv + min_thrust_;
	}
	return motors;
}

const Vector4f& TriPilot::motors() const
{
	return motors_;
}

void TriPilot::set_min_thrust(float min)
{
	min_thrust_ = fmax(0.0,min);
}

void TriPilot::set_max_thrust(float max)
{
	max_thrust_ = fmin(1.0,max);
}

void TriPilot::set_target_thrust(float thr) {
	target_thrust_ = fmax(fmin(max_thrust_, thr), min_thrust_);
}
