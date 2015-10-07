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



