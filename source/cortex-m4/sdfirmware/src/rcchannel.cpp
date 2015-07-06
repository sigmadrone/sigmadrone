/*
 * rcchannel.cpp
 *
 *  Created on: Jul 4, 2015
 *      Author: svassilev
 */

#include <stdexcept>
#include "rcchannel.h"

RcChannel::RcChannel(const PwmRxConsts& pwmRxConsts, FunctionPointer callback) :
	decoder_(pwmRxConsts.timer_id_,pwmRxConsts.pin_, TimeSpan::from_milliseconds(30), callback, true,
			TimeSpan::from_microseconds(20)),
	tim_period_channel_(pwmRxConsts.timer_period_channel_)
{
	if (tim_period_channel_ != 1 && tim_period_channel_ != 2) {
		throw std::invalid_argument("invalid tim_period_channel" + tim_period_channel_);
	}
}

void RcChannel::start_receive() {
	if (tim_period_channel_ == 1) {
		decoder_.start_on_ch1_ch2();
	} else {
		assert(tim_period_channel_ == 2);
		decoder_.start_on_ch2_ch1();
	}
}

void RcChannel::stop_receive() {
	decoder_.stop();
}

const PwmDecoder& RcChannel::decoder() const  {
	return decoder_;
}
