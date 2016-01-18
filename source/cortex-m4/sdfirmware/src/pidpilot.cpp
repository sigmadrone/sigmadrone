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
#include "pidpilot.h"

PidPilot::PidPilot() {
	min_thrust_ = 0.0;
	max_thrust_ = 1.0;
	target_thrust_ = 0.0;

	Vector3f thrust_dir(0, 0, -1);
	m0_ = Vector3f::cross(Vector3f(-1, -1,  0), thrust_dir).normalize();
	m1_ = Vector3f::cross(Vector3f(-1,  1,  0), thrust_dir).normalize();
	m2_ = Vector3f::cross(Vector3f( 1,  1,  0), thrust_dir).normalize();
	m3_ = Vector3f::cross(Vector3f( 1, -1,  0), thrust_dir).normalize();
	m0_.at(2) = -0.5;
	m1_.at(2) = 0.5;
	m2_.at(2) = -0.5;
	m3_.at(2) = 0.5;
}

PidPilot::~PidPilot() {
}

void PidPilot::set_pid_coefficents(const DroneState& state)
{
	pid_.pid_roll_.set_kp_ki_kd(state.kp_, state.ki_, state.kd_);
	pid_.pid_roll_.set_derivative_filter(state.pid_filter_freq_);
	pid_.pid_pitch_.set_kp_ki_kd(state.kp_, state.ki_, state.kd_);
	pid_.pid_pitch_.set_derivative_filter(state.pid_filter_freq_);
	pid_.pid_yaw_.set_kp_ki_kd(state.yaw_kp_, state.yaw_ki_, state.yaw_kd_);
}

void PidPilot::update_state(DroneState& state)
{
	Vector3f torque_rpm;

	pid_.set_target(state.target_);
	set_target_thrust(state.base_throttle_);
	set_pid_coefficents(state);

	torque_correction_ = pid_.get_torque(state.attitude_, state.dt_, state.yaw_throttle_factor_);
	state.twist_ = pid_.twist_;



	//  From the motor trust measurement:
	//  0.6 --> 450g * 22.5cm
	//  0.6 --> (450/1000) * (22.5/100)
	//  0.6 --> 0.10125 kg.m
	static const float rpm_coeff = 0.6 / (101.25/1000.0) / 2.0;
	torque_rpm.at(0) = torque_correction_.at(0) * rpm_coeff;
	torque_rpm.at(1) = torque_correction_.at(1) * rpm_coeff;
	torque_rpm.at(2) = torque_correction_.at(2) * rpm_coeff;

	Vector3f torque_bias(state.roll_bias_, state.pitch_bias_, state.yaw_bias_);
	torque_rpm = torque_rpm + torque_bias;

	if (torque_rpm.length() > target_thrust_)
		torque_rpm = torque_rpm.normalize() * target_thrust_;

#ifndef PILOT_Z_COMPENSATE
	Vector3f torque_yaw(0.0, 0.0, state.yaw_throttle_factor_ * state.yaw_ * target_thrust_);
	motors_ = Vector4f(
			target_thrust_ + Vector3f::dot(torque_rpm, m0_) + Vector3f::dot(torque_yaw, m0_),
			target_thrust_ + Vector3f::dot(torque_rpm, m1_) + Vector3f::dot(torque_yaw, m1_),
			target_thrust_ + Vector3f::dot(torque_rpm, m2_) + Vector3f::dot(torque_yaw, m2_),
			target_thrust_ + Vector3f::dot(torque_rpm, m3_) + Vector3f::dot(torque_yaw, m3_));
#else
	motors_ = Vector4f(
			target_thrust_ + Vector3f::dot(torque_rpm, m0_),
			target_thrust_ + Vector3f::dot(torque_rpm, m1_),
			target_thrust_ + Vector3f::dot(torque_rpm, m2_),
			target_thrust_ + Vector3f::dot(torque_rpm, m3_));
#endif

	set_and_scale_motors(
			motors_.at(0),
			motors_.at(1),
			motors_.at(2),
			motors_.at(3));

	state.motors_ = motors();

	state.pid_torque_ = torque_rpm;

	return;
}

void PidPilot::set_and_scale_motors(
		float m1,
		float m2,
		float m3,
		float m4)
{
	Vector4f mv(m1,m2,m3,m4);
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
		float scale = (max_thrust_-min_thrust_)/(max_val-min_val);
		mv = mv - min_val;
		mv = mv * scale;
		mv = mv + min_thrust_;
	}
	motors_ = mv;
}

const Vector4f& PidPilot::motors() const
{
	return motors_;
}

void PidPilot::set_min_thrust(float min)
{
	min_thrust_ = fmax(0.0,min);
}

void PidPilot::set_max_thrust(float max)
{
	max_thrust_ = fmin(1.0,max);
}

void PidPilot::set_target_thrust(float thr) {
	target_thrust_ = fmax(fmin(max_thrust_, thr), min_thrust_);
}
