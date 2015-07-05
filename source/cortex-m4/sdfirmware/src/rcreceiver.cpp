/*
 * radioreceiver.cpp
 *
 *  Created on: Jul 4, 2015
 *      Author: svassilev
 */
#include "rcreceiver.h"

RcReceiver::RcReceiver(uint32_t numChannels, FunctionPointer callback) {
	for (uint32_t i = 0; i < numChannels; ++i) {
		channels_.push_back(new RcChannel(i, callback));
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

const RcChannel* RcReceiver::channel(uint32_t channelno) const {
	assert(channelno < channels_.size());
	return channels_[channelno];
}



