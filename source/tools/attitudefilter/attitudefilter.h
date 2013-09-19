/*
 * attitudefilter.h
 *
 *  Created on: Mar 10, 2013
 *      Author: svassilev
 */

#ifndef ATTITUDEFILTER_H_
#define ATTITUDEFILTER_H_

#include "axesdata.h"
#include "matrix.h"
#include "firfilt.h"

typedef Quaternion<double> QuaternionD;

/*
 * Implements attitude estimation, by filtering the data read from the IMU.
 * Gravity vector is used as an anchor (reference) for error correction.
 * On every step an error is calculated, which is the cross product of the initial
 * gravity vector and the estimated gravity vector, calculated from the estimated
 * attitude quaternion from the previous iteration. The error is fed into PID
 * controller, whose output is used to compensate the gyro readings. The compensated
 * gyro data is then converted into quaternion representing the current attitude.
 */
class AttitudeFilter
{
public:
	AttitudeFilter(
			double fsHz,
			double maxDps,
			double maxAdc,
			double kp=1,
			double ki=0,
			double kd=0) :
				m_Counter(0)
	{
		SetParams(fsHz,maxDps,maxAdc,kp,ki,kd);
	}
	~AttitudeFilter()
	{
		m_Counter = 0;
	}
	void SetParams(
			double fsHz,
			double maxDps,
			double maxAdc,
			double kp=1,
			double ki=0,
			double kd=0)
	{
		m_Dt = 1/fsHz;
		m_Kp = kp;
		m_Ki = ki;
		m_Kd = kd;
		m_MaxDps = maxDps;
		m_MaxAdc = maxAdc;
	}

	void SetBias(
			const int3d_t& gyroBias,
			const int3d_t& accBias)
	{
		m_GyroBias = Vector3d(gyroBias.x,gyroBias.y,gyroBias.z);
		m_AccBias = Vector3d(accBias.x,accBias.y,accBias.z);
	}

	const QuaternionD& Update(
			const short3d_t& gyroDataRaw,
			const short3d_t& accDataRaw,
			const short3d_t& magDataRaw)
	{
		Vector3d gyroData(gyroDataRaw.x,gyroDataRaw.y,gyroDataRaw.z);
		Vector3d accData(accDataRaw.x,accDataRaw.y,accDataRaw.z);
		Vector3d magData(magDataRaw.x,magDataRaw.y,magDataRaw.z);

		gyroData = gyroData - m_GyroBias;
		gyroData = gyroData * (double)m_MaxDps / (double)m_MaxAdc;
		gyroData = gyroData * M_PI / 180;

		accData = accData - m_AccBias;
		accData = accData.normalize();

		if (0 == m_OrigAcc.length())
		{
			double zVector[3] = {0.0,0.0,1.0};
			m_OrigAcc = accData;
			m_OrigMag = Vector3d(zVector);
			m_MagQ = QuaternionD::fromVectors(magData,m_OrigMag);
			m_GyroQ = QuaternionD(1,0,0,0);
			return m_GyroQ;
		}

		//m_GyroLpf.DoFilter(gyroData.at(0),gyroData.at(1),gyroData.at(2));


		/*calculate the acc estimate*/
		Vector3d accEst = m_GyroQ.rotate(m_OrigAcc);
		/*calculate the acc error*/
		Vector3d err = Vector3d::cross(accData,accEst);

		Vector3d derivativeErr;

		/*implement the PID controller logic to calculate the resultant error*/
		m_IntegralError = m_IntegralError + err * m_Dt;
		derivativeErr = (err - m_Err) / m_Dt;
		m_Err = err * m_Kp;
		m_Err = m_Err + m_IntegralError * m_Ki;
		m_Err = m_Err + derivativeErr * m_Kd;

		/* compensate the gyro readings with the calculated error */
		gyroData = gyroData - m_Err;

		/* calculate quaternion derivative */
		QuaternionD qDot(0,gyroData);
		qDot = (qDot * m_GyroQ) * 0.5;

		/* integrate the derivative to yield quaternion*/
		m_GyroQ += qDot * m_Dt;
		m_GyroQ = m_GyroQ.normalize();

		++m_Counter;
		return m_GyroQ;
	}

	const QuaternionD& GetAttitude() const { return m_GyroQ; }


private:
	Vector3d m_Err;
	Vector3d m_IntegralError;
	Vector3d m_OrigAcc;
	Vector3d m_OrigMag;
	Vector3d m_GyroBias;
	Vector3d m_AccBias;
	QuaternionD m_MagQ;
	QuaternionD m_GyroQ;
	LpPreFilter3d m_GyroLpf;
	LpPreFilter3d m_AccLpf;
	double m_Kp; /*proportional gain*/
	double m_Ki; /*integral gain*/
	double m_Kd; /*derivative gain*/
	double m_Dt; /*1/fs*/
	double m_MaxDps;
	double m_MaxAdc;
	uint64_t m_Counter;
};

#endif /* ATTITUDEFILTER_H_ */
