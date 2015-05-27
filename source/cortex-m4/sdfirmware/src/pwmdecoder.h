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
	/** Constructs and initializes an instance of PwmDecoder
	 *
	 *  @param timer_id ID of the timer to be used
	 *  @param input_pin Name of the input pin
	 *  @param max_period The maximum period that can be decoded
	 *  @param callback Callback to be invoked when a period is decoded
	 *  @param callback_on_change_only Controls whether the callback will be
	 *  invoked only when the duty cycle changes
	 */
	PwmDecoder(HwTimer::Id timer_id,
			PinName input_pin,
			const TimeSpan& max_period = TimeSpan::from_milliseconds(20),
			const FunctionPointer callback = FunctionPointer(),
			bool callback_on_change_only = false);

	/** Constructs and initializes an instance of PwmDecoder
	 *
	 *  @param timer_id ID of the timer to be used
	 *  @param gpio GPIOPin initialized with the input pin to be used
	 *  @param max_period The maximum period that can be decoded
	 *  @param callback Callback to be invoked when a period is decoded
	 *  @param callback_on_change_only Controls whether the callback will be
	 *  invoked only when the duty cycle changes
	 */
	PwmDecoder(HwTimer::Id timer_id,
			const GPIOPin& gpio,
			const TimeSpan& max_period = TimeSpan::from_milliseconds(20),
			const FunctionPointer callback = FunctionPointer(),
			bool callback_on_change_only = false);

	/** Stops and destructs the decoder instance
	 *
	 */
	~PwmDecoder() { stop(); }

	/** Starts the decoder.
	 *
	 *  @returns true - success, false - failure
	 */
	bool start();

	/** Stops the decoder
	 *
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
	 *
	 *  @returns Current duty_cycle, the value is in the range [0..1].
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
