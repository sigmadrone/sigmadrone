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

#ifndef ATTITUDEFILTER_H_
#define ATTITUDEFILTER_H_

#include "axesdata.h"
#include "d3math.h"
#include "firfilt.h"

typedef Quaternion<double> QuaternionF;

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
		m_GyroBias = Vector3f(gyroBias.x,gyroBias.y,gyroBias.z);
		m_AccBias = Vector3f(accBias.x,accBias.y,accBias.z);
	}

	const QuaternionF& Update(
			const short3d_t& gyroDataRaw,
			const short3d_t& accDataRaw,
			const short3d_t& magDataRaw)
	{
		Vector3f gyroData(gyroDataRaw.x,gyroDataRaw.y,gyroDataRaw.z);
		Vector3f accData(accDataRaw.x,accDataRaw.y,accDataRaw.z);
		Vector3f magData(magDataRaw.x,magDataRaw.y,magDataRaw.z);

		gyroData = gyroData - m_GyroBias;
		gyroData = gyroData * (double)m_MaxDps / (double)m_MaxAdc;
		gyroData = gyroData * M_PI / 180;

		accData = accData - m_AccBias;
		accData = accData.normalize();

		if (0 == m_OrigAcc.length())
		{
			double zVector[3] = {0.0,0.0,1.0};
			m_OrigAcc = accData;
			m_OrigMag = Vector3f(zVector);
			m_MagQ = QuaternionF::fromVectors(magData,m_OrigMag);
			m_GyroQ = QuaternionF(1,0,0,0);
			return m_GyroQ;
		}

		//m_GyroLpf.DoFilter(gyroData.at(0),gyroData.at(1),gyroData.at(2));


		/*calculate the acc estimate*/
		Vector3f accEst = m_GyroQ.rotate(m_OrigAcc);
		/*calculate the acc error*/
		Vector3f err = Vector3f::cross(accData,accEst);

		Vector3f derivativeErr;

		/*implement the PID controller logic to calculate the resultant error*/
		m_IntegralError = m_IntegralError + err * m_Dt;
		derivativeErr = (err - m_Err) / m_Dt;
		m_Err = err * m_Kp;
		m_Err = m_Err + m_IntegralError * m_Ki;
		m_Err = m_Err + derivativeErr * m_Kd;

		/* compensate the gyro readings with the calculated error */
		gyroData = gyroData - m_Err;

		/* calculate quaternion derivative */
		QuaternionF qDot(gyroData, 0);
		qDot = (qDot * m_GyroQ) * 0.5;

		/* integrate the derivative to yield quaternion*/
		m_GyroQ += qDot * m_Dt;
		m_GyroQ = m_GyroQ.normalize();

		++m_Counter;
		return m_GyroQ;
	}

	const QuaternionF& GetAttitude() const { return m_GyroQ; }


private:
	Vector3f m_Err;
	Vector3f m_IntegralError;
	Vector3f m_OrigAcc;
	Vector3f m_OrigMag;
	Vector3f m_GyroBias;
	Vector3f m_AccBias;
	QuaternionF m_MagQ;
	QuaternionF m_GyroQ;
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
