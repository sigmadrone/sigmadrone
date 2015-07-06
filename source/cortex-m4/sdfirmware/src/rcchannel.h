/*
 * rcchannel.h
 *
 *  Created on: Jul 4, 2015
 *      Author: svassilev
 */

#ifndef RCCHANNEL_H_
#define RCCHANNEL_H_

#include "pwmdecoder.h"
#include "rcdefs.h"

class RcChannel {
public:
	RcChannel(const PwmRxConsts& pwmRxConsts, FunctionPointer callback);
	const PwmDecoder& decoder() const;
	void start_receive();
	void stop_receive();

private:
	RcChannel(const RcChannel&);
	const RcChannel& operator=(const RcChannel&);

private:
	PwmDecoder decoder_;
	uint32_t tim_period_channel_;
};

#endif /* RCCHANNEL_H_ */
