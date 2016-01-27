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

template <typename DataType, typename CoeffType, const size_t N>
class FirFilter
{
public:
	using Nominator = std::array<CoeffType,N>;
	using State = std::array<DataType,N>;

	FirFilter() { reset(); }
	FirFilter(const Nominator& nominator): nominator_(nominator) { reset(); }
	~FirFilter() = default;

	void reset()
	{
		for (auto& s : state_) {
			s = static_cast<DataType>(0);
		}
		out_ = static_cast<DataType>(0);
	}
	const DataType& do_filter(const DataType& in)
	{
		out_ = in * nominator_[0] + state_[0];
		for (size_t i = 1; i < N; ++i) {
			state_[i-1] = in * nominator_[i] + state_[i];
		}
		return out_;
	}
	const Nominator& nominator() const { return nominator_; }
	const DataType& output()     const { return out_; }

private:
	State state_;
	Nominator nominator_;
	DataType out_;
};

/*
 * class LpPreFilter3d
 *
 * Implements 10th order low-pass filter, with cutoff frequency
 * Fc = 0.1 * Fs / 2. The filter is designed using the windows method
 * and the frequency response does not have particularly steep slope -
 * 20 dB attenuation is achieved at 0.3*Fs/2; 40 dB @ 0.4*Fs/2
 */

class LpPreFilter3d : public FirFilter<Vector3d, double, 11>
{
	using Base = FirFilter<Vector3d, double, 11>;
public:
	LpPreFilter3d() : Base({0.0143f, 0.0303f, 0.0723f, 0.1245f, 0.1670f, 0.1833f,
		0.1670f, 0.1245f, 0.0723f, 0.0303f, 0.0143f}) {}
};

template<typename DataType, typename CoeffType, const size_t N>
class MovingAverageFilter: public FirFilter<DataType, CoeffType, N>
{
	using Base = FirFilter<DataType, CoeffType, N>;
	using Nominator = std::array<CoeffType,N>;

public:
	MovingAverageFilter() : Base(GenerateCoefficients()) {}

private:
	static Nominator GenerateCoefficients()
	{
		Nominator nom;
		nom.fill(static_cast<CoeffType>(1) / N);
		return nom;
	}
};


#endif // __FIRFILT_H__
