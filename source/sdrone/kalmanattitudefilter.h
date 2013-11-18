/*
 * kalmanattitudefilter.h
 *
 *  Created on: Nov 9, 2013
 *      Author: svassilev
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
	int Start(CommandArgs*);
	void Stop(int flags);
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
	int m_RefCnt;
};

#endif /* KALMANATTITUDEFILTER_H */
