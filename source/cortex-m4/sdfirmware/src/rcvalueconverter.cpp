/*
 * rcvalueconverter.cpp
 *
 *  Created on: Jul 5, 2015
 *      Author: svassilev
 */

#include "rcvalueconverter.h"

const float Throttle::MIN_VALUE = 0.0;
const float Throttle::MAX_VALUE = 1.0;

static const float MAX_EULER_FROM_RC = M_PI / 6.0;

RcValueConverter::RcValueConverter(
		const RcChannelMapper& mapper,
		const RcReceiver& receiver,
		float scale_factor,
		const TimeSpan& min_duty_cycle,
		const TimeSpan& max_duty_cycle) : pwm_converter_(min_duty_cycle, max_duty_cycle),
				mapper_(mapper), receiver_(receiver), scale_factor_(scale_factor) {
	update();
}

void RcValueConverter::update() {
	throttle_ = Throttle(get_value_as_float(receiver_, mapper_.channel_no(RC_CHANNEL_THROTTLE)) * scale_factor_);
	float yaw = get_value_as_float(receiver_, mapper_.channel_no(RC_CHANNEL_YAW));
	if (yaw > 0.0) {
		yaw = (yaw - 0.5) * MAX_EULER_FROM_RC * scale_factor_;
	}
	float pitch = get_value_as_float(receiver_, mapper_.channel_no(RC_CHANNEL_PITCH));
	if (pitch > 0.0) {
		pitch = (pitch - 0.5) * MAX_EULER_FROM_RC * scale_factor_;
	}
	float roll = get_value_as_float(receiver_, mapper_.channel_no(RC_CHANNEL_ROLL));
	if (roll > 0.0) {
		roll = (roll - 0.5) * MAX_EULER_FROM_RC * scale_factor_;
	}
	quaternion_ = QuaternionF::fromEulerAngles(roll, pitch, yaw);
}

float RcValueConverter::get_value_as_float(const RcReceiver& receiver, uint32_t channelno)
{
	TimeSpan duty_cycle = receiver.channel(channelno)->decoder().duty_cycle();
	return pwm_converter_.to_float(duty_cycle);
}

QuaternionD RcValueConverter::target_quaternion() const {
	return quaternion_;
}

Throttle RcValueConverter::base_throttle() const {
	return throttle_;
}
