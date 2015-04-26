/*
 * pwmdecoder.cpp
 *
 *  Created on: Jan 21, 2015
 *      Author: svetlio
 */

#include "pwmdecoder.h"

Frequency PwmDecoder::timer_clock_from_pwm_period(const TimeSpan& max_period) {
	static const uint32_t CCR_MAX_VALUE = 65535;
	return Frequency::from_timespan(max_period) * CCR_MAX_VALUE;
}

PwmDecoder::PwmDecoder(
		HwTimer::Id timer_id,
		PinName pin,
		const TimeSpan& max_period,
		const FunctionPointer callback,
		bool callback_on_change_only) :
				timer_(timer_id, timer_clock_from_pwm_period(max_period),
						FunctionPointer(this,&PwmDecoder::capture_callback)),
				gpio_(pin, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_HIGH, HwTimer::get_gpio_altfunc(timer_id)),
				decoded_period_(TimeSpan::from_seconds(0)),
				duty_cycle_(TimeSpan::from_seconds(0)),
				max_period_(max_period),
				callback_(callback),
				callback_on_change_only_(callback_on_change_only)
{
	gpio_.init();
}

PwmDecoder::PwmDecoder(
		HwTimer::Id timer_id,
		const GPIOPin& gpio,
		const TimeSpan& max_period,
		const FunctionPointer callback,
		bool callback_on_change_only) :
				timer_(timer_id, Frequency::from_timespan(max_period), FunctionPointer(this,
						&PwmDecoder::capture_callback)),
				gpio_(gpio),
				decoded_period_(TimeSpan::from_seconds(0)),
				duty_cycle_(TimeSpan::from_seconds(0)),
				max_period_(max_period),
				callback_(callback),
				callback_on_change_only_(callback_on_change_only)
{
	gpio_.init();
}

bool PwmDecoder::start() {
	stop();
	if (!timer_.start_pwm_decode_mode()) {
		return false;
	}
	return true;
}

void PwmDecoder::stop() {
	timer_.stop();
}

void PwmDecoder::capture_callback() {
	uint32_t ic1value = timer_.read_captured_value(1);
	uint32_t ic2value = timer_.read_captured_value(2);
	if (ic2value != 0) {
		TimeSpan last_period = decoded_period_;
		TimeSpan last_duty_cycle = duty_cycle_;
		decoded_period_ = TimeSpan::from_ticks(ic2value, timer_.timer_clock());
		duty_cycle_ = decoded_period_ * ic1value / ic2value;
		if (callback_on_change_only_) {
			if (last_period != decoded_period_ || last_duty_cycle != duty_cycle_) {
				callback_.call();
			}
		} else {
			callback_.call();
		}
	}
}
