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
	RcReceiver(uint32_t numChannels, FunctionPointer callback = FunctionPointer());
	const RcChannel* channel(uint32_t channelno) const;
	void start();
	void stop();
private:
	std::vector<RcChannel*> channels_;
};

#endif /* RCRECEIVER_H_ */
