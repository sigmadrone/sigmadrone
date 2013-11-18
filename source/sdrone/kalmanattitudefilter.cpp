/*
 * kalmanattitudefilter.cpp
 *
 *  Created on: Nov 9, 2013
 *      Author: svassilev
 */

#include "kalmanattitudefilter.h"

KalmanAttitudeFilter::KalmanAttitudeFilter() :
	m_State(),
	m_AttQ(1,0,0,0)
{
	m_RefCnt = 1;
	m_Runtime = 0;
}

KalmanAttitudeFilter::~KalmanAttitudeFilter()
{
	assert(0 == m_RefCnt);
}

int KalmanAttitudeFilter::AttachToChain(
		void* droneContext,
		SdPluginAttachFunc attachFunc)
{
	int err = attachFunc(
			droneContext,
			this,
			SD_ALTITUDE_GROUP_FILTER,
			0,
			0,
			&m_Runtime);
	return err;
}

int KalmanAttitudeFilter::Start(CommandArgs* cmdArgs)
{
	m_Runtime->SetIoFilters(
			SD_DEVICEID_TO_FLAG(SD_DEVICEID_IMU),
			SD_IOCODE_TO_FLAG(SD_IOCODE_RECEIVE));
	return 0;
}

int KalmanAttitudeFilter::AddRef()
{
	return __sync_fetch_and_add(&m_RefCnt,1);
}

int KalmanAttitudeFilter::Release()
{
	int refCnt = __sync_sub_and_fetch(&m_RefCnt,1);
	if (0 == refCnt) {
		delete this;
	}
	return refCnt;
}

void KalmanAttitudeFilter::Stop(int flags)
{
	if (!!(flags&FLAG_STOP_AND_DETACH)) {
		m_Runtime->DetachPlugin();
	}
}

const char* KalmanAttitudeFilter::GetName()
{
	return SD_PLUGIN_IMU_FILTER;
}

SdDeviceId KalmanAttitudeFilter::GetDeviceId()
{
	return SD_DEVICEID_FILTER;
}

const char* KalmanAttitudeFilter::GetVersion()
{
	return SD_PLUGIN_VERSION;
}

extern char **g_Argv;
const char* KalmanAttitudeFilter::GetDlFileName()
{
	return g_Argv[0];
}


int KalmanAttitudeFilter::IoCallback(SdIoPacket* ioPacket)
{
	assert(SD_DEVICEID_IMU == ioPacket->DeviceId());
	assert(SD_IOCODE_RECEIVE == ioPacket->IoCode());

	Vector3d earthG(0,0,-1);
	SdIoData earthGData = ioPacket->GetAttribute(SDIO_ATTR_EARTH_G);
	if (earthGData.dataType == SdIoData::TYPE_VECTOR3D) {
		earthG = *earthGData.asVector3d;
	}

	Vector3d accel = ioPacket->AccelData();
	Vector3d accelEst = (~m_AttQ).rotate(earthG);
	QuaternionD qdiff = QuaternionD::fromVectors(
			accel,accelEst);
	Vector3d gyroFromAccel = Qt2Euler(qdiff) / ioPacket->DeltaTime() / 3;

	/*
	 * Obtain the kalman estimate for the actual angular speed
	 */
	Vector3d gyro = ioPacket->GyroData() * M_PI / 180.0;
	Vector3d gyroEst = m_State.Update(gyro, gyroFromAccel);

	/*
	 * Integrate the angular speed to obtain the quaternion
	 */
	QuaternionD dq(0,gyroEst.at(0,0),gyroEst.at(1,0),gyroEst.at(2,0));
	dq = m_AttQ * dq;
	dq = dq * 0.5 * ioPacket->DeltaTime();
	m_AttQ = m_AttQ + dq;
	m_AttQ.z = 0;
	m_AttQ = m_AttQ.normalize();

	gyroEst = RAD2DEG(gyroEst);
	ioPacket->SetAttribute(SDIO_ATTR_ATTITUDE_Q,SdIoData(&m_AttQ));
	ioPacket->SetAttribute(SDIO_ATTR_CORR_VELOCITY,SdIoData(&gyroEst));
	return SD_ESUCCESS;
}

