/*
 * radioreceiver.h
 *
 *  Created on: Jun 29, 2015
 *      Author: svassilev
 */

#ifndef RCRECEIVER_H_
#define RCRECEIVER_H_

#include "rcchannel.h"

class RcReceiver {
public:
	RcReceiver(const std::vector<PwmRxConsts> rxConsts, FunctionPointer callback = FunctionPointer());
	RcChannel* channel(uint32_t channelno);
	void start();
	void stop();
	size_t channel_count() { return channels_.size(); }
private:
	std::vector<RcChannel*> channels_;
};

#endif /* RCRECEIVER_H_ */
