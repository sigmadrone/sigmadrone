/*
 * rcvalueconverter.h
 *
 *  Created on: Jul 5, 2015
 *      Author: svassilev
 */

#ifndef RCVALUECONVERTER_H_
#define RCVALUECONVERTER_H_

#include "rcchannelmapper.h"
#include "matrix.h"
#include "rcreceiver.h"

struct Throttle {
	static const float MIN_VALUE;// = 0.0;
	static const float MAX_VALUE;// = 1.0;
	inline Throttle(float throttle = 0.0) : throttle_(throttle) {}
	inline float get(float min = MIN_VALUE, float max = MAX_VALUE) const {
		return (throttle_ < min ? min : (throttle_ > max ? max : throttle_));
	}
	float get_unbound() const { return throttle_; }
private:
	float throttle_;
};

struct PwmPulse {
	PwmPulse(const TimeSpan& min_duty, const TimeSpan& max_duty) : min_(min_duty), max_(max_duty) {
		assert(min_ < max_);
	}
	inline float to_float(const TimeSpan& duty_cycle) {
		if (duty_cycle <= min_) {
			return 0.0;
		} else if (duty_cycle >= max_) {
			return 1.0;
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
	RcValueConverter(const RcChannelMapper& mapper, const RcReceiver& receiver,
			const TimeSpan& min_duty_cycle = TimeSpan::from_milliseconds(1),
			const TimeSpan& max_duty_cycle = TimeSpan::from_milliseconds(2));
	QuaternionD target_quaternion() const;
	Throttle base_throttle() const;
	void update();
private:
	float get_value_as_float(const RcReceiver& receiver, uint32_t channelno);
private:
	PwmPulse pwm_converter_;
	QuaternionF quaternion_;
	Throttle throttle_;
	const RcChannelMapper& mapper_;
	const RcReceiver& receiver_;
};

#endif /* RCVALUECONVERTER_H_ */
