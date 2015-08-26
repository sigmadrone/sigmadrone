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

Vector3f PidTorque::get_torque(const QuaternionF &in_Q, const TimeSpan& dt)
{
	Vector3f torq;
	Vector3f Zset = set_Q_.rotate(Vector3f(0.0, 0.0, 1.0));
	Vector3f Zin = (~in_Q).rotate(Vector3f(0.0, 0.0, 1.0));
	QuaternionF Qtorq = QuaternionF::fromVectors(Zin, Zset);
	Vector3f error = Qtorq.axis().normalize() * Qtorq.angle() * -1.0;

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

#if 0
	QuaternionF in_q = QuaternionF::fromAxisRot(in_Q.axis(), 0.5 * in_Q.angle());
	Vector3f Xset = set_Q_.rotate(Vector3f(1.0, 0.0, 0.0));
	Vector3f Xin = (~in_q).rotate(Vector3f(1.0, 0.0, 0.0));
	QuaternionF Qtorq_z = QuaternionF::fromVectors(Xin, Xset);
	Vector3f error_z = Qtorq_z.axis().normalize() * Qtorq_z.angle() * -1.0;
	error_z.at(0,0) = error_z.at(1,0) = 0.0;
	torq.at(0) = torq.at(1) = 0;
	torq.at(2,0) = pid_controller_yaw_.get_pid(error_z, dt.seconds_float()).at(2,0);
	error.at(2,0) = error_z.at(2,0);
#endif

#if 1
	// targetQ = attitudeQ * errQ; ==> (~attitudeQ) * attitudeQ * errQ = (~attitudeQ) * targetQ;
	// ==> errQ = (~attitudeQ) * targetQ;
	Qtorq = (~in_Q) * set_Q_;
	QuaternionF swing;
	QuaternionF::decomposeTwistSwing(Qtorq, Vector3d(0,0,1), swing, twist_);
	Vector3f error_z = twist_.axis().normalize() * twist_.angle() * -1.0;
	error_z.at(0) = error_z.at(1) = 0.0;
	torq.at(2) = pid_controller_yaw_.get_pid(error_z, dt.seconds_float()).at(2);
	error.at(2) = error_z.at(2);

#endif

	return torq;
}
