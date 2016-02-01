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

#include "pwmencoder.h"

PwmEncoder::PwmEncoder(HwTimer::Id timer_id,
		const TimeSpan& pwm_period,
		const std::vector<PinName>& out_pin_names,
		const std::vector<uint32_t>& channels) :
		timer_(timer_id, pwm_period, Frequency::from_megahertz(22)),
		channels_(channels)
{
	for (auto pin_name : out_pin_names) {
		GPIOPin gpio_pin(pin_name,GPIO_MODE_AF_PP,GPIO_PULLUP,
				GPIO_SPEED_HIGH,HwTimer::get_gpio_altfunc(timer_id));
		gpio_pin.init();
		out_pins_.push_back(gpio_pin);
	}
	if (0 == channels_.size()) {
		for (size_t i = 0; i < out_pin_names.size(); ++i) {
			channels_.push_back(i+1);
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

uint32_t PwmEncoder::get_channel_count() {
	return channels_.size();
}
