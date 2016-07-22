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

#ifndef PWMPULSE_H_
#define PWMPULSE_H_

#include "rcchannelmapper.h"
#include "d3math.h"
#include "rcreceiver.h"

struct PwmPulse {
	PwmPulse(const TimeSpan& min_duty, const TimeSpan& max_duty) : min_(min_duty), max_(max_duty)
	{
		assert(min_ < max_);
		assert(!min_.is_null());
	}

	inline float to_float(const TimeSpan& _duty_cycle)
	{
		TimeSpan duty_cycle = _duty_cycle;
		if (duty_cycle.is_null()) {
			return 0.0;
		}
		if (duty_cycle <= min_) {
			duty_cycle = min_ + TimeSpan::from_microseconds(1);
		} else if (duty_cycle > max_) {
			duty_cycle = max_;
		}
		return static_cast<float>((duty_cycle-min_).microseconds()) /
				static_cast<float>((max_-min_).microseconds());
	}

	inline TimeSpan to_timespan(float value)
	{
		assert(value >= 0 && value <= 1.0);
		float nsecs = static_cast<float>((max_-min_).nanoseconds()) * value +
				static_cast<float>(min_.nanoseconds());
		return TimeSpan::from_nanoseconds(nsecs);
	}

private:
	TimeSpan min_;
	TimeSpan max_;
};

#endif /* PWMPULSE_H_ */
