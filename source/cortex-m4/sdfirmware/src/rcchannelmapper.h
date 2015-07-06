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
