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

#ifndef __FIRFILT_H__
#define __FIRFILT_H__

#include <string.h>
#include "d3math.h"

template<typename T, int N>
struct FiltVector{
	typedef MatrixMN<T, N, 1> type;
};

template <typename T, const size_t N, const size_t DIM>
class FirFilter
{
public:
	FirFilter()
	{
		reset();
		memset(coeff_,0,sizeof(coeff_));
		coeff_[0] = 1.0;
	}
	FirFilter(const T coeff[N])
	{
		reset();
		memcpy(coeff_,coeff,sizeof(coeff_));
	}
	__inline ~FirFilter() {}
	__inline void reset()
	{
		memset(state_,0,sizeof(state_));
		memset(output_,0,sizeof(output_));
		ptr_ = 0;
	}
	void init_coeff(const T coeff[N])
	{
		memcpy(coeff_,coeff,sizeof(coeff_));
		reset();
	}
	const T* do_filter(const T in[DIM])
	{
		for (size_t i = 0; i < DIM; i++)
		{
			output_[i] = 0.0;
			state_[i][ptr_] = in[i];
			for (size_t j = 0; j < N; j++)
			{
				output_[i] += state_[i][ptr_] * coeff_[j];
				retreat_ptr();
			}
		}
		advance_ptr();  /*prepare for the next input sample*/
		return output_;
	}

	typename FiltVector<T,DIM>::type
	do_filter(const typename FiltVector<T,DIM>::type& in)
	{
		typename FiltVector<T,DIM>::type out;
		do_filter((const T*)in);
		for (size_t i = 0; i < DIM; ++i) {
			out.at(i,0) = output_[i];
		}
		return out;
	}
	const T* /*[N]*/ get_coeff() const { return coeff_; }
	const T* /*[DIM]*/ get_output() const { return output_; }

private:
	__inline void advance_ptr() { ptr_ = (ptr_ + 1) % N; }
	__inline void retreat_ptr() { ptr_ = (0 == ptr_) ? N-1 : ptr_-1; }

private:
	T state_[DIM][N];
	T coeff_[N];
	T output_[DIM];
	size_t ptr_;
};

template<const size_t N>
struct FirFilter3f {
  typedef FirFilter<float, N, 3> type;
};

template<const size_t N>
struct FirFilter3d {
  typedef FirFilter<double, N, 3> type;
};


/*
 * class LpPreFilter3d
 *
 * Implements 10th order low-pass filter, with cutoff frequency
 * Fc = 0.1 * Fs / 2. The filter is designed using the windows method
 * and the frequency response does not have particularly steep slope -
 * 20 dB attenuation is achieved at 0.3*Fs/2; 40 dB @ 0.4*Fs/2
 */

class LpPreFilter3d
{
public:
	LpPreFilter3d(): fir_filter_()
	{
		static double s_Coeff[] = {
			0.0143f, 0.0303f, 0.0723f, 0.1245f, 0.1670f, 0.1833f,
			0.1670f, 0.1245f, 0.0723f, 0.0303f, 0.0143f
		};
		fir_filter_.init_coeff(s_Coeff);
	}

	__inline void Reset() { fir_filter_.reset(); }
	const double* DoFilter(double in[3])
	{
		return fir_filter_.do_filter(in);
	}
	const double* DoFilter(double a, double b, double c)
	{
		double in[3] = {a,b,c};
		return fir_filter_.do_filter(in);
	}
	Vector3f DoFilter(const Vector3f& in)
	{
		Vector3f out;
		double inArr[3] = {in.at(0,0),in.at(1,0),in.at(2,0)};
		DoFilter(inArr);
		out.at(0,0) = fir_filter_.get_output()[0];
		out.at(1,0) = fir_filter_.get_output()[1];
		out.at(2,0) = fir_filter_.get_output()[2];
		return out;
	}
	const double* GetCoeff() { return fir_filter_.get_coeff(); }
	const double* GetOutput() { return fir_filter_.get_output(); }

	static const size_t s_Order = 11;
	static const size_t s_Dim = 3;

private:
	FirFilter<double, s_Order, s_Dim> fir_filter_;
};

#endif // __FIRFILT_H__
