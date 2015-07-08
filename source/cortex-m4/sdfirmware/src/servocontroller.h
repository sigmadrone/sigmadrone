/*
 * servocontroller.h
 *
 *  Created on: Jul 6, 2015
 *      Author: svassilev
 */

#ifndef SERVOCONTROLLER_H_
#define SERVOCONTROLLER_H_

#include "rcdefs.h"
#include "units.h"

class ServoController {
public:
	ServoController(const std::vector<PwmTxConsts>& pwmChannels,
			const Frequency& pwm_rate,
			const TimeSpan& max_pulse = TimeSpan::from_microseconds(2100));
	~ServoController();

	/**
	 *  Start pulse generation, i.e. PWM output
	 */
	void start(const Frequency& pwm_rate = Frequency::from_hertz(0));

	/**
	 * Stop pulse generation
	 */
	void stop();

	/**
	 * Returns the number of channels
	 */
	size_t channel_count() const;

	/**
	 * Arms the motors by setting 1 mS pulse. PWM output must be enabled for
	 * this call to take effect.
	 */
	void arm_motors(const TimeSpan& pulse = TimeSpan::from_milliseconds(1));

	/**
	 * Disarms the motors by setting 0 ms pulse.
	 */
	void disarm_motors();

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

private:
	class PwmEncoder* get_encoder_from_channel_no(uint32_t channelno);
	uint32_t get_encoder_channel_from_channel_no(uint32_t channelno);

private:
	std::vector<class PwmEncoder*> encoders_;
	Frequency pwm_rate_;
	TimeSpan max_pulse_;
};

#endif /* SERVOCONTROLLER_H_ */
