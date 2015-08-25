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

struct Throttle: public ScaledUnit<float> {
	static const float MIN_VALUE;// = 0.0;
	static const float MAX_VALUE;// = 1.0;
	inline Throttle(float throttle = 0.0) : ScaledUnit<float>(throttle) {}
	inline float get(float min = MIN_VALUE, float max = MAX_VALUE) const {
		return (unit() < min ? min : (unit() > max ? max : unit()));
	}
	float get_unbound() const { return unit(); }
private:
};

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
	Throttle base_throttle() const;
	bool motors_armed() const;
	void update();
	float get_yaw() const;
	float get_pitch() const;
	float get_roll() const;

private:
	float get_value_as_float(uint32_t channelno);
private:
	PwmPulse pwm_converter_;
	QuaternionF quaternion_;
	QuaternionF quaternion_yaw_;
	Throttle throttle_;
	const RcChannelMapper& mapper_;
	RcReceiver& receiver_;
	float scale_factor_;
	float last_gear_;
	bool motors_armed_;
	float yaw_;
	float pitch_;
	float roll_;

};

#endif /* RCVALUECONVERTER_H_ */
