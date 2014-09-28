/*
 * quadrotorpilot.h
 *
 *  Created on: May 18, 2013
 *      Author: svassilev
 */

#ifndef PIDPILOT_H_
#define PIDPILOT_H_

#include "commoninc.h"
#include "pidtorque.h"
#include "pidcontroller.h"
#include "firfilt.h"

class ServoDevice;

class PidPilot: public IPlugin
{
public:
	PidPilot();

	int AttachToChain(void*,SdPluginAttachFunc attachPlugin);

	int AddRef();
	int Release();
	int ExecuteCommand(SdCommandParams* commandArgs);
	const char* GetName();
	SdDeviceId GetDeviceId();
	const char* GetVersion();
	const char* GetDlFileName();

	/*
	 * Updates its state by taking estimated and desired attitude and
	 * desired thrust. The attitude is specified in earth coordinate system.
	 * As final step, sends command to the servo.
	 */
	int IoCallback(SdIoPacket* ioPacket);

	int IoDispatchThread() { assert(false); return EINVAL; }

private:
	virtual ~PidPilot();
	int Start(const SdDroneConfig* config);
	void Stop(bool detach);
	void SetMinRev(double minRev);
	void SetMaxRev(double maxRev);
	const Vector4d* GetMotors() const;
	double GetMinRev() const { return m_MinRev; }
	double GetMaxRev() const { return m_MaxRev; }
	const double GetErrorAngle() const { return m_ErrorAngle; }
	const Vector3d& GetErrorAxis() const { return m_ErrorAxis; }
	const Vector3d& GetOmega() const { return m_Omega; }
	const Vector3d& GetAngularAccel() const { return m_AngAccel; }

	int UpdateState(
			SdIoPacket*
			);
	int IssueCommandToServo();

	static double GetTorqueScale(
			double errAngle
			);
	static double CalcDesiredOmega(
			double errAngle
			);
	static Vector3d CalcDesiredOmega3d(
			const Vector3d&
			);
private:
	void SetAndScaleMotors(
			double w0,
			double w1,
			double w2,
			double w3
			);
	void CalcThrustFromErrAxis(
			const Vector3d& err,
			double targetThrust
			);
	double CalcTorqueCorrection(
			double torqueEst,
			double omega,
			double desiredOmega,
			double angAccel,
			int axis
			);

	static Vector3d FromQtToTorqueVector(const QuaternionD& q);

private:
	IPluginRuntime* m_Runtime;
	Vector4d m_Motors;
	Vector3d m_ErrorAxis;
	Vector3d m_ErrorAxisPid;
	Vector3d m_ErrorI;
	Vector3d m_ErrorD;
	Vector3d m_ErrorP;
	SdQuadRotorConfig m_Config;
	QuaternionD m_TargetQ;
	QuaternionD m_PrevQ;
	QuaternionD m_RotZQ;
	LpPreFilter3d m_GyroFilt;
	LpPreFilter3d m_ErrFilt;
	LpPreFilter3d m_TcFilt;
	double m_MinRev;
	double m_MaxRev;
	double m_targetThrust;
	int m_Counter;
	double m_ErrorAngle;
	Vector3d m_Omega;
	Vector3d m_AngAccel;
	Vector3d m_Step;
	int m_Skip;
	int m_RefCnt;


	PidTorque m_pid;
	Vector3d m_TorqueCorrection;
	Vector3d m_M0, m_M1, m_M2, m_M3;
};


#endif /* PIDPILOT_H_ */
