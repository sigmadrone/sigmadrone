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
			const Frequency& timerClock = Frequency::from_hertz(0),
			const FunctionPointer& interruptCallback = FunctionPointer());
	virtual ~HwTimer();
	bool start();
	void stop();
	const TimeSpan& current_value() const;
	const TimeSpan& period() const { return period_; }
	void set_period(const TimeSpan& period) { period_ = period; }
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

	static size_t interrupt_handler_count_;
};

#endif /* TEST_PRINTF_HWTIMER_H_ */
