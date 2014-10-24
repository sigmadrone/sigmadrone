
#include <sys/time.h>
#include "matrix.h"

#ifndef _PIDTORQUE_H_
#define _PIDTORQUE_H_

class pidtorque
{
public:
	pidtorque();
	~pidtorque();

	void set_target(const QuaternionD &setQ);
	Vector3d get_xy_torque(const QuaternionD &inQ, const Vector3d& Omega, double dT);
	Vector3d get_z_torque(const QuaternionD &inQ, const Vector3d& Omega, double dT);
	void reset(double Kp, double Ki, double Kd);

public:
	QuaternionD target_q_;
	QuaternionD old_q_;
	Vector3d error_;
	Vector3d integral_error_;
	timespec previous_time_;
	double Kp_, Ki_, Kd_;
};




#endif

