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
#include "pwmpulse.h"
#include "digitalin.h"
#include "digitalout.h"

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
	float get_gear() const;
	void reset_twist_quaternion(const QuaternionF&);
	void interrupt_user_switch();

private:
	float get_value_as_float(uint32_t channelno, float default_value);
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
	float pitch_bias_;
	float roll_bias_;
	float gear_raw_;
	DigitalIn user_sw_;
	DigitalOut user_led_;

public:
	bool gear_alive_;

};

#endif /* RCVALUECONVERTER_H_ */
