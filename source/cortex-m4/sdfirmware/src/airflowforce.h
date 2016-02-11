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

#ifndef AIRFLOWFORCE_H_
#define AIRFLOWFORCE_H_

template<typename T>
class AirflowForce {
public:
	static constexpr T Ro = 1.225; 		// Air density

	AirflowForce(T diameter, T maxthrust)
		: half_Ro_A_(0.5 * Ro * 3.14159265 * (diameter * diameter / 4.0))
		, Fmax_(maxthrust)
		, Vmax_(std::sqrt(Fmax_ / half_Ro_A_))
	{
	}

	T force(T Vout, T Vin = 0.0) const
	{
		return half_Ro_A_ * (Vout - Vin) * (Vout + Vin);
	}

	T velocity_delta(T deltaF, T Vout, T Vin = 0.0) const
	{
		T F = force(Vout, Vin);
		T Fnew = (F + deltaF > 0) ? F + deltaF : 0.0f;
		T c = Vout * Vout - (Fnew + half_Ro_A_ * Vin * Vin) / half_Ro_A_;
		T b = 2.0f * Vout;
		const T a = 1.0f;
		T Det = std::sqrt(b * b - 4.0f * a * c);
		T x1 = 0.5 * (-b + Det) / a;
	//	T x2 = 0.5 * (-b - Det) / a;
		return x1;
	}

	T velocity(T F, T Vin = 0.0) const
	{
		if (F < 0)
			return static_cast<T>(0);
		return std::sqrt(Vin + F / half_Ro_A_);
	}

	/*
	 * Return the Force as a percentage of the maxtrhust i.e 0..1
	 */
	T nforce(T Vout, T Vin = 0.0) const
	{
		return force(Vout * Vmax_, Vin * Vmax_) / Fmax_;
	}

	/*
	 * Return the Velocity as a percentage, i.e. 0..1
	 */
	T nvelocity_delta(T deltaF, T Vout, T Vin = 0.0) const
	{
		return velocity_delta(deltaF * Fmax_, Vout * Vmax_, Vin * Vmax_) / Vmax_;
	}

	T nvelocity(T F, T Vin = 0.0) const
	{
		return velocity(F * Fmax_, Vin * Vmax_) / Vmax_;
	}

private:
	T half_Ro_A_;
	T Fmax_;
	T Vmax_;
};

#endif /* AIRFLOWFORCE_H_ */
