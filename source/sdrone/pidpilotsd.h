/*
 *  Sigmadrone
 *  Copyright (c) 2013-2015 The Sigmadrone Developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Martin Stoilov <martin@sigmadrone.org>
 *  Svetoslav Vassilev <svassilev@sigmadrone.org>
 */

#ifndef PIDPILOT_H_
#define PIDPILOT_H_

#include "commoninc.h"
#include "pidtorque.h"
#include "pidcontroller.h"
#include "firfilt.h"

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

	int IoDispatchThread();

private:
	virtual ~PidPilot();
	int Start(const SdDroneConfig* config);
	void Stop(bool detach);
	void SetMinRev(double minRev);
	void SetMaxRev(double maxRev);
	const Vector4d* GetMotors() const;
	double GetMinRev() const { return m_minThrust; }
	double GetMaxRev() const { return m_maxThrust; }

	int UpdateState(
			SdIoPacket*
			);
	int IssueCommandToServo(const Vector4d& motors);

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
	IPluginRuntime* m_runtime;
	Vector4d m_motors;
	SdQuadRotorConfig m_config;
	double m_minThrust;
	double m_maxThrust;
	double m_targetThrust;
	int m_refCnt;

	PidTorque m_pid;
	Vector3d m_torqueCorrection;
	Vector3d m_M0, m_M1, m_M2, m_M3;
	deltatime m_dt;
};


#endif /* PIDPILOT_H_ */
