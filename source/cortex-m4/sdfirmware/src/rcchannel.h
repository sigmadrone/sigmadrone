/*
 * rcchannel.h
 *
 *  Created on: Jul 4, 2015
 *      Author: svassilev
 */

#ifndef RCCHANNEL_H_
#define RCCHANNEL_H_

#include "colibripwm.h"
#include "pwmdecoder.h"
#include "rcdefs.h"

class RcChannel {
public:
	RcChannel(uint32_t channelno, FunctionPointer callback);
	const PwmDecoder& decoder() const;
	void start_receive();
	void stop_receive();

private:
	RcChannel(const RcChannel&);
	const RcChannel& operator=(const RcChannel&);

private:
	PwmDecoder decoder_;
};

#endif /* RCCHANNEL_H_ */
