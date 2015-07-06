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
		bool callback_on_change_only,
		const TimeSpan& epsilon) :
				timer_(timer_id, timer_clock_from_pwm_period(max_period),
						FunctionPointer(this,&PwmDecoder::capture_callback)),
				gpio_(pin, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_HIGH, HwTimer::get_gpio_altfunc(timer_id)),
				decoded_period_(TimeSpan::from_seconds(0)),
				duty_cycle_(TimeSpan::from_seconds(0)),
				max_period_(max_period),
				callback_(callback),
				period_channel_(HwTimer::INVALID_CHANNEL_NO),
				duty_cycle_channel_(HwTimer::INVALID_CHANNEL_NO),
				callback_on_change_only_(callback_on_change_only),
				epsilon_(epsilon)
{
	gpio_.init();
}

PwmDecoder::PwmDecoder(
		HwTimer::Id timer_id,
		const GPIOPin& gpio,
		const TimeSpan& max_period,
		const FunctionPointer callback,
		bool callback_on_change_only,
		const TimeSpan& epsilon) :
				timer_(timer_id, timer_clock_from_pwm_period(max_period), FunctionPointer(this,
						&PwmDecoder::capture_callback)),
				gpio_(gpio),
				decoded_period_(TimeSpan::from_seconds(0)),
				duty_cycle_(TimeSpan::from_seconds(0)),
				max_period_(max_period),
				callback_(callback),
				period_channel_(HwTimer::INVALID_CHANNEL_NO),
				duty_cycle_channel_(HwTimer::INVALID_CHANNEL_NO),
				callback_on_change_only_(callback_on_change_only),
				epsilon_(epsilon)
{
	gpio_.init();
}

bool PwmDecoder::start(
		uint32_t period_channel_no,
		uint32_t duty_cycle_channel_no,
		uint32_t input_trigger) {
	stop();
	period_channel_= period_channel_no;
	duty_cycle_channel_ = duty_cycle_channel_no;
	return timer_.start_pwm_decode_mode(period_channel_, duty_cycle_channel_, input_trigger);
}

void PwmDecoder::stop() {
	timer_.stop();
}

bool PwmDecoder::ts_within_range(const TimeSpan& ts1, const TimeSpan& ts2)
{
	if (ts1 >= ts2 && (ts1-ts2) <= epsilon_) {
		return true;
	}
	if (ts2 > ts1 && (ts2-ts1) <= epsilon_) {
		return true;
	}
	return false;
}

void PwmDecoder::capture_callback() {
	uint32_t duty_cycle_value = timer_.read_captured_value(duty_cycle_channel_);
	uint32_t period_value = timer_.read_captured_value(period_channel_);
	if (period_value != 0) {
		TimeSpan last_period = decoded_period_;
		TimeSpan last_duty_cycle = duty_cycle_;
		TimeSpan new_period = TimeSpan::from_ticks(period_value, timer_.timer_clock());
		TimeSpan new_duty_cycle = new_period * duty_cycle_value / period_value;
		bool do_call = true;
		if (callback_on_change_only_) {
			if (ts_within_range(last_period, new_period) &&
					ts_within_range(last_duty_cycle, new_duty_cycle)) {
				do_call = false;
			}
		}
		if (do_call) {
			decoded_period_ = new_period;
			duty_cycle_ = new_duty_cycle;
			callback_.call();
		}
	}
}
