
#include <sys/time.h>
#include "matrix.h"
#include "pidcontroller.h"

#ifndef _PIDTORQUE_H_
#define _PIDTORQUE_H_

class PidTorque
{
public:
	PidTorque();
	~PidTorque();

	void SetTarget(const QuaternionD &setQ);
	Vector3d GetTorque(const QuaternionD &inQ, const Vector3d& Omega, double dT);
	void Reset(double Kp, double Ki, double Kd);

public:
	QuaternionD m_setQ;
	QuaternionD m_oldQ;
	Vector3d m_lastError;
	Vector3d m_integralError;
	timespec m_lastTime;
	double m_Kp, m_Ki, m_Kd;
	PidController3d m_pidController;
};




#endif

