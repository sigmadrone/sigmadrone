/*
 * HwTimer.cpp
 *
 *  Created on: Jan 13, 2015
 *      Author: svetlio
 */

#include "hwtimer.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f429xx.h"
#include "diag/Trace.h"

struct HwTimerStatic {
	TIM_HandleTypeDef handle_;
	HwTimer* hwtimer_;
	uint32_t busnum_; // what bus the timer is connected to - 1 or 2
	uint32_t counter_resolution_;
};

#define array_size(x) (sizeof(x) / sizeof((x)[0]))

static HwTimerStatic all_timers_[] = {
		{ {0}, 0, 0}, // invalid entry
		{ {TIM1},  0, 2, 0xffff }, { {TIM2},  0, 1, 0xffffffff }, { {TIM3}, 0, 1, 0xffff },
		{ {TIM4},  0, 1, 0xffff }, { {TIM5},  0, 1, 0xffffffff }, { {TIM6}, 0, 1, 0xffff },
		{ {TIM7},  0, 1, 0xffff }, { {TIM8},  0, 2, 0xffff },
		{ {TIM9},  0, 2, 0xffff }, { {TIM10}, 0, 2, 0xffff },
		{ {TIM11}, 0, 2, 0xffff }, { {TIM12}, 0, 1, 0xffff },
		{ {TIM13}, 0, 1, 0xffff }, { {TIM14}, 0, 1, 0xffff }
};

static bool is_valid_timer_id(uint32_t timer_id) {
	return timer_id > 0 && timer_id < array_size(all_timers_);
}

static TIM_HandleTypeDef* timer_handle(uint32_t timer_id) {
	return is_valid_timer_id(timer_id) ? &all_timers_[timer_id].handle_ : 0;
}

static HwTimer* from_handle_to_timer(TIM_HandleTypeDef* handle) {
	assert(handle);
	assert((size_t)(reinterpret_cast<HwTimerStatic*>(handle) - all_timers_) > 0 &&
			(size_t)(reinterpret_cast<HwTimerStatic*>(handle) - all_timers_) < array_size(all_timers_));
	assert(reinterpret_cast<HwTimerStatic*>(handle)->hwtimer_);
	return reinterpret_cast<HwTimerStatic*>(handle)->hwtimer_;
}

static Frequency get_apb_bus_freq(uint32_t busnum) {
	if (1 == busnum) {
		return Frequency::from_hertz(HAL_RCC_GetPCLK1Freq());
	}
	assert(2 == busnum);
	return Frequency::from_hertz(HAL_RCC_GetPCLK2Freq());
}

Frequency HwTimer::get_timx_input_clock(uint32_t timer_id)
{
	assert(is_valid_timer_id(timer_id));
	uint32_t apbusnum = all_timers_[timer_id].busnum_;
	assert(1 == apbusnum || 2 == apbusnum);

	Frequency bus_freq = get_apb_bus_freq(apbusnum);
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	uint32_t fLatency;
	HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &fLatency);
	RCC_PeriphCLKInitTypeDef  periphClkInit;
	HAL_RCCEx_GetPeriphCLKConfig(&periphClkInit);
	uint32_t apb_prescaler = (apbusnum == 1) ? RCC_ClkInitStruct.APB1CLKDivider :
			RCC_ClkInitStruct.APB2CLKDivider;

	/* Bit 24 TIMPRE: Timers clocks prescalers selection
	 * This bit is set and reset by software to control the clock frequency of all
	 * the timers connected to APB1 and APB2 domain.
	 * 0: If the APB prescaler (PPRE1, PPRE2 in the RCC_CFGR register) is configured to a
	 *    division factor of 1, TIMxCLK = PCLKx. Otherwise, the timer clock frequencies are set to
	 *    twice to the frequency of the APB domain to which the timers are connected:
	 *    TIMxCLK = 2xPCLKx.
	 * 1: If the APB prescaler (PPRE1, PPRE2 in the RCC_CFGR register) is configured to a
	 *    division factor of 1, 2 or 4, TIMxCLK = HCLK. Otherwise, the timer clock frequencies are set
	 *    to four times to the frequency of the APB domain to which the timers are connected:
	 *    TIMxCLK = 4xPCLKx.
	 */
	if (periphClkInit.TIMPresSelection == RCC_TIMPRES_DESACTIVATED) {
		if (apb_prescaler == RCC_HCLK_DIV1) {
			return bus_freq;
		}
		return bus_freq * 2;
	}

	assert(periphClkInit.TIMPresSelection == RCC_TIMPRES_ACTIVATED);
	if (apb_prescaler == RCC_HCLK_DIV1 || apb_prescaler == RCC_HCLK_DIV2 ||
			apb_prescaler == RCC_HCLK_DIV4) {
		return Frequency::from_hertz(HAL_RCC_GetSysClockFreq());
	}
	return bus_freq * 4;
}

HwTimer::HwTimer(uint32_t timer_id,
		const TimeSpan& period,
		const Frequency& timer_clock,
		const FunctionPointer& interrupt_callback) :
		timer_id_(0), period_(period), callback_(interrupt_callback)
{
	if (is_valid_timer_id(timer_id) && !all_timers_[timer_id].hwtimer_) {
		all_timers_[timer_id].hwtimer_ = this;
		timer_id_ = timer_id;
	}
	set_timer_clock(timer_clock);
}

