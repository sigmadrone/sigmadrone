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
 *
 *
 *  The implementation of the derivative filter is entirely based on:
 *  http://www.holoborodko.com/pavel/numerical-methods/numerical-derivative/smooth-low-noise-differentiators/
 *
 */

#ifndef DERIVATIVE_FILTER_H_
#define DERIVATIVE_FILTER_H_

#include <assert.h>
#include "timestamp.h"


template <typename T>
T factorial(T num)
{
	assert(num >= 0);
	uint64_t res = 1;
	for (size_t i = 2; i < num; ++i) {
		res *= i;
	}
	return static_cast<T>(res);
}

template <typename T>
T binom_coeff(T n, T k)
{
	return factorial(n) / (factorial(n-k) * factorial(k));
}

template <typename DataType, const size_t N>
class DerivativeFilter
{
public:
	using State = std::array<DataType,N>;
	static const size_t M = (N-1)/2;
	using Coeff = std::array<float, M>;

	DerivativeFilter()
	{
		assert(N%2 != 0);
		populate_coeff();
		reset();
	}

	~DerivativeFilter() = default;

	void reset()
	{
		for (auto& s : state_) {
			s = static_cast<DataType>(0);
		}
		out_ = static_cast<DataType>(0);
		index_ = 0;
		ready_ = false;
	}
	const DataType& do_filter(const DataType& in, const TimeSpan& dt)
	{
		state_[index_] = in;
		if (index_ == N -1) {
			ready_ = true;
		}
		if (ready_) {
			out_ = static_cast<DataType>(0);
			for (size_t k = 1; k <= M; ++k) {
				if (dt.seconds_float() < 0.0001) {
					out_ = static_cast<DataType>(0);
					break;
				}
				out_ += ck(k) * (fk(k) - fk(-k));
			}
			out_ /= dt.seconds_float();
		}
		index_ = (index_+1) % N;
		return out_;
	}
	const DataType& output()     const { return out_; }

private:

	void populate_coeff()
	{
		// ugly, but the formulas cited in the paper do not pan out
		switch (N) {
		case 5:
			coeff_[0] = 2.0f/8.0f;
			coeff_[1] = 1.0f/8.0f;
			break;
		case 7:
#if 1
			coeff_[0] = 5.0f/32.0f;
			coeff_[1] = 4.0f/32.0f;
			coeff_[2] = 1.0f/32.0f;
#else
			coeff_[0] = 27.0f/96.0f;
			coeff_[1] = 12.0f/96.0f;
			coeff_[2] = -5.0f/96.0f;
#endif
			break;
		case 9:
			coeff_[0] = 14.0f/128.0f;
			coeff_[1] = 14.0f/128.0f;
			coeff_[2] = 6.0f/128.0f;
			coeff_[3] = 1.0f/128.0f;
			break;
		case 11:
			coeff_[0] = 42.0f/512.0f;
			coeff_[1] = 48.0f/512.0f;
			coeff_[2] = 27.0f/512.0f;
			coeff_[3] = 8.0f /512.0f;
			coeff_[4] = 1.0f /512.0f;
			break;
		default:
			assert(false);
		}
	}

	DataType fk(size_t k) { return state_[rel_index(k)]; }
	size_t rel_index(int k)
	{
		int idx = static_cast<int>(index_+k-M) % static_cast<int>(N);
		if (idx < 0) {
			idx = N + idx;
		}
		return idx;
	}
	DataType ck(size_t k)
	{
		assert(k > 0 && k <= coeff_.size());
		return coeff_[k-1];
	}

private:
	State state_;
	Coeff coeff_;
	size_t index_;
	DataType out_;
	bool ready_;
};


#endif /* DERIVATIVE_FILTER_H_ */
