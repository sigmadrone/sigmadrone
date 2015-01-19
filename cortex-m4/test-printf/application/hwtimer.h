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
#include "units.h"

class HwTimer {
public:
	HwTimer(uint32_t timerId,
			const TimeSpan& period,
			const Frequency& timerClock = Frequency::from_kilohertz(10),
			const FunctionPointer& interruptCallback = FunctionPointer());
	virtual ~HwTimer();

	/*
	 * Starts the timer. The routine will implicitly stop the timer if it is
	 * currently ticking
	 */
	bool start();

	/*
	 * Stops the timer
	 */
	void stop();

	/*
	 * The new values from set_period/timer_clock will take effect after
	 * the timer was restarted
	 */
	void set_period(const TimeSpan& period) { period_ = period; }
	void set_timer_clock(const Frequency& timer_clock);

	/*
	 * Returns the elapsed interval since the timer was zeroed out
	 */
	const TimeSpan& time_elapsed() const;

	/*
	 * Returns the current timer counter value
	 */
	uint32_t current_counter_value() const;

	/*
	 * Returns the configured timer period
	 */
	const TimeSpan& period() const { return period_; }

	uint32_t timerid() const { return timer_id_; }

	/*
	 * Returns the timer input clock
	 */
	static Frequency get_timx_input_clock(uint32_t timer_id);
private:
	friend void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *handle);
	void period_elapsed();

	uint32_t timer_id_;
	TimeSpan period_;
	Frequency timer_clock_;
	FunctionPointer callback_;
};

#endif /* TEST_PRINTF_HWTIMER_H_ */
