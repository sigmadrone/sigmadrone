#include <stdio.h>
#include "pidtorque.h"

PidTorque::PidTorque() :
	set_Q_(1), pid_controller_(0.0,0.0,0.0)
{
	reset(0.0, 0.0, 0.0);
}

PidTorque::~PidTorque()
{
}

void PidTorque::reset(float kp, float ki, float kd)
{
	kp_ = kp / 1000.0 * 22.5 / 100.0 / (3.1415/2);
	ki_ = ki / 1000.0 * 22.5 / 100.0 / (3.1415/2);
	kd_ = kd / 1000.0 * 22.5 / 100.0 / (3.1415/2);
	pid_controller_.reset(kp_,0,kd_,0);
	last_error_ = Vector3f();
}

void PidTorque::set_target(const QuaternionF &setQ)
{
	set_Q_ = setQ;
}


Vector3f PidTorque::get_torque(const QuaternionF &in_Q, const TimeSpan& dt)
{
	Vector3f torq;
	Vector3f Zset = set_Q_.rotate(Vector3f(0.0, 0.0, 1.0));
	Vector3f Zin = (~in_Q).rotate(Vector3f(0.0, 0.0, 1.0));
	QuaternionF Qtorq = QuaternionF::fromVectors(Zin, Zset);
	Vector3f error = Qtorq.axis().normalize() * Qtorq.angle() * -1.0;

	last_error_ = error;

	torq = pid_controller_.get_pid(error,dt.seconds_float());

	return torq;
}
