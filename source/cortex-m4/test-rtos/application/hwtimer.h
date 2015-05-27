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
		MODE_NOT_INITIALIZED,
		MODE_BASIC,
		MODE_PWM_DECODE,
		MODE_PWM_ENCODE
	};

	enum Id {
		TIMER_INVALID, TIMER_1, TIMER_2, TIMER_3, TIMER_4, TIMER_5, TIMER_6, TIMER_7, TIMER_8,
		TIMER_9, TIMER_10, TIMER_11, TIMER_12, TIMER_13, TIMER_14, TIMER_LAST
	};

	static const uint32_t INVALID_CHANNEL_NO = 0xffffffff;

	/** Constructs and initializes a timer instance
	 *  @param timer_id Id of the timer to be used
	 *  @param period Desired timer period
	 *  @param timer_clock At what frequency the timer will be clocked
	 *  @param interrupt_callback Callback to be invoked when timer event happens
	 */
	HwTimer(Id timer_id,
			const TimeSpan& period,
			const Frequency& timer_clock = Frequency::from_kilohertz(10),
			const FunctionPointer& interruptCallback = FunctionPointer());

	/** Initializes the timer to be used as a simple counter or in slave mode
	 *  The period will be set to the max value
	 *  @param timer_id Id of the timer to be used
	 *  @param timer_clock At what frequency the timer will be clocked
	 *  @param interrupt_callback Callback to be invoked when timer event happens
	 */
	HwTimer(Id timer_id,
			const Frequency& timer_clock,
			const FunctionPointer& interrupt_callback = FunctionPointer());
	virtual ~HwTimer();

	/** Starts the timer in basic mode. The routine will implicitly stop the timer if
	 *  it is currently ticking. Upon successful execution the timer will be in
	 *  MODE_PWM_BASIC
	 *  @returns true - success, false - failure
	 */
	bool start();

	/** Starts the timer in PWM decode/input/capture mode. Implicitly stop the timer if
	 * it is currently ticking. Upon successful execution the timer will be in
	 * MODE_PWM_DECODE
	 *
	 * @returns true - success, false - failure
	 */
	bool start_pwm_decode_mode();

	/** Starts the timer in PWM encode mode. Implicitly stops the timer if
	 *  it is currently ticking. Upon successful execution the timer will be in
	 *  MODE_PWM_ENCODE
	 *
	 *  @returns true - success, false - failure
	 */
	bool start_pwm_encode_mode(const std::vector<uint32_t>& channels);

	/** Stops the timer. After this call the timer will be in MODE_NOT_INITIALIZED
	 */
	void stop();

	/** Sets new timer period value. Note: do not call this routine if the timer is started
	 *  @param period The new value will take effect after the timer was restarted.
	 */
	void set_period(const TimeSpan& period) { period_ = period; }

	/** Sets the desired timer clock. Note: do not call this routine if the timer is started
	 *  @param timer_clock The new value will take effect after the timer was restarted.
	 */
	void set_timer_clock(const Frequency& timer_clock);

	/** Returns the elapsed interval since the timer was zeroed out
	 *  @returns Elapsed time interval
	 */
	TimeSpan time_elapsed() const;

	/** Returns the current timer counter value
	 *  @returns Timer counter value
	 */
	uint32_t current_counter_value() const;

	/** Returns the configured timer period
	 *  @returns Timer period
	 */
	inline const TimeSpan& period() const { return period_; }

	/** Returns the timer Id
	 *  @returns Timer Id
	 */
	inline Id timerid() const { return timer_id_; }

	/** Returns the current mode of operation
	 *  @returns Mode of operation
	 */
	inline Mode mode() const { return mode_; }

	/** Returns the timer input clock
	 *  @returns The timer input clock, prior to pre-scaler and divider
	 */
	inline Frequency input_clock() { return get_timx_input_clock(timer_id_); }

	/** Returns the captured value from the specified channel
	 *  @param channel Channel number to be queried, value must be in the range [1..4]
	 *  @returns The current captured value
	 */
	uint32_t read_captured_value(uint32_t channel);

	/** Returns the actual timer clock
	 *  @returns Returns the clock at which the timer ticks.
	 */
	Frequency timer_clock() const { return timer_clock_; }

	/** Sets the duty cycle on the specified channel. Timer must be configured
	 *  in MODE_PWM_ENCODE
	 *  @param channel_no Channel number [1..4]
	 *  @param pulse_period Lenght of the duty cycle, must be LEQ than the timer period
	 */
	bool set_pwm_duty_cycle(uint32_t channel_no, const TimeSpan& pulse_period);

	inline uint64_t period_elapsed_count() const { return period_elapsed_cnt_; }

	/** Returns the timer input clock for any of the timers
	 *  @param timer_id Timer ID
	 *  @returns Timer input clock, prior to pre-scaler and divider
	 */
	static Frequency get_timx_input_clock(Id timer_id);

	/** Returns alternative function number for the specified timer id
	 *  @param timer_id Timer ID
	 *  @returns The corresponding alternative function number
	 */
	static uint32_t get_gpio_altfunc(Id timer_id);

private:
	friend void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *handle);
	friend void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *handle);
	void on_period_elapsed();
	TIM_HandleTypeDef* init_handle();
	static uint32_t get_timx_channel(uint32_t channel_no);

private:
	Id timer_id_;
	TimeSpan period_;
	Frequency timer_clock_;
	FunctionPointer callback_;
	Mode mode_;
	std::vector<uint32_t> channels_;
	volatile uint64_t period_elapsed_cnt_;
};

#endif /* TEST_PRINTF_HWTIMER_H_ */
