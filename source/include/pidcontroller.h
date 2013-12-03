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

template <int M>
class PidController
{
public:

	typedef MatrixMN<double, M, 1> PidVector;

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
		m_Err.clear();
		m_IntegralErr.clear();
		m_LastDErr.clear();
		m_LastInput.clear();
		m_Filter = (fCutoff > 0.0) ? (1 / (2 * M_PI * fCutoff)) : 0;
		m_AfterReset = true;
	}

	PidVector GetD(const PidVector& err, double dt)
	{
		PidVector derivative;
		double filter = dt / (m_Filter + dt);
		if (!m_AfterReset) {
			derivative = (err - m_LastInput) / dt;
		} else {
			m_AfterReset = false;
		}
		derivative = m_LastDErr + (derivative - m_LastDErr) * filter;
		m_LastInput = err;
		m_LastDErr = derivative;
		return derivative * m_Kd;
	}

	PidVector GetP(const PidVector& err)
	{
		return err * m_Kp;
	}

	PidVector GetI(
			const PidVector& err,
			double dt)
	{
		return _GetI(err,dt,0);
	}


	PidVector GetI(
			const PidVector& err,
			double dt,
			const PidVector& limit)
	{
		return _GetI(err,dt,&limit);
	}

	PidVector GetI(
			const PidVector& err,
			double dt,
			double leakRate) // 0..1
	{
		return _GetI(err,dt,leakRate,0);
	}

	PidVector GetI(
			const PidVector& err,
			double dt,
			double leakRate, // 0..1
			const PidVector& limit)
	{
		m_IntegralErr = m_IntegralErr - m_IntegralErr*leakRate;
		return _GetI(err,dt,&limit);
	}

	PidVector GetPid(const PidVector& err, double dt)
	{
		return GetP(err) + GetD(err, dt) + GetI(err,dt);
	}

	PidVector GetPid(
			const PidVector& err,
			double dt,
			double leakRate)
	{
		return GetP(err) + GetD(err, dt) + GetI(err,dt,leakRate);
	}

	const PidVector& IntegralError() const { return m_IntegralErr; }

private:
	PidVector _GetI(
			const PidVector& err,
			double dt,
			const PidVector* limit)
	{
		m_IntegralErr = m_IntegralErr + err * dt;
		LimitVector(&m_IntegralErr,limit);
		return m_IntegralErr * m_Ki;
	}

	static void LimitVector(PidVector* v, const PidVector* limit)
	{
		if (!!limit) {
			for (size_t i = 0; i < 3; i++) {
				v->at(i,0) = fmin(v->at(i,0),limit->at(i,0));
				v->at(i,0) = fmax(v->at(i,0),-limit->at(i,0));
			}
		}
	}

private:
	PidVector m_Err;
	PidVector m_IntegralErr;   // accumulated integral error
	PidVector m_LastDErr; // last derivative err
	PidVector m_LastInput;
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



typedef PidController<3> PidController3d;
typedef PidController<4> PidController4d;

#endif /* PIDCONTROLLER_H_ */
