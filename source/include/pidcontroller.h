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

#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_

#include "d3math.h"

template <class FLOAT, int M>
class PidController
{
public:

	static constexpr FLOAT MAX_VALUE = std::numeric_limits<FLOAT>::infinity();

	typedef MatrixMN<FLOAT, M, 1> PidVector;

	PidController(
			FLOAT kp,
			FLOAT ki,
			FLOAT kd,
			FLOAT fCutoff = -1)
	{
		reset(kp,ki,kd,fCutoff);
	}
	void reset(
			FLOAT kp,
			FLOAT ki,
			FLOAT kd,
			FLOAT fCutoff = -1)
	{
		kp_ = kp;
		ki_ = ki;
		kd_ = kd;
		err_.clear();
		integral_err_.clear();
		last_der_err_.clear();
		last_input_.clear();
		filter_ = (fCutoff > 0.0) ? (1 / (2 * M_PI * fCutoff)) : 0;
	}

	PidVector get_d(const PidVector& err, FLOAT dt)
	{
		PidVector derivative = (err - last_input_) / dt;
		if (filter_ != 0.0) {
			FLOAT filter = dt / (filter_ + dt);
			derivative = last_der_err_ + (derivative - last_der_err_) * filter;
		}
		last_input_ = err;
		last_der_err_ = derivative;
		return derivative * kd_;
	}

	PidVector get_d(const PidVector& err, FLOAT dt, const PidVector& abs_limit)
	{
		return limit_vector(get_d(err, dt), &abs_limit);
	}

	PidVector get_p(const PidVector& err)
	{
		return err * kp_;
	}

	PidVector get_i(
			const PidVector& err,
			FLOAT dt)
	{
		return _get_i(err,dt,0);
	}


	PidVector get_i(
			const PidVector& err,
			FLOAT dt,
			const PidVector& limit)
	{
		return _get_i(err,dt,&limit);
	}

	PidVector get_i(
			const PidVector& err,
			FLOAT dt,
			FLOAT leakRate) // 0..1
	{
		return _get_i(err,dt,leakRate,0);
	}

	PidVector get_i(
			const PidVector& err,
			FLOAT dt,
			FLOAT leakRate, // 0..1
			const PidVector& limit)
	{
		integral_err_ = integral_err_ - integral_err_*leakRate;
		return _get_i(err,dt,&limit);
	}

	PidVector get_pid(const PidVector& err, FLOAT dt)
	{
		return get_p(err) + get_d(err, dt) + get_i(err,dt);
	}

	PidVector get_pid(const PidVector& err, FLOAT dt, const PidVector& abs_limit)
	{
		return limit_vector(get_pid(err, dt), &abs_limit);
	}

	PidVector get_pid(
				const PidVector& err,
				FLOAT dt,
				FLOAT leakRate,
				const PidVector& abs_limit)
	{
		return limit_vector(get_pid(err,dt,leakRate),&abs_limit);
	}

	PidVector get_pid(
			const PidVector& err,
			FLOAT dt,
			FLOAT leakRate)
	{
		return get_p(err) + get_d(err, dt) + get_i(err,dt,leakRate);
	}

	PidVector get_pd(const PidVector& err, FLOAT dt)
	{
		return get_p(err) + get_d(err, dt);
	}

	PidVector get_pd(const PidVector& err, FLOAT dt, const PidVector& abs_limit)
	{
		return limit_vector(get_pd(err,dt),&abs_limit);
	}

	const PidVector& integral_error() const { return integral_err_; }

	void set_kp(float kp) { kp_ = kp; }
	void set_ki(float ki) { ki_ = ki; }
	void set_kd(float kd) { kd_ = kd; }
	float get_kp() const { return kp_; }
	float get_ki() const { return ki_; }
	float get_kd() const { return kd_; }
	void set_kp_ki_kd(float kp, float ki, float kd) {
		set_kp(kp);
		set_ki(ki);
		set_kd(kd);
	}

private:
	PidVector _get_i(
			const PidVector& err,
			FLOAT dt,
			const PidVector* limit)
	{
		integral_err_ = limit_vector(integral_err_ + err * dt,limit);
		return integral_err_ * ki_;
	}

	static FLOAT min_value(FLOAT a, FLOAT b) {
		return a < b ? a : b;
	}
	static FLOAT max_value(FLOAT a, FLOAT b) {
		return a > b ? a : b;
	}

	static PidVector limit_vector(const PidVector& v, const PidVector* abs_limit)
	{
		PidVector limited_vec;
		if (!!abs_limit) {
			for (size_t i = 0; i < M; i++) {
				limited_vec.at(i,0) = min_value(v.at(i,0),abs_limit->at(i,0));
				limited_vec.at(i,0) = max_value(limited_vec.at(i,0),-abs_limit->at(i,0));
			}
		}
		return limited_vec;
	}

private:
	PidVector err_;
	PidVector integral_err_;   // accumulated integral error
	PidVector last_der_err_; // last derivative err
	PidVector last_input_;
	FLOAT kp_; /*proportional gain*/
	FLOAT ki_; /*integral gain*/
	FLOAT kd_; /*derivative gain*/

	//
	// Low pass derivative filter, filtering out sudden jumps in the derivative
	// err. Calculated as 1 / (2 * PI * Fc)
	//
	FLOAT filter_;
};



typedef PidController<float, 3> PidController3f;
typedef PidController<float, 4> PidController4f;

typedef PidController<double, 3> PidController3d;
typedef PidController<double, 4> PidController4d;

#endif /* PIDCONTROLLER_H_ */
