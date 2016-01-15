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

#ifndef RCVALUECONVERTER_H_
#define RCVALUECONVERTER_H_

#include "rcchannelmapper.h"
#include "d3math.h"
#include "rcreceiver.h"

struct PwmPulse {
	PwmPulse(const TimeSpan& min_duty, const TimeSpan& max_duty) : min_(min_duty), max_(max_duty) {
		assert(min_ < max_);
		assert(!min_.is_null());
	}
	inline float to_float(const TimeSpan& _duty_cycle) {
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
	inline TimeSpan to_timespan(float value) {
		assert(value >= 0 && value <= 1.0);
		float usecs = static_cast<float>((max_-min_).microseconds()) * value +
				static_cast<float>(min_.microseconds());
		return TimeSpan::from_microseconds(usecs);
	}
private:
	TimeSpan min_;
	TimeSpan max_;
};

class RcValueConverter {
public:
	RcValueConverter(const RcChannelMapper& mapper, RcReceiver& receiver,
			float scale_factor = 1.0,
			const TimeSpan& min_duty_cycle = TimeSpan::from_milliseconds(1),
			const TimeSpan& max_duty_cycle = TimeSpan::from_milliseconds(2));
	QuaternionF target_quaternion() const;
	QuaternionF target_twist() const;
	QuaternionF target_swing() const;
	Throttle base_throttle() const;
	bool motors_armed() const;
	bool previous_motors_armed() const;
	void update();
	float get_yaw() const;
	float get_pitch() const;
	float get_roll() const;
	void reset_twist_quaternion(const QuaternionF&);

private:
	float get_value_as_float(uint32_t channelno);
private:
	PwmPulse pwm_converter_;
	QuaternionF target_twist_;		/* Yaw */
	QuaternionF target_swing_;		/* Combined rotation of Pitch and Roll */
	Throttle throttle_;
	const RcChannelMapper& mapper_;
	RcReceiver& receiver_;
	float scale_factor_;
	float last_gear_;
	bool motors_armed_;
	bool prev_motors_armed_;
	float yaw_;
	float pitch_;
	float roll_;

};

#endif /* RCVALUECONVERTER_H_ */
