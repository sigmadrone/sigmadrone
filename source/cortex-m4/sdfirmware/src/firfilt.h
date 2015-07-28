/*
 * firfilt.h
 *
 *  Created on: Jul 25, 2015
 *      Author: svassilev
 */

#ifndef FIRFILT_H_
#define FIRFILT_H_
/*
 * Copyright (C) 2013 Svetoslav Vassilev
 */

#ifndef __FIRFILT_H__
#define __FIRFILT_H__

#include <string.h>
#include "matrix.h"

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
		do_filter((const T*)in.data);
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

#endif // __FIRFILT_H__





#endif /* FIRFILT_H_ */