Vector3d KalmanAttitudeFilter::Qt2Euler(const QuaternionD& q)
{
	Vector3d angle(0,0,0);
	static const double eps = 0.0000001;

	double nom = 2*(q.y*q.z-q.w*q.x);
	double denom = 2*q.w*q.w-1+2*q.z*q.z;
	if (fabs(nom) > eps || fabs(denom) > eps) {
	    angle.at(0,0) = atan2(-nom,denom);
	}

	angle.at(1,0) = asin(2*(q.x*q.z+q.w*q.y));

	nom = -2*(q.x*q.y-q.w*q.z);
	denom = 2*q.w*q.w-1+2*q.x*q.x;
	if (fabs(nom) > eps || fabs(denom) > eps) {
	    angle.at(2,0) = atan2(nom,denom);
	}

	return angle;
}

KalmanAttitudeFilter::KalmanState::KalmanState() :
		m_Pk(0,0,0),
		m_Kk(0,0,0)
{
	/*
	 * Init the process noise - in this case this is the variance of the
	 * gyro measurement
	 */
	m_Qk = Vector3d(0.3718,0.1965,0.3413) * 0.00001f;

	/*
	 * Init the measurement noise - the variance in the accelerometer
	 * measurements
	 */
	m_Rk = Vector3d(0.1110,0.1550,0.1000)*0.5;
}

KalmanAttitudeFilter::KalmanState::~KalmanState() {}

Vector3d /*gyro estimate*/ KalmanAttitudeFilter::KalmanState::Update(
		const Vector3d& XkEst,
		const Vector3d& Zk)
{
	/*
	 * Predict state and covariance:
	 * * X[k|k-1] = Fk * X[k-1|k-1] + Bk*U[k-1]
	 * P[k|k-1] = Fk * P[k-1|k-1] * Fk' + Qk
	 *
	 * Fk is [1] and Uk is [0], hence Xk=XkEst.
	 */
	Vector3d Pk = m_Pk + m_Qk;

	/*
	 * Update state.
	 */

	/*
	 * Innovation/residual covariance
	 * Sk = P[k|k-1] + Rk
	 */
	Vector3d Sk = Pk + m_Rk;

	/*
	 * Kalman gain
	 * Kk = P[k|k-1] / Sk
	 */
	m_Kk.at(0,0) = Pk.at(0,0) / Sk.at(0,0);
	m_Kk.at(1,0) = Pk.at(1,0) / Sk.at(1,0);
	m_Kk.at(2,0) = Pk.at(2,0) / Sk.at(2,0);

	/*
	 * Measurement/residual - innovation
	 */
	Vector3d Yk = Zk - XkEst;

	/*
	 * Updated/aposteriori state estimate
	 * X[k|k] = X[k|k-1] + Kk *Yk
	 */
	Yk.at(0,0) = Yk.at(0,0) * m_Kk.at(0,0);
	Yk.at(1,0) = Yk.at(1,0) * m_Kk.at(1,0);
	Yk.at(2,0) = Yk.at(2,0) * m_Kk.at(2,0);
	Vector3d Xk = XkEst;
	Xk = Xk + Yk;


	/*
	 * Updated/aposteriori estimate covariance
	 * P[k|k] = (I - Kk) * P[k|k-1]
	 */
	m_Pk.at(0,0) = (1.0-m_Kk.at(0,0)) * Pk.at(0,0);
	m_Pk.at(1,0) = (1.0-m_Kk.at(1,0)) * Pk.at(1,0);
	m_Pk.at(2,0) = (1.0-m_Kk.at(2,0)) * Pk.at(2,0);

	return Xk;
}
