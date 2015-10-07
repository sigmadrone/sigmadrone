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

#ifndef KALMANATTITUDEFILTER_H
#define KALMANATTITUDEFILTER_H

#include "commoninc.h"

class KalmanAttitudeFilter : public IPlugin{
public:
	KalmanAttitudeFilter();
	int AttachToChain(void*,SdPluginAttachFunc attachPlugin);

	int AddRef();
	int Release();
	int ExecuteCommand(SdCommandParams* params);
	const char* GetName();
	SdDeviceId GetDeviceId();
	const char* GetVersion();
	const char* GetDlFileName();

	/*
	 * Takes into account the IMU readings and comes with an estimate
	 * of the current attite
	 */
	int IoCallback(SdIoPacket* ioPacket);

	int IoDispatchThread() { assert(false); return EINVAL; }

	struct KalmanState
	{
		KalmanState();
		~KalmanState();
		Vector3d /*gyro estimate*/ Update(
				const Vector3d& xkEst,
				const Vector3d& zk);

		const Vector3d& KalmanGain() { return m_Kk; }
		const Vector3d& EstimateCov() { return m_Pk; }
	private:
		Vector3d m_Qk; // process noise cov matrix
		Vector3d m_Rk; // measurement noise cov matrix
		Vector3d m_Pk; // predicted estimate covariance
		Vector3d m_Kk; // kalman gain
	};

private:
	static Vector3d Qt2Euler(const QuaternionD& q);
	virtual ~KalmanAttitudeFilter();

private:
	IPluginRuntime* m_Runtime;
	KalmanState m_State;
	QuaternionD m_AttQ;
	Vector3d m_EarthG;
	int m_RefCnt;
};

#endif /* KALMANATTITUDEFILTER_H */
