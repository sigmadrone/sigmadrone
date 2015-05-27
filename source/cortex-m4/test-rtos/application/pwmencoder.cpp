/*
 * pwmencoder.cpp
 *
 *  Created on: Jan 22, 2015
 *      Author: svetlio
 */

#include "pwmencoder.h"

PwmEncoder::PwmEncoder(HwTimer::Id timer_id,
		const TimeSpan& pwm_period,
		const std::vector<PinName>& out_pin_names,
		const std::vector<uint32_t>& channels) : timer_(timer_id, pwm_period), channels_(channels)
{
	for (auto pin_name : out_pin_names) {
		GPIOPin gpio_pin(pin_name,GPIO_MODE_AF_PP,GPIO_PULLUP,
				GPIO_SPEED_HIGH,HwTimer::get_gpio_altfunc(timer_id));
		gpio_pin.init();
		out_pins_.push_back(gpio_pin);
	}
	if (0 == channels_.size()) {
		for (size_t i = 0; i < out_pin_names.size(); ++i) {
			channels_.push_back(i);
		}
	}
}

bool PwmEncoder::start(const TimeSpan& pwm_period) {
	if (!pwm_period.is_null()) {
		timer_.set_period(pwm_period);
	}
	return timer_.start_pwm_encode_mode(channels_);
}

void PwmEncoder::stop() {
	timer_.stop();
}

bool PwmEncoder::set_duty_cycle(uint32_t channel, const TimeSpan& cycle) {
	return timer_.set_pwm_duty_cycle(channel, cycle);
}

bool PwmEncoder::set_duty_cycle(uint32_t channel, float value) {
	TimeSpan cycle = TimeSpan::from_nanoseconds(
			static_cast<float>(timer_.period().nanoseconds()) * value);
	return set_duty_cycle(channel, cycle);
}
