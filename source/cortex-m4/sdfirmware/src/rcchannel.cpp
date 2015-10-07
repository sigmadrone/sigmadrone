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

PwmDecoder& RcChannel::decoder() {
	return decoder_;
}

bool RcChannel::is_live() const {
	TimeSpan elapsed = decoder_.last_capture_ts().elapsed();
	return elapsed < decoder_.decoded_period() * 4;
}
