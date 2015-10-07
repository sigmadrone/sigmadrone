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

#ifndef RCCHANNELMAPPER_H_
#define RCCHANNELMAPPER_H_

#include <cassert>
#include <stdexcept>
#include <vector>
#include "rcdefs.h"

class RcChannelMapper {
public:
	RcChannelMapper(const std::vector<RcChannelName> channelIndex) : channel_index_(channelIndex) {}
	uint32_t channel_no(RcChannelName chType) const {
		for (uint32_t i = 0; i < channel_index_.size(); ++i) {
			if (channel_index_[i] == chType) {
				return i;
			}
		}
		throw std::invalid_argument("invalid channel type provided");
	}
	RcChannelName channel_name(uint32_t channelno) const {
		return channel_index_[channelno];
	}
private:
	std::vector<RcChannelName> channel_index_;
};



#endif /* RCCHANNELMAPPER_H_ */
