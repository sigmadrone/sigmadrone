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
				mapper_(mapper), receiver_(receiver), scale_factor_(scale_factor), motors_armed_(false) {
	update();
}

void RcValueConverter::update() {
	float yaw = get_value_as_float(mapper_.channel_no(RC_CHANNEL_YAW));
	float pitch = get_value_as_float(mapper_.channel_no(RC_CHANNEL_PITCH));
	float roll = get_value_as_float(mapper_.channel_no(RC_CHANNEL_ROLL));
	float throttle = get_value_as_float(mapper_.channel_no(RC_CHANNEL_THROTTLE));
	float gear = get_value_as_float(mapper_.channel_no(RC_CHANNEL_ARM_MOTOR));

	throttle_ = Throttle(throttle * scale_factor_);
	if (yaw > 0.0) {
		yaw = (yaw - 0.5) * MAX_EULER_FROM_RC * scale_factor_;
	}
	if (pitch > 0.0) {
		pitch = (pitch - 0.5) * MAX_EULER_FROM_RC * scale_factor_;
	}
	if (roll > 0.0) {
		roll = (roll - 0.5) * MAX_EULER_FROM_RC * scale_factor_;
	}
	quaternion_ = QuaternionF::fromEulerAngles(roll, pitch, yaw);

	/*
	 * Note: on Spektrum DX8 by _default_ GEAR switch set to 0 results in max pulse and
	 * GEAR set to 1 results in min pulse. This settings of course can be reversed, but
	 * we'd rather work with the defaults. I have no experience with other RCs.
	 */
	if (gear > 0.0 && gear < 0.5) {
		motors_armed_ = true;
	} else {
		motors_armed_ = false;
	}
}

float RcValueConverter::get_value_as_float(uint32_t channelno)
{
	TimeSpan duty_cycle = receiver_.channel(channelno)->decoder().duty_cycle();
	return pwm_converter_.to_float(duty_cycle);
}

QuaternionD RcValueConverter::target_quaternion() const {
	return quaternion_;
}

Throttle RcValueConverter::base_throttle() const {
	return throttle_;
}

bool RcValueConverter::motors_armed() const {
	return motors_armed_;
}
