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

#ifndef IMUFILTER_H_
#define IMUFILTER_H_

#include "axesdata.h"
#include "matrix.h"
#include "firfilt.h"
#include "pidcontroller.h"

/*
 * Implements attitude estimation, by filtering the data read from the IMU.
 * Gravity vector is used as an anchor (reference) for error correction.
 * On every step an error is calculated, which is the cross product of the initial
 * gravity vector and the estimated gravity vector, calculated from the estimated
 * attitude quaternion from the previous iteration.
 */

static const QuaternionD s_IdentityQ = QuaternionD(1, 0, 0, 0);

class ImuFilter
{
public:
	ImuFilter()
	{
		Reset();
	}
	~ImuFilter() {}
	void Reset()
	{
		m_Q = QuaternionD(1,0,0,0);
		SetEarthG(Vector3d(1,0,0));
	}

	void SetEarthG(const Vector3d& earthG)
	{
		m_earthG = earthG;
	}

	const QuaternionD& Update(
			Vector3d gyroDataDps,
			Vector3d accData,
			Vector3d magData,
			double dT)
	{
		accData = accData.normalize();
		magData = magData.normalize();

		m_LastGyroData = gyroDataDps;
		m_LastAccData = accData;
		m_LastMagData = magData;

		Vector3d omega = gyroDataDps - m_Vg;
		QuaternionD deltaQ = QuaternionD::fromAngularVelocity(omega * M_PI / 180.0f, dT);

		m_Q = m_Q * deltaQ; // m_Q * deltaQ * ~m_Q * m_Q;
		m_Q.normalize();

		Vector3d accEst = (~m_Q).rotate(m_earthG);
		QuaternionD q = QuaternionD::fromVectors(accEst, accData);

		m_Vg = QuaternionD::angularVelocity(s_IdentityQ, q, 0.75);  // The last parameter (dT) controls how fast to do the compensation
		m_Vg = m_Vg * 180.0f / M_PI;
		return m_Q;
	}

	const QuaternionD& GetAttitude() const { return m_Q; }
	const Vector3d& GetGyroData() const { return m_LastGyroData; }
	const Vector3d& GetAccelData() const { return m_LastAccData; }
	const Vector3d& GetMagData() const { return m_LastMagData; }
	const Vector3d& GetVg() const {return m_Vg;}

private:
	Vector3d m_earthG;
	Vector3d m_LastGyroData;
	Vector3d m_LastAccData;
	Vector3d m_LastMagData;
	Vector3d m_Vg;
	QuaternionD m_Q;
};

#endif /* IMUFILTER_H_ */
