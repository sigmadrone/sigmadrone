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

#ifndef SERVOCONTROLLER_H_
#define SERVOCONTROLLER_H_

#include "rcdefs.h"
#include "units.h"

class ServoController {
public:
	ServoController(const std::vector<PwmTxConsts>& pwmChannels,
			const Frequency& pwm_rate,
			const TimeSpan& max_pulse = TimeSpan::from_microseconds(2200));
	~ServoController();

	/**
	 *  Start pulse generation, i.e. PWM output and arm the motors. The function can
	 *  be called multiple times, but subsequent calls will not take effect, unless
	 *  there was an intervening call to stop()
	 */
	void start(const Frequency& pwm_rate = Frequency::from_hertz(0),
			const TimeSpan& arm_pulse = TimeSpan::from_milliseconds(1));

	/**
	 * Stop pulse generation
	 */
	void stop();

	/**
	 * Returns the number of channels
	 */
	size_t channel_count() const;

	/**
	 * Get the rate of the generated PWM pulses
	 */
	Frequency get_pwm_rate();

	/**
	 * Get the PWM period
	 */
	TimeSpan get_pwm_period();

	/**
	 * Get the current pulse from the hardware
	 */
	TimeSpan get_pwm_pulse(uint32_t channel);

	/**
	 * Set pulse on the specified channel
	 */
	void set_pwm_pulse(uint32_t channel, const TimeSpan& pulse);

	/**
	 * Checks if PWM encoders have been started.
	 */
	bool is_started();

private:
	void arm_motors(const TimeSpan& arm_pulse = TimeSpan::from_milliseconds(1));

	class PwmEncoder* get_encoder_from_channel_no(uint32_t channelno);
	uint32_t get_encoder_channel_from_channel_no(uint32_t channelno);

private:
	std::vector<class PwmEncoder*> encoders_;
	Frequency pwm_rate_;
	TimeSpan max_pulse_;
	bool started_;
};

#endif /* SERVOCONTROLLER_H_ */
