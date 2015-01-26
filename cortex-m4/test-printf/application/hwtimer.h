/*
 * HwTimer.h
 *
 *  Created on: Jan 13, 2015
 *      Author: svetlio
 */

#ifndef TEST_PRINTF_HWTIMER_H_
#define TEST_PRINTF_HWTIMER_H_

#include "stm32f4xx_hal.h"
#include "functionptr.h"
#include <cassert>
#include <vector>
#include "units.h"
#include "gpiopin.h"

class HwTimer {
public:

	enum Mode {
		MODE_UNKNOWN,
		MODE_BASIC,
		MODE_PWM_DECODE,
		MODE_PWM_ENCODE
	};

	enum Id {
		TIMER_INVALID, TIMER_1, TIMER_2, TIMER_3, TIMER_4, TIMER_5, TIMER_6, TIMER_7, TIMER_8,
		TIMER_9, TIMER_10, TIMER_11, TIMER_12, TIMER_13, TIMER_14, TIMER_LAST
	};

	static const uint32_t INVALID_CHANNEL_NO = 0xffffffff;

	HwTimer(Id timer_id,
			const TimeSpan& period,
			const Frequency& timerClock = Frequency::from_kilohertz(10),
			const FunctionPointer& interruptCallback = FunctionPointer());

	/*
	 * Initializes the timer to be used as a simple counter or in slave mode
	 * The period will be set to the max value
	 */
	HwTimer(Id timer_id,
			const Frequency& timer_clock,
			const FunctionPointer& interruptCallback = FunctionPointer());
	virtual ~HwTimer();

	/*
	 * Starts the timer in basic mode. The routine will implicitly stop the timer if
	 * it is currently ticking
	 */
	bool start();

	/*
	 * Starts the timer in PWM decode/input/capture mode. Implicitly stop the timer if
	 * it is currently ticking
	 *
	 */
	bool start_pwm_decode_mode();

	/*
	 * Starts the timer in PWM encode mode. Implicitly stop the timer if
	 * it is currently ticking
	 */
	bool start_pwm_encode_mode(const std::vector<uint32_t>& channels);

	/*
	 * Stops the timer
	 */
	void stop();

	/*
	 * The new values from set_period/timer_clock will take effect after
	 * the timer was restarted
	 */
	void set_period(const TimeSpan& period) { period_ = period; }

	/*
	 * Sets the desired timer clock. The timer must be stopped
	 */
	void set_timer_clock(const Frequency& timer_clock);

	/*
	 * Returns the elapsed interval since the timer was zeroed out
	 */
	TimeSpan time_elapsed() const;

	/*
	 * Returns the current timer counter value
	 */
	uint32_t current_counter_value() const;

	/*
	 * Returns the configured timer period
	 */
	const TimeSpan& period() const { return period_; }

	Id timerid() const { return timer_id_; }

	Mode mode() { return mode_; }

	/*
	 * Returns the timer input clock
	 */
	Frequency input_clock() { return get_timx_input_clock(timer_id_); }

	/*
	 * Returns the captured value from the specified channel
	 */
	uint32_t read_captured_value(uint32_t channel);

	Frequency timer_clock() { return timer_clock_; }

	/*
	 * Sets the duty cycle on the specified channel. Timer must be configured
	 * in MODE_PWM_ENCODE
	 */
	bool set_pwm_duty_cycle(uint32_t channel_no, const TimeSpan& pulse_period);

	/*
	 * Returns the timer input clock for any of the timers
	 */
	static Frequency get_timx_input_clock(Id timer_id);

	static uint32_t get_gpio_altfunc(Id timer_id);

	static uint32_t get_timx_channel(uint32_t channel_no);

private:
	friend void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *handle);
	friend void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *handle);
	void on_period_elapsed();
	TIM_HandleTypeDef* init_handle();

	Id timer_id_;
	TimeSpan period_;
	Frequency timer_clock_;
	FunctionPointer callback_;
	Mode mode_;
	std::vector<uint32_t> channels_;
};

#endif /* TEST_PRINTF_HWTIMER_H_ */
