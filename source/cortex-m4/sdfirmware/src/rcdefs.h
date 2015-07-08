/*
 * rcdefs.h
 *
 *  Created on: Jul 4, 2015
 *      Author: svassilev
 */

#ifndef RCDEFS_H_
#define RCDEFS_H_

#include "hwtimer.h"

struct PwmRxConsts {
	HwTimer::Id timer_id_;
	PinName pin_;
	uint32_t timer_period_channel_;
	uint32_t timer_dutycycle_channel_;
};

struct PwmTxConsts {
	HwTimer::Id timer_id_;
	std::vector<PinName> pins_;
};

enum RcChannelName {
	RC_CHANNEL_THROTTLE = 0,
	RC_CHANNEL_ROLL = 1,
	RC_CHANNEL_AILERON = RC_CHANNEL_ROLL,
	RC_CHANNEL_PITCH = 2,
	RC_CHANNEL_ELEVATOR = RC_CHANNEL_PITCH,
	RC_CHANNEL_YAW = 3,
	RC_CHANNEL_RUDDER = RC_CHANNEL_YAW,
	RC_CHANNEL_LAST
};

#endif /* RCDEFS_H_ */
