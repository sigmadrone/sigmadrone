/*
 * rcchannel.cpp
 *
 *  Created on: Jul 4, 2015
 *      Author: svassilev
 */

#include "rcchannel.h"

RcChannel::RcChannel(uint32_t channelno, FunctionPointer callback) :
				decoder_(colibri::PWM_IN_CONSTS[channelno].timer_id_,
					colibri::PWM_IN_CONSTS[channelno].pin_,
					TimeSpan::from_milliseconds(30),
					callback,
					true)
{
	assert(channelno < colibri::PWM_IN_CHANNEL_COUNT);
}

void RcChannel::start_receive() {
	decoder_.start_on_ch1_ch2();
}

void RcChannel::stop_receive() {
	decoder_.stop();
}

const PwmDecoder& RcChannel::decoder() const  {
	return decoder_;
}
