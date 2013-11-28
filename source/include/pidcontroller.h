/*
 * pidcontroller.h
 *
 * Implements Proportional Integral Derivative controller logic
 *
 *  Created on: May 27, 2013
 *      Author: svassilev
 */

#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_

#include "matrix.h"

template <typename T>
class PidController
{
public:

	PidController(
			double kp,
			double ki,
			double kd,
			double fCutoff = -1)
	{
		Reset(kp,ki,kd,fCutoff);
	}
	void Reset(
			double kp,
			double ki,
			double kd,
			double fCutoff = -1)
	{
		m_Kp = kp;
		m_Ki = ki;
		m_Kd = kd;
		m_Err = T();
		m_IntegralErr = T();
		m_LastDErr = T();
		m_LastInput = T();
		m_Filter = (fCutoff > 0.0) ? (1 / (2 * M_PI * fCutoff)) : 0;
		m_AfterReset = true;
	}

	T GetD(const T& err, double dt)
	{
		T derivative = T();
		double filter = dt / (m_Filter + dt);
		if (!m_AfterReset) {
			derivative = (err - m_LastInput) / dt;
		} else {
			m_AfterReset = false;
		}
		derivative = m_LastDErr + (derivative - m_LastDErr) * filter;
		derivative = derivative * m_Kd;
		m_LastInput = err;
		m_LastDErr = derivative;
		return derivative;
	}

	T GetP(const T& err)
	{
		return err * m_Kp;
	}

	T GetI(
			const T& err,
			double dt)
	{
		m_IntegralErr = m_IntegralErr + err * dt;
		// TODO: must limit the max error
		return m_IntegralErr * m_Ki;
	}

	T GetI(
			const T& err,
			double dt,
			double leakRate) // 0..1
	{
		m_IntegralErr = m_IntegralErr - m_IntegralErr*leakRate;
		return GetI(err,dt);
	}

	T GetPid(const T& err, double dt)
	{
		return GetP(err) + GetD(err, dt) + GetI(err,dt);
	}

	T GetPid(
			const T& err,
			double dt,
			double leakRate)
	{
		return GetP(err) + GetD(err, dt) + GetI(err,dt,leakRate);
	}

	const T& IntegralError() const { return m_IntegralErr; }

private:
	T m_Err;
	T m_IntegralErr;   // accumulated integral error
	T m_LastDErr; // last derivative err
	T m_LastInput;
	double m_Kp; /*proportional gain*/
	double m_Ki; /*integral gain*/
	double m_Kd; /*derivative gain*/

	//
	// Low pass derivative filter, filtering out sudden jumps in the derivative
	// err. Calculated as 1 / (2 * PI * Fc)
	//
	double m_Filter;

	bool m_AfterReset;
};

typedef PidController<Vector3d> PidController3d;
typedef PidController<Vector4d> PidController4d;

#endif /* PIDCONTROLLER_H_ */
