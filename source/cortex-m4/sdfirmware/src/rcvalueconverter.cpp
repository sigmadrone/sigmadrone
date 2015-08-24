/*
 * rcvalueconverter.cpp
 *
 *  Created on: Jul 5, 2015
 *      Author: svassilev
 */

#include "rcvalueconverter.h"

const float Throttle::MIN_VALUE = 0.0;
const float Throttle::MAX_VALUE = 1.0;

static const float MAX_EULER_FROM_RC = M_PI / 4.0;

RcValueConverter::RcValueConverter(
		const RcChannelMapper& mapper,
		RcReceiver& receiver,
		float scale_factor,
		const TimeSpan& min_duty_cycle,
		const TimeSpan& max_duty_cycle) : pwm_converter_(min_duty_cycle, max_duty_cycle),
				mapper_(mapper), receiver_(receiver), scale_factor_(scale_factor),
				last_gear_(0.0), integrated_yaw_(0.0f), motors_armed_(false) {
	receiver_.channel(mapper_.channel_no(RC_CHANNEL_YAW))->decoder().callback_on_change_only(false);
	update();
}

void RcValueConverter::update() {
	float yaw = get_value_as_float(mapper_.channel_no(RC_CHANNEL_YAW));
	float pitch = get_value_as_float(mapper_.channel_no(RC_CHANNEL_PITCH));
	float roll = get_value_as_float(mapper_.channel_no(RC_CHANNEL_ROLL));
	float throttle = get_value_as_float(mapper_.channel_no(RC_CHANNEL_THROTTLE));
	float gear = get_value_as_float(mapper_.channel_no(RC_CHANNEL_ARM_MOTOR));

	throttle_ = Throttle(throttle * scale_factor_);
	if (yaw > 0.0) {// && (fabs(yaw-0.5f) > 0.05)) {
		TimeSpan dt = receiver_.channel(mapper_.channel_no(RC_CHANNEL_YAW))->decoder().decoded_period();
		integrated_yaw_ += (yaw - 0.5) * (MAX_EULER_FROM_RC/4) * scale_factor_ * dt.seconds_float();
		yaw = (yaw - 0.5) * MAX_EULER_FROM_RC * scale_factor_;
	}
	if (pitch > 0.0) {
		pitch = -1.0 * (pitch - 0.5) * MAX_EULER_FROM_RC * scale_factor_;
	}
	if (roll > 0.0) {
		roll = (roll - 0.5) * MAX_EULER_FROM_RC * scale_factor_;
	}
//	quaternion_ = QuaternionF::fromEulerAngles(roll, pitch, yaw);
	quaternion_ = QuaternionF::fromAngularVelocity(Vector3f(roll, pitch, 0), 1.0);

	/*
	 * Note: on Spektrum DX8 by _default_ GEAR switch set to 0 results in max pulse and
	 * GEAR set to 1 results in min pulse. This settings of course can be reversed, but
	 * we'd rather work with the defaults. Here we will assume that gear set to 1 means
	 * "landed" and gear set to 0 "prepare for take off", i.e. motors armed.
	 */
	if (last_gear_ != 0.0 && last_gear_ != gear) {
		motors_armed_ = (gear > 0.5) ? true : false;
	}
	last_gear_ = gear;
}

float RcValueConverter::get_value_as_float(uint32_t channelno)
{
	TimeSpan duty_cycle = receiver_.channel(channelno)->decoder().duty_cycle();
	return pwm_converter_.to_float(duty_cycle);
}

QuaternionF RcValueConverter::target_quaternion() const {
	return quaternion_;
}

Throttle RcValueConverter::base_throttle() const {
	return throttle_;
}

bool RcValueConverter::motors_armed() const {
	return motors_armed_;
}
