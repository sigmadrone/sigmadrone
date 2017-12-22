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
#include "rcvalueconverter.h"
#include "colibrihwmap.h"
#include "dronestate.h"

static const float MAX_EULER_FROM_RC = M_PI / 4.0;

RcValueConverter::RcValueConverter(
		const RcChannelMapper& mapper,
		RcReceiver& receiver,
		const TimeSpan& min_duty_cycle,
		const TimeSpan& max_duty_cycle) :
			    pwm_converter_(min_duty_cycle, max_duty_cycle),
				mapper_(mapper),
				receiver_(receiver),
				motors_armed_(false),
				yaw_(0.0f),
				pitch_(0.0f),
				roll_(0.0f),
				gear_raw_(0.0f),
				user_sw_(USER_SWITCH_2_PIN, DigitalIn::PullNone, DigitalIn::InterruptRising),
				user_led_(USER_LED2_PIN),
				arm_sequence_idle_counter(0),
				gear_alive_(false)
{
	receiver_.channel(mapper_.channel_no(RC_CHANNEL_YAW))->decoder().callback_on_change_only(false);
	user_sw_.callback(this, &RcValueConverter::toggle_motors_armed);
}

void RcValueConverter::toggle_motors_armed()
{
	user_led_.toggle();
	motors_armed_ = user_led_.read();
}

#define CENTER_RADIUS 0.015

void RcValueConverter::update()
{
	float yaw = get_value_as_float(mapper_.channel_no(RC_CHANNEL_YAW), 0.5f);
	float pitch = get_value_as_float(mapper_.channel_no(RC_CHANNEL_PITCH), 0.5f);
	float roll = get_value_as_float(mapper_.channel_no(RC_CHANNEL_ROLL), 0.5f);
	float throttle = get_value_as_float(mapper_.channel_no(RC_CHANNEL_THROTTLE), 0.0f);
	gear_alive_ = receiver_.channel(mapper_.channel_no(RC_CHANNEL_ARM_MOTOR))->is_live();
	gear_raw_ = get_value_as_float(mapper_.channel_no(RC_CHANNEL_ARM_MOTOR), 0.0f);

	throttle_ = Throttle(throttle);
	if ((fabs(yaw - 0.5f) < CENTER_RADIUS)) {
		yaw = 0.5;
	}

	if ((fabs(pitch - 0.5f) < CENTER_RADIUS)) {
		pitch = 0.5;
	}

	if ((fabs(roll - 0.5f) < CENTER_RADIUS)) {
		roll = 0.5;
	}

	yaw_ = -1 * ((yaw - 0.5) * MAX_EULER_FROM_RC * 2);
	pitch_ = (pitch - 0.5) * MAX_EULER_FROM_RC;
	roll_ = (roll - 0.5) * MAX_EULER_FROM_RC;

	if ((yaw_ > 0.7 && ((float)throttle_) < 0.1f && pitch_ < -0.3 && roll_ > 0.3) ||
		(yaw_ <  -0.7 && ((float)throttle_) < 0.1f && pitch_ < -0.3 && roll_ < -0.3))
	{
		if (arm_sequence_idle_counter >= max_arm_sequence_idle_counter) {
			arm_sequence_idle_counter = 0;
			toggle_motors_armed();
		}
	} else {
		if (arm_sequence_idle_counter < max_arm_sequence_idle_counter)
			++arm_sequence_idle_counter;
	}
}

float RcValueConverter::get_value_as_float(uint32_t channelno, float default_value)
{
	if (!receiver_.channel(channelno)->is_live()) {
		return default_value;
	}
	TimeSpan duty_cycle = receiver_.channel(channelno)->decoder().duty_cycle();
	return pwm_converter_.to_float(duty_cycle);
}

Throttle RcValueConverter::base_throttle() const {
	return throttle_;
}

bool RcValueConverter::motors_armed() const
{
	return motors_armed_;
}

float RcValueConverter::get_yaw() const
{
	return yaw_;
}

float RcValueConverter::get_pitch() const
{
	return pitch_;
}

float RcValueConverter::get_roll() const
{
	return roll_;
}

float RcValueConverter::get_gear() const
{
	return gear_raw_;
}
