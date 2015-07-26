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

#if 0
	Vector3f Xset = set_Q_.rotate(Vector3f(1.0, 0.0, 0.0));
	Vector3f Xin = (~in_Q).rotate(Vector3f(1.0, 0.0, 0.0));
	QuaternionF QtorqX = QuaternionF::fromVectors(Xin, Xset);
	Vector3f errorX = QtorqX.axis().normalize() * QtorqX.angle() * -1.0;
#endif

	// undef the code below to test Z compensation
#if 0
	// targetQ = attitudeQ * errQ; ==> (~attitudeQ) * attitudeQ * errQ = (~attitudeQ) * targetQ;
	// ==> errQ = (~attitudeQ) * targetQ;
	Qtorq = (~in_Q) * set_Q_;
	Vector3f error_q = Qtorq.axis().normalize();
	float angle_rad = Qtorq.angle();
	if (fabs(angle_rad) > M_PI) {
		if (angle_rad > 0) {
			angle_rad -= 2.0 * M_PI;
		} else {
			angle_rad += 2.0 * M_PI;
		}
	}
	error_q = error_q * angle_rad;
	error = error_q;
#endif

	torq = pid_controller_.get_pid(error,dt.seconds_float());

	last_error_ = error;

	return torq;
}
