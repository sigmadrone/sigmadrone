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

#ifndef SENSORS_PREFILTERS_H_
#define SENSORS_PREFILTERS_H_

#include "firfilt.h"
#include "iirfilt.h"

class MagLowPassPreFilter3d : public FirFilter<Vector3f, float, 18> {
	using Base = FirFilter<Vector3f, float, 18>;
public:
	MagLowPassPreFilter3d() : Base(
			{0.0086, 0.0120, 0.0216, 0.0362, 0.0537, 0.0719, 0.0883, 0.1006, 0.1072, 0.1072,
		0.1006, 0.0883, 0.0719, 0.0537, 0.0362, 0.0216, 0.0120, 0.0086}) {}
};

typedef IirLowPassFilter5Hz<Vector3d, double> AccelLowPassPreFilter3d;

/*
  * IIR LPF
  *  Order=2
  *  Fc=20 Hz
  *  Fs=760 Hz
  */
template <typename DataType>
class IirLpfFs760Fc20: public IirFilter<DataType, double, 3>  {
public:
	IirLpfFs760Fc20() : IirFilter<DataType, double, 3>(
			 {0.00610841465080494, 0.0122168293016099, 0.00610841465080494},
			 {1.000000000000000, -1.76706147650352, 0.791495135106737}) {}
};

/*
  * IIR LPF
  *  Order=2
  *  Fc=50 Hz
  *  Fs=760 Hz
  */
template <typename DataType>
class IirLpfFs760Fc50: public IirFilter<DataType, double, 3>  {
public:
	IirLpfFs760Fc50() : IirFilter<DataType, double, 3>(
			 {0.032797487266435,0.065594974532871,0.032797487266435},
			 {1.000000000000000,-1.426392036019501,0.557558198508524}) {}
};

template <typename DataType>
class FirLpfFs760Fc20 : public FirFilter<DataType, float, 7> {
public:
	FirLpfFs760Fc20() : FirFilter<DataType, float, 7>(
			{ 0.000160,0.027691,0.242240,0.459815,0.242240,0.027691,0.000160}) {}
};

template <typename DataType>
class FirLpfFs760Fc10 : public FirFilter<DataType, float, 5> {
public:
	FirLpfFs760Fc10() : FirFilter<DataType, float, 5>(
			{ 0.000248354104054,0.155912925734534,0.687677440322823,0.155912925734535,0.000248354104054}) {}
};

typedef FirLpfFs760Fc10<Vector3f> GyroPreFilter;


template <typename DataType>
class FirLpfFs100Fc10 : public FirFilter<DataType, float, 7> {
public:
	FirLpfFs100Fc10() : FirFilter<DataType, float, 7>(
			{ 0.096350,0.11920787812041358,0.15598533208570348,0.16991505955919545,
		0.15598533208570348,0.11920787812041358,0.09635098809039429}) {}
};

template <typename DataType>
class MedianFilter // simple 3rd order median filter
{
public:
	MedianFilter() { reset(); }
	~MedianFilter() = default;

	void reset() { min_ = max_ = out_ = last_in_ = 0; }
	const DataType& do_filter(const DataType& in)
	{
		out_ = in;
		if (out_ > max_) {
			out_ = max_;
		} else if (out_ < min_) {
			out_ = min_;
		}
		if (in > last_in_) {
			max_ = in;
			min_ = last_in_;
		} else {
			min_ = in;
			max_ = last_in_;
		}
		last_in_ = in;
		return out_;
	}
	const DataType& output() const { return out_; }
private:
	DataType min_;
	DataType max_;
	DataType out_;
	DataType last_in_;
};

template <typename DataType, typename CoeffType>
class LowPassFilter
{
public:
	LowPassFilter(CoeffType alpha) : alpha_(alpha), beta_(1.0 - alpha) { reset(); }
	LowPassFilter(CoeffType alpha, CoeffType beta) : alpha_(alpha), beta_(beta) { reset(); }
	~LowPassFilter() = default;

	void reset(const DataType& out = {0})
	{
		out_ = out;
	}

	const DataType& do_filter(const DataType& in)
	{
		out_ = out_ * alpha_ + in * beta_;
		return out_;
	}

	const DataType& output() const
	{
		return out_;
	}

	void set_alpha(const CoeffType& alpha)
	{
		alpha_ = alpha;
		beta_ = 1.0 - alpha;
	}

	CoeffType alpha() { return alpha_; }
private:
	DataType out_;
	CoeffType alpha_, beta_;
};

class PressurePreFilter: public LowPassFilter<float, float>
{
public:
	PressurePreFilter() : LowPassFilter<float, float> (0.965) {}
};

typedef PressurePreFilter TemperaturePreFilter;

#endif /* SENSORS_PREFILTERS_H_ */
