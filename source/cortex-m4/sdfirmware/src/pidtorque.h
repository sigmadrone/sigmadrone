
#include "units.h"
#include "matrix.h"
#include "pidcontroller.h"

#ifndef _PIDTORQUE_H_
#define _PIDTORQUE_H_

class PidTorque
{
public:
	PidTorque();
	~PidTorque();

	void set_target(const QuaternionF &set_q);
	Vector3f get_torque(const QuaternionF &in_q, const TimeSpan& dt);
	void reset(float kp, float ki, float kd);

public:
	QuaternionF set_Q_;
	Vector3f last_error_;
	float kp_, ki_, kd_;
	PidController3f pid_controller_;
	PidController3f pid_controller_z_;
};

#endif

