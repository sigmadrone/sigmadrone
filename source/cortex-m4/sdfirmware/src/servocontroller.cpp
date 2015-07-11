/*
 * servocontroller.cpp
 *
 *  Created on: Jul 6, 2015
 *      Author: svassilev
 */

#include "servocontroller.h"
#include "pwmencoder.h"


ServoController::ServoController(
		const std::vector<PwmTxConsts>& pwmChannels,
		const Frequency& pwm_rate,
		const TimeSpan& max_pulse) : pwm_rate_(pwm_rate), max_pulse_(max_pulse), started_(false) {
	for (auto encoder_params: pwmChannels) {
		encoders_.push_back(new PwmEncoder(encoder_params.timer_id_, pwm_rate.period(),
				encoder_params.pins_));
	}
}

ServoController::~ServoController() {
	for (auto encoder: encoders_) {
		delete encoder;
	}
}

void ServoController::start(const Frequency& pwm_rate, const TimeSpan& arm_pulse) {
	TimeSpan period;

	if (started_) {
		return;
	}

	if (!pwm_rate.is_null()) {
		pwm_rate_ = pwm_rate;
		period = pwm_rate.period();
	}

	for (auto encoder: encoders_) {
		encoder->start(period);
	}

	arm_motors(arm_pulse);

	started_ = true;
}

void ServoController::stop() {
	for (auto encoder: encoders_) {
		encoder->stop();
	}
	started_ = false;
}

size_t ServoController::channel_count() const {
	return encoders_.size() * PwmEncoder::MAX_CHANNEL_COUNT;
}

void ServoController::arm_motors(const TimeSpan& pulse) {
	for (size_t i = 0; i < channel_count(); ++i) {
			set_pwm_pulse(i, pulse);
	}
}

Frequency ServoController::get_pwm_rate() {
	return pwm_rate_;
}

TimeSpan ServoController::get_pwm_period() {
	return pwm_rate_.period();
}

TimeSpan ServoController::get_pwm_pulse(uint32_t channel) {
	PwmEncoder* encoder = get_encoder_from_channel_no(channel);
	return encoder->get_duty_cycle(get_encoder_channel_from_channel_no(channel));
}

void ServoController::set_pwm_pulse(uint32_t channel, const TimeSpan& pulse) {
	if (started_) {
		PwmEncoder* encoder = get_encoder_from_channel_no(channel);
		encoder->set_duty_cycle(get_encoder_channel_from_channel_no(channel),
				pulse <= max_pulse_ ? pulse : max_pulse_ );
	}
}

bool ServoController::is_started() {
	return started_;
}

PwmEncoder* ServoController::get_encoder_from_channel_no(uint32_t channelno) {
	assert(encoders_.size() > channelno / PwmEncoder::MAX_CHANNEL_COUNT);
	return encoders_[channelno / PwmEncoder::MAX_CHANNEL_COUNT];
}

uint32_t ServoController::get_encoder_channel_from_channel_no(uint32_t channelno) {
	return (channelno % PwmEncoder::MAX_CHANNEL_COUNT) + 1;
}


