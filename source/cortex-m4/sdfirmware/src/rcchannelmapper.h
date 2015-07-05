/*
 * rcchannelmapper.h
 *
 *  Created on: Jul 4, 2015
 *      Author: svassilev
 */

#ifndef RCCHANNELMAPPER_H_
#define RCCHANNELMAPPER_H_

#include <cassert>
#include <stdexcept>
#include <vector>
#include "rcdefs.h"

class RcChannelMapper {
public:
	RcChannelMapper(const std::vector<RcChannelName> channelIndex) : channelIndex_(channelIndex) {
		assert(channelIndex_.size() == RC_CHANNEL_LAST);
	}
	uint32_t channel_no(RcChannelName chType) const {
		for (auto index: channelIndex_) {
			if (index == chType) {
				return index;
			}
		}
		throw std::invalid_argument("invalid channel type provided");
	}
	RcChannelName channel_name(uint32_t channelno) const {
		return channelIndex_[channelno];
	}
private:
	std::vector<RcChannelName> channelIndex_;
};



#endif /* RCCHANNELMAPPER_H_ */
