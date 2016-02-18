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
#include "fixedpoint.h"

template <typename DataType, typename CoeffType, const size_t N>
class IirFilter
{
public:
	using Nominator = std::array<CoeffType,N>;
	using Denominator = std::array<CoeffType,N>;
	using State = std::array<DataType,N>;

	IirFilter()  { reset(); }
	IirFilter(const Nominator& coeff_nom, const Denominator& coeff_denom) :
		nominator_(coeff_nom), denominator_(coeff_denom) { reset(); }

	~IirFilter() = default;

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
			state_[i-1] = in * nominator_[i] + state_[i] - out_ * denominator_[i];
		}
		return out_;
	}

	const Nominator& nominator() const { return nominator_; }
	const Denominator& denominator() const { return denominator_; }
	const DataType& output() const { return out_; }

private:
	State state_;
	Nominator nominator_;
	Denominator denominator_;
	DataType out_;
};


/*
 * IIR LPF, calculated using the elliptic method:
 *  Order       = 6
 *  Fc          = 0.025 * Fs / 2 (10Hz when Fs is 800 Hz)
 *  Attenuation = 40dB
 */
template <typename DataType, typename CoeffType>
class IirLowPassFilter10Hz: public IirFilter<DataType, CoeffType, 7>  {
public:
	IirLowPassFilter10Hz() : IirFilter<DataType, CoeffType, 7>(
			{ 0.009130213043213, -0.053785884660517, 0.132987927812535, -0.176664445470034,
		      0.132987927812535,  -0.053785884660517,   0.009130213043213},
			{1.0, -5.763011904477186, 13.843008765719702, -17.739823971400483,
				   12.791644394232126, -4.920788915790463, 0.788971698636730}) {}
};

template <typename DataType>
class IirLpf10HzFixedPoint: public IirFilter<DataType, Q16dot48, 7>  {
public:
	IirLpf10HzFixedPoint() : IirFilter<DataType, Q16dot48, 7>(
			{ 0.009130213043213, -0.053785884660517, 0.132987927812535, -0.176664445470034,
			  0.132987927812535, -0.053785884660517, 0.009130213043213},
			{1.0, -5.763011904477186, 13.843008765719702, -17.739823971400483,
			      12.791644394232126,-4.920788915790463, 0.788971698636730}) {}
};


 /*
  * IIR LPF, calculated using the Chebyshev 2nd method:
  *  Order       = 6
  *  Fc          = 0.0125 * Fs / 2 (5 Hz when Fs is 800 Hz)
  *  Attenuation = 40dB
  */
 template <typename DataType, typename CoeffType>
 class IirLowPassFilter5Hz: public IirFilter<DataType, CoeffType, 7>  {
 public:
 	IirLowPassFilter5Hz() : IirFilter<DataType, CoeffType, 7>(
 			{ 0.009490307141041, -0.056679620583394, 0.141306795302753, -0.188234962613877,
		      0.141306795302753, -0.056679620583394, 0.009490307141041},
 			{1.0, -5.881527464347489, 14.414639972281712, -18.843021855934243,
 			 13.856507112731949, -5.434871112577345, 0.888273348952340}) {}
 };

 template <typename DataType>
  class IirLpf5HzFixedPoint: public IirFilter<DataType, Q16dot48, 7>  {
  public:
  	IirLpf5HzFixedPoint() : IirFilter<DataType, Q16dot48, 7>(
  			{ 0.009490307141041, -0.056679620583394, 0.141306795302753, -0.188234962613877,
 		      0.141306795302753, -0.056679620583394, 0.009490307141041},
  			{1.0, -5.881527464347489, 14.414639972281712, -18.843021855934243,
  			 13.856507112731949, -5.434871112577345, 0.888273348952340}) {}
  };

#endif // __IIRFILT_H__
