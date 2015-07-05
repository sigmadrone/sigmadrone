/*
 * rcvalueconverter.cpp
 *
 *  Created on: Jul 5, 2015
 *      Author: svassilev
 */

#include "rcvalueconverter.h"

const float Throttle::MIN_VALUE = 0.0;
const float Throttle::MAX_VALUE = 1.0;

static const TimeSpan max_pwm_signal = TimeSpan::from_milliseconds(2);

static float get_2ms_based_duty_cycle(const TimeSpan& duty) {
	return (float)duty.microseconds() / (float)max_pwm_signal.microseconds();
}

RcValueConverter::RcValueConverter(const RcChannelMapper& mapper, const RcReceiver& receiver) {
	const PwmDecoder& throttlePwm = receiver.channel(mapper.channel_no(RC_CHANNEL_THROTTLE))->decoder();
	throttle_ = Throttle(get_2ms_based_duty_cycle(throttlePwm.duty_cycle()));
}

QuaternionD RcValueConverter::target_quaternion() {
	return quaternion_;
}

Throttle RcValueConverter::base_throttle() {
	return throttle_;
}
