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

#ifndef PWMDECODER_H_
#define PWMDECODER_H_

#include "hwtimer.h"
#include "gpiopin.h"
#include "timestamp.h"

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
			bool callback_on_change_only = false,
			const TimeSpan& epsilon = TimeSpan::from_seconds(0));

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
			bool callback_on_change_only = false,
			const TimeSpan& epsilon = TimeSpan::from_seconds(0));

	/** Stops and destructs the decoder instance
	 *
	 */
	~PwmDecoder() { stop(); }

	/** Starts the decoder.
	 *
	 * @param period_channel_no - timer channel no. that is active on the rising edge.
	 *  The capture register from this channel gives us the period.
	 * @param duty_cycle_channel_no - timer channel no. that is active on the falling edge.
	 *  The capture register from this channel gives us the duty cycle.
	 * @param input_trugger - trigger for the slave controller - TIM_TS_TI1FP1 or TIM_TS_TI2FP2
	 *
	 * @returns true - success, false - failure
	 */
	bool start(
			uint32_t period_channel_no,
			uint32_t duty_cycle_channel_no,
			uint32_t input_trigger);

	inline bool start_on_ch1_ch2() { return start(1,2,TIM_TS_TI1FP1); }
	inline bool start_on_ch2_ch1() { return start(2,1,TIM_TS_TI2FP2); }

	/** Stops the decoder
	 *
	 */
	void stop();

	/** Returns the decoded period
	 *  @returns Returns the decoded period as TimeSpan
	 */
	TimeSpan decoded_period() const { return decoded_period_; }

	/** Returns the duty cycle
	 *  @returns Returns TimeSpan representing the duty cycle
	 */
	TimeSpan duty_cycle() const { return duty_cycle_; }

	/** Returns the duty cycle as a relative value
	 *
	 *  @returns Current duty_cycle, the value is in the range [0..1].
	 */
	float duty_cycle_rel() const {
		return (float)duty_cycle_.unit() / (float)decoded_period_.unit();
	}

	TimeStamp last_capture_ts() const { return last_capture_ts_; }

	void callback_on_change_only(bool on_change_only) {
		callback_on_change_only_ = on_change_only;
	}

private:
	static Frequency timer_clock_from_pwm_period(const TimeSpan& max_period);
	void capture_callback();
	bool ts_within_range(const TimeSpan& ts1, const TimeSpan& ts2);

	PwmDecoder(const PwmDecoder&);
	const PwmDecoder& operator=(const PwmDecoder&);

private:
	HwTimer timer_;
	GPIOPin gpio_;
	TimeSpan decoded_period_;
	TimeSpan duty_cycle_;
	TimeSpan max_period_;
	FunctionPointer callback_;
	uint32_t period_channel_;
	uint32_t duty_cycle_channel_;
	bool callback_on_change_only_;
	TimeSpan epsilon_;
	TimeStamp last_capture_ts_;
};

#endif /* PWMDECODER_H_ */
