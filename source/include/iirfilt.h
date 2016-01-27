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

#ifndef __IIRFILT_H__
#define __IIRFILT_H__

#include <string.h>
#include "d3math.h"


template <typename T, const size_t N>
class IirFilter
{
public:

	using CoeffNominator = MatrixMN<T, N, 1>;
	using CoeffDenominator = MatrixMN<T, N, 1>;

	IirFilter() { reset(); }
	IirFilter(const CoeffNominator& coeff_nom, const CoeffDenominator& coeff_denom) :
		nominator_(coeff_nom), denominator_(coeff_denom) { reset(); }

	~IirFilter() = default;

	void reset()
	{
		memset(state_,0,sizeof(state_));
		out_ = 0;
	}
	const T& do_filter(const T& in)
	{
		out_ = nominator_[0] * in + state_[0];
		for (size_t i = 1; i < N; ++i) {
			state_[i-1] = nominator_[i] * in + state_[i] - denominator_[i] * out_;
		}
		return out_;
	}

	const CoeffNominator& get_coeff_nom() const { return nominator_.get_coeff(); }
	const CoeffDenominator& get_coeff_denom() const { return denominator_.get_coeff(); }
	const T& get_output() const { return out_; }

private:
	T state_[N];
	CoeffNominator nominator_;
	CoeffDenominator denominator_;
	T out_;
};


/*
 * IIR LPF, calculated using the elliptic method:
 *  Order=6
 *  Fc=10Hz
 *  Attenuation=40dB
 */
template <typename T>
class IirSensorPreFilter10Hz: public IirFilter<T, 7>  {
public:
	IirSensorPreFilter10Hz() : IirFilter<T, 7>(
			{ 0.009130213043213, -0.053785884660517, 0.132987927812535, -0.176664445470034,
		      0.132987927812535,  -0.053785884660517,   0.009130213043213},
			{1.0, -5.763011904477186, 13.843008765719702, -17.739823971400483,
				   12.791644394232126, -4.920788915790463, 0.788971698636730}) {}
};

 /*
  * IIR LPF, calculated using the Chebyshev 2nd method:
  *  Order=6
  *  Fc=5Hz
  *  Attenuation=40dB
  */
 template <typename T>
 class IirSensorPreFilter5Hz: public IirFilter<T, 7>  {
 public:
 	IirSensorPreFilter5Hz() : IirFilter<T, 7>(
 			{ 0.009490307141041, -0.056679620583394, 0.141306795302753, -0.188234962613877,
		      0.141306795302753, -0.056679620583394, 0.009490307141041},
 			{1.0, -5.881527464347489, 14.414639972281712, -18.843021855934243,
 			 13.856507112731949, -5.434871112577345, 0.888273348952340}) {}
 };

typedef IirSensorPreFilter5Hz<float> IirSensorPreFilter1f;
typedef IirSensorPreFilter5Hz<double> IirSensorPreFilter1d;

#endif // __IIRFILT_H__
