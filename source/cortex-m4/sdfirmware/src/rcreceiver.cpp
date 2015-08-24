/*
 * radioreceiver.cpp
 *
 *  Created on: Jul 4, 2015
 *      Author: svassilev
 */
#include "rcreceiver.h"

RcReceiver::RcReceiver(const std::vector<PwmRxConsts> rxConsts, FunctionPointer callback) {
	for (auto rxConst: rxConsts) {
		channels_.push_back(new RcChannel(rxConst, callback));
	}
}

void RcReceiver::start() {
	for (auto channel: channels_) {
		channel->start_receive();
	}
}

void RcReceiver::stop() {
	for (auto channel: channels_) {
		channel->stop_receive();
	}
}

RcChannel* RcReceiver::channel(uint32_t channelno) {
	assert(channelno < channels_.size());
	return channels_[channelno];
}



