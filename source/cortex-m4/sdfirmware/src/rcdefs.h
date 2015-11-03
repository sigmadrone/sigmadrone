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
	RC_CHANNEL_ARM_MOTOR = 4,
	RC_CHANNEL_GEAR = RC_CHANNEL_ARM_MOTOR,
	RC_CHANNEL_LAST
};

inline const char* RcChannelToString(RcChannelName name)
{
	switch(name) {
	case RC_CHANNEL_THROTTLE: return "RC_CHANNEL_THROTTLE";
	case RC_CHANNEL_AILERON: return "RC_CHANNEL_AILERON";
	case RC_CHANNEL_ELEVATOR: return "RC_CHANNEL_ELEVATOR";
	case RC_CHANNEL_RUDDER: return "RC_CHANNEL_RUDDER";
	case RC_CHANNEL_GEAR: return "RC_CHANNEL_GEAR";
	default: return "RC_CHANNEL_UNKNOWN";
	}
}

#endif /* RCDEFS_H_ */
