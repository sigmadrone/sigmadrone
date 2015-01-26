/*
 * pwmdecoder.h
 *
 *  Created on: Jan 21, 2015
 *      Author: svetlio
 */

#ifndef PWMDECODER_H_
#define PWMDECODER_H_

#include "hwtimer.h"
#include "gpiopin.h"

class PwmDecoder {
public:
	PwmDecoder(HwTimer::Id timer_id,
			PinName pin,
			const TimeSpan& max_period = TimeSpan::from_milliseconds(20),
			const FunctionPointer callback = FunctionPointer(),
			bool callback_on_change_only = false);
	PwmDecoder(HwTimer::Id timer_id,
			const GPIOPin& gpio,
			const TimeSpan& max_period = TimeSpan::from_milliseconds(20),
			const FunctionPointer callback = FunctionPointer(),
			bool callback_on_change_only = false);
	~PwmDecoder() { stop(); }

	/** Starts the decoding
	 */
	bool start();

	/** Stops the decoding
	 */
	void stop();

	/** Returns the decoded period
	 *  @returns Returns the decoded period as TimeSpan
	 */
	TimeSpan decoded_period() { return decoded_period_; }

	/** Returns the duty cycle
	 *  @returns Returns TimeSpan representing the duty cycle
	 */
	TimeSpan duty_cycle() { return duty_cycle_; }

	/** Returns the duty cycle as a relative value
	 *  @returns The returned value is in the range [0..1]
	 */
	float duty_cycle_rel() {
		return (float)duty_cycle_.unit() / (float)decoded_period_.unit();
	}

private:
	static Frequency timer_clock_from_pwm_period(const TimeSpan& max_period);
	void capture_callback();

private:
	HwTimer timer_;
	GPIOPin gpio_;
	TimeSpan decoded_period_;
	TimeSpan duty_cycle_;
	TimeSpan max_period_;
	FunctionPointer callback_;
	bool callback_on_change_only_;
};

#endif /* PWMDECODER_H_ */
