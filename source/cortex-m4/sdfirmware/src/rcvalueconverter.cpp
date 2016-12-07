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

static const float MAX_EULER_FROM_RC = M_PI / 4.0;

RcValueConverter::RcValueConverter(
		const RcChannelMapper& mapper,
		RcReceiver& receiver,
		float scale_factor,
		const TimeSpan& min_duty_cycle,
		const TimeSpan& max_duty_cycle) :
			    pwm_converter_(min_duty_cycle, max_duty_cycle),
				target_twist_(1,0,0,0),
				mapper_(mapper),
				receiver_(receiver),
				scale_factor_(scale_factor),
				last_gear_(0.0),
				motors_armed_(false),
				prev_motors_armed_(false),
				yaw_(0.0f),
				pitch_(0.0f),
				roll_(0.0f),
				pitch_bias_(0.0),
				roll_bias_(0.0)
{
	receiver_.channel(mapper_.channel_no(RC_CHANNEL_YAW))->decoder().callback_on_change_only(false);
}

void RcValueConverter::update()
{
	float yaw = get_value_as_float(mapper_.channel_no(RC_CHANNEL_YAW), 0.5f);
	float pitch = get_value_as_float(mapper_.channel_no(RC_CHANNEL_PITCH), 0.5f);
	float roll = get_value_as_float(mapper_.channel_no(RC_CHANNEL_ROLL), 0.5f);
	float throttle = get_value_as_float(mapper_.channel_no(RC_CHANNEL_THROTTLE), 0.0f);
	float gear = (get_value_as_float(mapper_.channel_no(RC_CHANNEL_ARM_MOTOR), 0.0f) > 0.5f) ? 1.0f : 0.0f;

	throttle_ = Throttle(throttle * scale_factor_);
	if ((fabs(yaw - 0.5f) < 0.0225)) {
		yaw = 0.5;
	}

	if ((fabs(pitch - 0.5f) < 0.0225)) {
		pitch = 0.5;
	}

	if ((fabs(roll - 0.5f) < 0.0225)) {
		roll = 0.5;
	}

	yaw_ = -1 * ((yaw - 0.5) * MAX_EULER_FROM_RC * scale_factor_ * 2);
	TimeSpan dt = receiver_.channel(mapper_.channel_no(RC_CHANNEL_YAW))->decoder().decoded_period();
	Vector3f ang_vel(0.0f, 0.0f, yaw_);
	target_twist_ *= QuaternionF::fromAngularVelocity(ang_vel, dt.seconds_float());

	pitch_ = (pitch - 0.5) * MAX_EULER_FROM_RC * scale_factor_  - pitch_bias_;
	roll_ = (roll - 0.5) * MAX_EULER_FROM_RC * scale_factor_ - roll_bias_;
	target_swing_ = QuaternionF::fromAngularVelocity(Vector3f(roll_, pitch_, 0), 1.0);

	/*
	 * Note: on Spektrum DX8 by _default_ GEAR switch set to 0 results in max pulse and
	 * GEAR set to 1 results in min pulse. This settings of course can be reversed, but
	 * we'd rather work with the defaults. Here we will assume that gear set to 1 means
	 * "landed" and gear set to 0 "prepare for take off", i.e. motors armed.
	 */
	if (receiver_.channel(RC_CHANNEL_ARM_MOTOR)->is_live() && last_gear_ != gear) {
		prev_motors_armed_ = motors_armed_;
		motors_armed_ = (gear > 0.5) ? true : false;
	}
	last_gear_ = gear;
}

void RcValueConverter::reset_twist_quaternion(const QuaternionF& current_q)
{
	target_twist_ = QuaternionF(current_q.w, 0, 0, current_q.z).normalize();
}

float RcValueConverter::get_value_as_float(uint32_t channelno, float default_value)
{
	if (!receiver_.channel(channelno)->is_live()) {
		return default_value;
	}
	TimeSpan duty_cycle = receiver_.channel(channelno)->decoder().duty_cycle();
	return pwm_converter_.to_float(duty_cycle);
}

QuaternionF RcValueConverter::target_quaternion() const {
	return 	target_swing_ * target_twist_;
}

QuaternionF RcValueConverter::target_twist() const
{
	return target_twist_;
}

QuaternionF RcValueConverter::target_swing() const
{
	return target_swing_;
}

Throttle RcValueConverter::base_throttle() const {
	return throttle_;
}

bool RcValueConverter::motors_armed() const
{
	return motors_armed_;
}

bool RcValueConverter::previous_motors_armed() const
{
	return prev_motors_armed_;
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
