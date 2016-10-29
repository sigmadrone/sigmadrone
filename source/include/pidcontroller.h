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

template <class T>
class PidController
{
public:
	PidController(float kp = 0, float ki = 0, float kd = 0, float fCutoff = -1) :
		integral_err_(0), last_derivative_err_(0), max_derivative_err_(0),
		min_derivative_err_(0), last_err_input_(0), kp_(kp), ki_(ki), kd_(kd),
		filter_(0)
	{
		set_derivative_filter(fCutoff);
	}

	T get_d(const T& err, float dt)
	{
		T derivative_err = (err - last_err_input_) / dt;
		last_err_input_ = err;

		return derivative_err * kd_;
	}

	T get_d_derivative(const T& err, float dt)
	{
		return get_d_common(err, dt, &PidController<T>::filter_derivative_lpf);
	}

	T get_d_median(const T& err, float dt)
	{
		return get_d_common(err, dt, &PidController<T>::find_derivative_median);
	}

	T get_d_fused(const T& err, float dt)
	{
		return (err > 0.05 || err < -0.05) ? get_d(err, dt) : get_d_median(err, dt);
	}

	T get_p(const T& err)
	{
		return err * kp_;
	}

	T get_i(const T& err, float dt)
	{
		return get_i(err, dt, 0);
	}

	T get_i(const T& err, float dt, float leakRate /* 0..1 */)
	{
		integral_err_ *= (1.0f - leakRate);
		integral_err_ += err * dt;
		return integral_err_ * ki_;
	}

	T get_pid(const T& err, float dt)
	{
		return get_p(err) + get_d(err, dt) + get_i(err, dt);
	}

	T get_pi_dmedian(const T& err, float dt)
	{
		return get_p(err) + get_d_median(err, dt) + get_i(err, dt);
	}

	T get_pi_dfused(const T& err, float dt)
	{
		return get_p(err) + get_d_fused(err, dt) + get_i(err, dt);
	}

	T get_pid(const T& err, float dt, float leakRate)
	{
		return get_p(err) + get_d(err, dt) + get_i(err, dt, leakRate);
	}

	T get_pi_dmedian(const T& err, float dt, float leakRate)
	{
		return get_p(err) + get_d_median(err, dt) + get_i(err, dt, leakRate);
	}

	T get_pi_dfused(const T& err, float dt, float leakRate)
	{
		return get_p(err) + get_d_fused(err, dt) + get_i(err, dt, leakRate);
	}

	T get_pd(const T& err, float dt)
	{
		return get_p(err) + get_d(err, dt);
	}

	T get_p_dmedian(const T& err, float dt)
	{
		return get_p(err) + get_d_median(err, dt);
	}

	T get_p_dfused(const T& err, float dt)
	{
		return get_p(err) + get_d_fused(err, dt);
	}

	const T& get_integral_error() const
	{
		return integral_err_;
	}

	void set_integral_error(const T& err)
	{
		integral_err_ = err;
	}

	void set_kp(float kp) { kp_ = kp; }
	void set_ki(float ki) { ki_ = ki; }
	void set_kd(float kd) { kd_ = kd; }
	float get_kp() const { return kp_; }
	float get_ki() const { return ki_; }
	float get_kd() const { return kd_; }
	void set_kp_ki_kd(float kp, float ki, float kd)
	{
		set_kp(kp);
		set_ki(ki);
		set_kd(kd);
	}
	void set_derivative_filter(float fCutoff)
	{
		filter_ = (fCutoff > 0.0) ? (1 / (2 * M_PI * fCutoff)) : 0;
	}

private:
	typedef T (PidController<T>::*DerivativeFilterFunc)(const T& err, float dt);

	T get_d_common(const T& err, float dt, DerivativeFilterFunc filter_func)
	{
		T derivative_err = (err - last_err_input_) / dt;
		T filtered_derivative_err = (this->*filter_func)(derivative_err, dt);

		update_derivative_min_max(derivative_err);

		last_derivative_err_ = derivative_err;
		last_err_input_ = err;

		return filtered_derivative_err * kd_;
	}

	T filter_derivative_lpf(const T& derivative_err, float dt)
	{
		float alpha = dt / (filter_ + dt);
		return derivative_err * alpha +  last_derivative_err_ * (1 - alpha);
	}

	T find_derivative_median(const T& derivative_err, float dt)
	{
		(void)dt;
		if (derivative_err > max_derivative_err_) {
			return max_derivative_err_;
		} else if (derivative_err < min_derivative_err_) {
			return min_derivative_err_;
		}
		return derivative_err;
	}

	void update_derivative_min_max(const T& derivative_err)
	{
		if (derivative_err > last_derivative_err_) {
			min_derivative_err_ = last_derivative_err_;
			max_derivative_err_ = derivative_err;
		} else {
			min_derivative_err_ = derivative_err;
			max_derivative_err_ = last_derivative_err_;
		}
	}

private:
	T integral_err_;        /* accumulated integral error */
	T last_derivative_err_; /* derivative err from the previous iteration */
	T max_derivative_err_;  /* max deriivative err from the last 2 iterations */
	T min_derivative_err_;  /* min derivative err from the last 2 iterations */
	T last_err_input_;      /* error input from the previous iteration */
	float kp_;              /*proportional gain*/
	float ki_;              /*integral gain*/
	float kd_;              /*derivative gain*/

	/*
	 * Low pass derivative filter, filtering out sudden jumps in the derivative
	 * err. Calculated as 1 / (2 * PI * Fc)
	 */
	float filter_;
};



typedef PidController<Vector3f> PidController3f;

#endif /* PIDCONTROLLER_H_ */
