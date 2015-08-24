/* * pidpilot.cpp
 *
 *  Created on: May 27, 2013
 *      Author: svassilev
 */
#include "pidpilot.h"

PidPilot::PidPilot(float kp, float ki, float kd) {
	reset(kp, ki, kd);
}

PidPilot::~PidPilot() {
}

void PidPilot::reset_pid(float kp, float ki, float kd)
{
	pid_.reset(kp * 1000.0, ki * 1000.0, kd * 1000.0);
}

void PidPilot::reset(float kp, float ki, float kd) {
	min_thrust_ = 0.0;
	max_thrust_ = 1.0;
	target_thrust_ = 0.0;

	Vector3f thrust_dir(0, 0, 1);
	m0_ = Vector3f::cross(Vector3f(-1.0, 1.0, 0), thrust_dir).normalize();
	m1_ = Vector3f::cross(Vector3f(-1.0, -1.0, 0), thrust_dir).normalize();
	m2_ = Vector3f::cross(Vector3f(1.0,  -1.0, 0), thrust_dir).normalize();
	m3_ = Vector3f::cross(Vector3f(1.0, 1.0, 0), thrust_dir).normalize();
	m0_.at(2,0) = 0.5;
	m1_.at(2,0) = -0.5;
	m2_.at(2,0) = 0.5;
	m3_.at(2,0) = -0.5;
	reset_pid(kp, ki, kd);
}

void PidPilot::update_state(DroneState& state, const QuaternionF& target_attitude)
{
	Vector3f torque_rpm;

	pid_.set_target(target_attitude);
	torque_correction_ = pid_.get_torque(state.attitude_, state.dt_);
	state.last_twist_ = pid_.last_twist_;



	//  From the motor trust measurement:
	//  0.6 --> 450g * 22.5cm
	//  0.6 --> (450/1000) * (22.5/100)
	//  0.6 --> 0.10125 kg.m
	static const float rpm_coeff = 0.6 / (101.25/1000.0) / 2.0;
	torque_rpm.at(0,0) = torque_correction_.at(0,0) * rpm_coeff;
	torque_rpm.at(1,0) = torque_correction_.at(1,0) * rpm_coeff;
	torque_rpm.at(2,0) = torque_correction_.at(2,0) * rpm_coeff;

	motors_ = Vector4f(
			target_thrust_ + Vector3f::dot(torque_rpm, m0_),
			target_thrust_ + Vector3f::dot(torque_rpm, m1_),
			target_thrust_ + Vector3f::dot(torque_rpm, m2_),
			target_thrust_ + Vector3f::dot(torque_rpm, m3_));

	set_and_scale_motors(
			motors_.at(0,0),
			motors_.at(1,0),
			motors_.at(2,0),
			motors_.at(3,0));

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
	float min_val = mv.minValue();
	if (min_val < min_thrust_) {
		mv = mv + (min_thrust_ - min_val);
	}
	float max_val = mv.maxValue();
	if (max_val > max_thrust_) {
		mv = mv - (max_val - max_thrust_);
	}
	max_val = mv.maxValue();
	min_val = mv.minValue();
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
