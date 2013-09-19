/*
 * quadrotorpilot.h
 *
 *  Created on: Aug 07, 2013
 *      Author: Martin Stoilov
 */

#ifndef _PIDPILOT_H_
#define _PIDPILOT_H_

#include "ipilot.h"

class ServoDevice;

class PidPilot : public IPilot
{
public:
	PidPilot();
	virtual ~PidPilot();
	virtual int Reset(const SdDroneConfig*, ServoDevice*);
	virtual int Update(
			const QuaternionD& currentAttitude,
			const QuaternionD& desiredAttitude,
			const Vector3d& gyroDps,
			double desiredThrust);

	virtual void SetMinRev(double minRev);
	virtual void SetMaxRev(double maxRev);
	virtual const Vector4d* GetMotors() const;
	virtual double GetMinRev() const { return m_MinRev; }
	virtual double GetMaxRev() const { return m_MaxRev; }
	virtual const double GetErrorAngle() const { return m_ErrorAngle; }
	virtual const Vector3d& GetErrorAxis() const { return m_ErrorAxis; }
	virtual const QuaternionD& GetErrorQt() const { return m_PrevErrQ; }
	virtual const Vector3d& GetTorqueCorrection() const { return m_TorqueCorrection; }
	virtual const Vector3d& GetOmega() const { return m_Omega; }
	virtual const Vector3d& GetAngularAccel() const { return m_AngAccel; }

private:
	void SetAndScaleMotors(
			double m1,
			double m2,
			double m3,
			double m4
			);

private:
	double m_MinRev;
	double m_MaxRev;
	int m_Counter;
	double m_Dt;
	double m_Step;
	double m_ErrorAngle;
	Vector4d m_Motors;
	Vector3d m_ErrorAxis;
	ServoDevice* m_Servo;
	SdQuadRotorConfig m_Config;
	QuaternionD m_TargetQ;
	QuaternionD m_PrevErrQ;
	QuaternionD m_RotZQ;
	PidController3d m_PidCtl;
	FirFilter<double,8,4> m_MotorFilt;
	Vector3d m_TorqueCorrection;
	Vector3d m_Omega;
	Vector3d m_AngAccel;
};


#endif /* QUADROTORPILOT_H_ */