HwTimer::~HwTimer() {
	stop();
	if (is_valid_timer_id(timer_id_) && all_timers_[timer_id_].hwtimer_ == this) {
		all_timers_[timer_id_].hwtimer_ = 0;
	}
}

bool HwTimer::start() {
	TIM_HandleTypeDef* handle = timer_handle(timer_id_);
	if (0 == handle) {
		return false;
	}
	assert(handle->Instance); // TIMx should be initialized

	stop();

	/*
	 * Initialize TIMX peripheral as follows:
	 * + Period = period - 1
	 * + Prescaler = input_clock/timer_clock - 1
	 * + ClockDivision = 0
	 * + Counter direction = Up
	 */

	uint64_t timer_counter = period_.nanoseconds() / timer_clock_.period().nanoseconds() - 1;
	if (timer_counter > all_timers_[timer_id_].counter_resolution_) {
		// since we will overflow the timer counter, we have to readjust the timer clock
		timer_counter = all_timers_[timer_id_].counter_resolution_;
		timer_clock_ = Frequency::from_timespan(period_ / (timer_counter + 1));
	}

	Frequency input_clock = get_timx_input_clock(timer_id_);
	uint32_t prescaler = input_clock.hertz() / timer_clock_.hertz() - 1;

	handle->Init.Period = timer_counter;
	handle->Init.Prescaler = prescaler;
	handle->Init.ClockDivision = 0;
	handle->Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_Base_Init(handle) != HAL_OK) {
		trace_printf("HAL_TIM_Base_Init failed\n");
		return false;
	}

	/*
	 * Setup the interrupt handler
	 */
	if(HAL_TIM_Base_Start_IT(handle) != HAL_OK) {
		/* Starting Error */
		trace_printf("HAL_TIM_Base_Start_IT failed\n");
		return false;
	}

	return true;
}

void HwTimer::period_elapsed() {
	if (callback_.get_function()) {
		callback_.call();
	}
}

void HwTimer::stop() {
	TIM_HandleTypeDef* handle = timer_handle(timer_id_);
	if (0 != handle) {
		HAL_TIM_Base_Stop_IT(handle);
		HAL_TIM_Base_DeInit(handle);
	}
}

void HwTimer::set_timer_clock(const Frequency& timer_clock) {
	if (timer_clock.unit() > 0) {
		timer_clock_ = timer_clock;
	} else {
		timer_clock_ = Frequency::from_kilohertz(10);
	}
}

/*
 * Global routines invoked by the HAL
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
	/* Here we must perform the following
	 * 1 - Enable peripherals and GPIO Clocks
	 *  	TIMx Peripheral clock enable
	 *  	TIMx_CLK_ENABLE();
	 * 2 - Configure the NVIC for TIMx
	 *   2.1 Set the TIMx priority
	 *   	HAL_NVIC_SetPriority(TIMx_IRQn, 0, 1);
	 *   2.2 Enable the TIMx global Interrupt
	 *   	HAL_NVIC_EnableIRQ(TIMx_IRQn);
	 */

	switch (from_handle_to_timer(htim)->timerid()) {
	/* TODO:
	case 1:
		__TIM1_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM1_IRQn, 15, 1);
		HAL_NVIC_EnableIRQ(TIM1_IRQn);
		break;
		*/
	case 2:
		__TIM2_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM2_IRQn, 15, 1);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
		break;
	case 3:
		__TIM3_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM3_IRQn, 15, 1);
		HAL_NVIC_EnableIRQ(TIM3_IRQn);
		break;
	case 4:
		__TIM4_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM4_IRQn, 15, 1);
		HAL_NVIC_EnableIRQ(TIM4_IRQn);
		break;
	case 5:
		__TIM5_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM5_IRQn, 15, 1);
		HAL_NVIC_EnableIRQ(TIM5_IRQn);
		break;
	case 7:
		__TIM7_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM7_IRQn, 15, 1);
		HAL_NVIC_EnableIRQ(TIM7_IRQn);
		break;
	}
}

extern "C" void TIM1_IRQHandler(void) {
	HAL_TIM_IRQHandler(&all_timers_[1].handle_);
}

extern "C" void TIM2_IRQHandler(void) {
	HAL_TIM_IRQHandler(&all_timers_[2].handle_);
}

extern "C" void TIM3_IRQHandler(void) {
	HAL_TIM_IRQHandler(&all_timers_[3].handle_);
}

extern "C" void TIM4_IRQHandler(void) {
	HAL_TIM_IRQHandler(&all_timers_[4].handle_);
}

extern "C" void TIM5_IRQHandler(void) {
	HAL_TIM_IRQHandler(&all_timers_[5].handle_);
}

extern "C" void TIM7_IRQHandler(void) {
	HAL_TIM_IRQHandler(&all_timers_[7].handle_);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *handle) {
	HwTimer* timer = from_handle_to_timer(handle);
	timer->period_elapsed();
}
