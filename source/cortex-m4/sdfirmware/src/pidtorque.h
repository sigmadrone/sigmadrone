
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

public:
	QuaternionF last_twist_;
	QuaternionF set_Q_;
	PidController3f pid_controller_;
	PidController3f pid_controller_yaw_;
};

#endif

