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

static HwTimerStatic all_timers_[HwTimer::TIMER_LAST] = {
		{ {0}, 0, 0}, // invalid entry
		{ {TIM1},  0, 2, 0xffff }, { {TIM2},  0, 1, 0xffffffff }, { {TIM3}, 0, 1, 0xffff },
		{ {TIM4},  0, 1, 0xffff }, { {TIM5},  0, 1, 0xffffffff }, { {TIM6}, 0, 1, 0xffff },
		{ {TIM7},  0, 1, 0xffff }, { {TIM8},  0, 2, 0xffff },
		{ {TIM9},  0, 2, 0xffff }, { {TIM10}, 0, 2, 0xffff },
		{ {TIM11}, 0, 2, 0xffff }, { {TIM12}, 0, 1, 0xffff },
		{ {TIM13}, 0, 1, 0xffff }, { {TIM14}, 0, 1, 0xffff }
};

static bool is_valid_timer_id(HwTimer::Id timer_id) {
	return timer_id > HwTimer::TIMER_INVALID && timer_id < HwTimer::TIMER_LAST;
}

static TIM_HandleTypeDef* timer_handle(HwTimer::Id timer_id) {
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

Frequency HwTimer::get_timx_input_clock(Id timer_id)
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

HwTimer::HwTimer(Id timer_id,
		const TimeSpan& period,
		const Frequency& timer_clock,
		const FunctionPointer& interrupt_callback) :
		timer_id_(TIMER_INVALID), period_(period), callback_(interrupt_callback),
		mode_(HwTimer::MODE_NOT_INITIALIZED),
		period_elapsed_cnt_(0)
{
	if (is_valid_timer_id(timer_id) && !all_timers_[timer_id].hwtimer_) {
		all_timers_[timer_id].hwtimer_ = this;
		timer_id_ = timer_id;
	}
	set_timer_clock(timer_clock);
}

HwTimer::HwTimer(Id timer_id,
		const Frequency& timer_clock,
		const FunctionPointer& interrupt_callback) :
		timer_id_(TIMER_INVALID), period_(), callback_(interrupt_callback),
		mode_(HwTimer::MODE_NOT_INITIALIZED),
		period_elapsed_cnt_(0)
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

TIM_HandleTypeDef* HwTimer::init_handle() {
	TIM_HandleTypeDef* handle = timer_handle(timer_id_);
	if (0 == handle || all_timers_[timer_id_].hwtimer_ != this) {
		return nullptr;
	}

	period_elapsed_cnt_ = 0;

	assert(handle->Instance); // TIMx should be initialized
	/*
	 * Initialize TIMX peripheral as follows:
	 * + Period = period - 1
	 * + Prescaler = input_clock/timer_clock - 1
	 * + ClockDivision = 0
	 * + Counter direction = Up
	 */

	uint64_t timer_counter;
	if (!period_.is_null()) {
		timer_counter = period_.nanoseconds() / timer_clock_.period().nanoseconds() - 1;
		if (timer_counter > all_timers_[timer_id_].counter_resolution_) {
			// since we will overflow the timer counter, we have to readjust the timer clock
			timer_counter = all_timers_[timer_id_].counter_resolution_;
			timer_clock_ = Frequency::from_timespan(period_ / (timer_counter + 1));
		}
	} else {
		timer_counter = all_timers_[timer_id_].counter_resolution_;
	}

	Frequency input_clock = get_timx_input_clock(timer_id_);
	uint32_t prescaler = input_clock.hertz() / timer_clock_.hertz() - 1;

	handle->Init.Period = timer_counter;
	handle->Init.Prescaler = prescaler;
	handle->Init.ClockDivision = 0;
	handle->Init.CounterMode = TIM_COUNTERMODE_UP;
	return handle;
}

bool HwTimer::start() {
	TIM_HandleTypeDef* handle = init_handle();
	if (0 == handle) {
		return false;
	}

	mode_ = MODE_BASIC;
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

bool HwTimer::start_pwm_decode_mode() {
	TIM_HandleTypeDef* handle = init_handle();
	if (0 == handle) {
		return false;
	}

	mode_ = MODE_PWM_DECODE;
	if (HAL_TIM_IC_Init(handle) != HAL_OK) {
		/* Initialization Error */
		trace_printf("HAL_TIM_IC_Init failed\n");
	}

	/* Configure the Input Capture channels */
	TIM_IC_InitTypeDef ic_init = {0};
	ic_init.ICPrescaler = TIM_ICPSC_DIV1;
	ic_init.ICFilter = 0;

	/* Configure the Input Capture of channel 1 */
	ic_init.ICPolarity = TIM_ICPOLARITY_FALLING;
	ic_init.ICSelection = TIM_ICSELECTION_INDIRECTTI;
	if (HAL_TIM_IC_ConfigChannel(handle, &ic_init, TIM_CHANNEL_1) != HAL_OK) {
		/* Initialization Error */
		trace_printf("HAL_TIM_IC_ConfigChannel failed\n");
		return false;
	}

	/* Configure the Input Capture of channel 2 */
	ic_init.ICPolarity = TIM_ICPOLARITY_RISING;
	ic_init.ICSelection = TIM_ICSELECTION_DIRECTTI;
	if (HAL_TIM_IC_ConfigChannel(handle, &ic_init, TIM_CHANNEL_2) != HAL_OK)
	{
		/* Initialization Error */
		trace_printf("HAL_TIM_IC_ConfigChannel failed\n");
		return false;
	}

	/* Configure the slave mode */
	/* Select the slave Mode: Reset Mode */
	/* Slave synchro config */
	TIM_SlaveConfigTypeDef slave_config = {0};
	slave_config.InputTrigger = TIM_TS_TI2FP2;
	slave_config.SlaveMode = TIM_SLAVEMODE_RESET;
	if (HAL_TIM_SlaveConfigSynchronization(handle, &slave_config) != HAL_OK)
	{
		trace_printf("HAL_TIM_SlaveConfigSynchronization failed\n");
		return false;
	}

	/* Start the Input Capture in interrupt mode on channel 2 */
	if (HAL_TIM_IC_Start_IT(handle, TIM_CHANNEL_2) != HAL_OK)
	{
		trace_printf("HAL_TIM_IC_Start_IT failed\n");
		return false;
	}

	/* Start the Input Capture in interrupt mode on channel 1 */
	if (HAL_TIM_IC_Start_IT(handle, TIM_CHANNEL_1) != HAL_OK)
	{
		trace_printf("HAL_TIM_IC_Start_IT failed\n");
		return false;
	}

	return true;
}

bool HwTimer::start_pwm_encode_mode(const std::vector<uint32_t>& channels)
{
	if (0 == channels.size() || channels.size() > 4) {
		return false;
	}

	for (uint32_t i = 0; i < channels_.size(); ++i) {
		if (get_timx_channel(channels[i]) == INVALID_CHANNEL_NO) {
			trace_printf("Invalid channel NO specified %d\n", channels[i]);
			return false;
		}
	}

	TIM_HandleTypeDef* handle = init_handle();
	if (0 == handle) {
		return false;
	}

	if (HAL_TIM_PWM_Init(handle) != HAL_OK) {
		trace_printf("HAL_TIM_PWM_Init failed!\n");
		return false;
	}

	mode_ = MODE_PWM_ENCODE;
	channels_ = channels;

	TIM_OC_InitTypeDef oc_init = {0};
	oc_init.OCMode     = TIM_OCMODE_PWM1;
	oc_init.OCPolarity = TIM_OCPOLARITY_HIGH;
	oc_init.OCFastMode = TIM_OCFAST_DISABLE;
	oc_init.Pulse = 1;
	for (uint32_t i = 0; i < channels_.size(); ++i) {
		if (HAL_TIM_PWM_ConfigChannel(handle, &oc_init, get_timx_channel(channels_[i])) != HAL_OK) {
			trace_printf("HAL_TIM_PWM_ConfigChannel failed\n");
			return false;
		}
	}

	for (uint32_t i = 0; i < channels_.size(); ++i) {
		if (HAL_TIM_PWM_Start(handle, get_timx_channel(channels_[i])) != HAL_OK) {
			trace_printf("HAL_TIM_PWM_Start failed\n");
			return false;
		}
	}

	return true;
}

bool HwTimer::set_pwm_duty_cycle(uint32_t channel_no, const TimeSpan& duty_cycle)
{
	TIM_HandleTypeDef* handle = timer_handle(timer_id_);
	if (!handle) {
		return false;
	}

	if (duty_cycle > period_) {
		return false;
	}

	uint32_t pulse = (duty_cycle * handle->Init.Period) / period_;

	// TODO: what will happen with the current period duty cycle generation?
	switch (channel_no) {
	case 1: handle->Instance->CCR1 = pulse; break;
	case 2: handle->Instance->CCR2 = pulse; break;
	case 3: handle->Instance->CCR3 = pulse; break;
	case 4: handle->Instance->CCR4 = pulse; break;
	default: return false;
	}

	return true;
}

void HwTimer::on_period_elapsed() {
	++period_elapsed_cnt_;
	callback_.call();
}

void HwTimer::stop() {
	TIM_HandleTypeDef* handle = timer_handle(timer_id_);
	if (0 != handle) {
		if (MODE_BASIC == mode_) {
			HAL_TIM_Base_Stop_IT(handle);
			HAL_TIM_Base_DeInit(handle);
		} else if (MODE_PWM_DECODE == mode_) {
			HAL_TIM_IC_Stop(handle, TIM_CHANNEL_1);
			HAL_TIM_IC_Stop(handle, TIM_CHANNEL_2);
			HAL_TIM_IC_DeInit(handle);
		} else if (MODE_PWM_ENCODE == mode_) {
			for (size_t i = 0; i < channels_.size(); ++i) {
				HAL_TIM_PWM_Stop(handle, get_timx_channel(channels_[i]));
			}
			HAL_TIM_PWM_DeInit(handle);
			channels_.clear();
		}
	}
	mode_ = MODE_NOT_INITIALIZED;
}

void HwTimer::set_timer_clock(const Frequency& timer_clock) {
	if (timer_clock.unit() > 0) {
		timer_clock_ = timer_clock;
	} else {
		timer_clock_ = Frequency::from_kilohertz(10);
	}
}

TimeSpan HwTimer::time_elapsed() const {
	return TimeSpan::from_ticks(current_counter_value(), timer_clock_);
}

uint32_t HwTimer::current_counter_value() const {
	assert(timer_handle(timer_id_));
	return __HAL_TIM_GetCounter(timer_handle(timer_id_));
}

uint32_t HwTimer::read_captured_value(uint32_t channelNo) {
	TIM_HandleTypeDef* htim = timer_handle(timer_id_);
	uint32_t tim_channel = get_timx_channel(channelNo);
	return !!htim ? HAL_TIM_ReadCapturedValue(htim, tim_channel) : 0;
}

uint32_t HwTimer::get_gpio_altfunc(Id timer_id)
{
	switch (timer_id) {
	case TIMER_1: return GPIO_AF1_TIM1;
	case TIMER_2: return GPIO_AF1_TIM2;

	case TIMER_3: return GPIO_AF2_TIM3;
	case TIMER_4: return GPIO_AF2_TIM4;
	case TIMER_5: return GPIO_AF2_TIM5;

	case TIMER_8: return GPIO_AF3_TIM8;
	case TIMER_9: return GPIO_AF3_TIM9;
	case TIMER_10: return GPIO_AF3_TIM10;
	case TIMER_11: return GPIO_AF3_TIM11;

	default: return (uint32_t)-1;
	}
}

uint32_t HwTimer::get_timx_channel(uint32_t channel_no)
{
	switch (channel_no) {
	case 1: return TIM_CHANNEL_1;
	case 2: return TIM_CHANNEL_2;
	case 3: return TIM_CHANNEL_3;
	case 4: return TIM_CHANNEL_4;
	default:;
	}
	assert(false);
	return -1;
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
	case HwTimer::TIMER_1:
		__TIM1_CLK_ENABLE();
		break;
	case HwTimer::TIMER_2:
		__TIM2_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM2_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
		break;
	case HwTimer::TIMER_3:
		__TIM3_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM3_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(TIM3_IRQn);
		break;
	case HwTimer::TIMER_4:
		__TIM4_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM4_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(TIM4_IRQn);
		break;
	case HwTimer::TIMER_5:
		__TIM5_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM5_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(TIM5_IRQn);
		break;
	case HwTimer::TIMER_6:
		__TIM6_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
		break;
	case HwTimer::TIMER_7:
		__TIM7_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM7_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(TIM7_IRQn);
		break;
	case HwTimer::TIMER_8:
		__TIM8_CLK_ENABLE();
		break;
	case HwTimer::TIMER_9:
		__TIM9_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
		break;
	case HwTimer::TIMER_10:
		__TIM10_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
		break;
	case HwTimer::TIMER_11:
		__TIM11_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
		break;
	case HwTimer::TIMER_12:
		__TIM12_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
		break;
	case HwTimer::TIMER_13:
		__TIM13_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
		break;
	case HwTimer::TIMER_14:
		__TIM14_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
		break;
	default:assert(false);
	}
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim) {
	HAL_TIM_Base_MspInit(htim);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
	HAL_TIM_Base_MspInit(htim);
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

extern "C" void TIM6_DAC_IRQHandler(void) {
	HAL_TIM_IRQHandler(&all_timers_[7].handle_);
}

extern "C" void TIM7_IRQHandler(void) {
	HAL_TIM_IRQHandler(&all_timers_[7].handle_);
}

extern "C" void TIM1_BRK_TIM9_IRQHandler(void) {
	HAL_TIM_IRQHandler(&all_timers_[9].handle_);
}

extern "C" void TIM1_UP_TIM10_IRQHandler(void) {
	HAL_TIM_IRQHandler(&all_timers_[10].handle_);
}

extern "C" void TIM1_TRG_COM_TIM11_IRQHandler(void) {
	HAL_TIM_IRQHandler(&all_timers_[9].handle_);
}

extern "C" void TIM8_BRK_TIM12_IRQHandler(void) {
	HAL_TIM_IRQHandler(&all_timers_[9].handle_);
}

extern "C" void TIM8_UP_TIM13_IRQHandler(void) {
	HAL_TIM_IRQHandler(&all_timers_[9].handle_);
}

extern "C" void TIM8_TRG_COM_TIM14_IRQHandler(void) {
	HAL_TIM_IRQHandler(&all_timers_[9].handle_);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *handle) {
	HwTimer* timer = from_handle_to_timer(handle);
	timer->on_period_elapsed();
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *handle) {
	if (handle->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
		HwTimer* timer = from_handle_to_timer(handle);
		timer->on_period_elapsed();
	}
}
