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

#ifndef COLIBRIPWM_H_
#define COLIBRIPWM_H_

#include "rcdefs.h"
#include "colibrihwmap.h"

namespace colibri {

/*
 * Define the pins and timers specific to colibri schematics. The timers below are
 * reserved and should not be used for anything other than PWM encode/decode functions.
 */

static const std::vector<PinName> PWM_OUT_PINS_1_4 = {PWM_OUT_PIN_1, PWM_OUT_PIN_2, PWM_OUT_PIN_3, PWM_OUT_PIN_4};
static const std::vector<PinName> PWM_OUT_PINS_5_8 = {PWM_OUT_PIN_5, PWM_OUT_PIN_6, PWM_OUT_PIN_7, PWM_OUT_PIN_8};

static const PwmTxConsts PWM_TX_1_4 = { PWM_OUT_TIMER_1, PWM_OUT_PINS_1_4 };
static const PwmTxConsts PWM_TX_5_8 = { PWM_OUT_TIMER_1, PWM_OUT_PINS_5_8 };

static const std::vector<PwmRxConsts> PWM_RX_CONSTS = {
		{PWM_IN_TIMER_1, PWM_IN_PIN_1, PWM_IN_TIMER_1_PERIOD_CHANNELNO, PWM_IN_TIMER_1_DUTYCYCLE_CHANNELNO},
		{PWM_IN_TIMER_2, PWM_IN_PIN_2, PWM_IN_TIMER_2_PERIOD_CHANNELNO, PWM_IN_TIMER_2_DUTYCYCLE_CHANNELNO},
		{PWM_IN_TIMER_3, PWM_IN_PIN_3, PWM_IN_TIMER_3_PERIOD_CHANNELNO, PWM_IN_TIMER_3_DUTYCYCLE_CHANNELNO},
		{PWM_IN_TIMER_4, PWM_IN_PIN_4, PWM_IN_TIMER_4_PERIOD_CHANNELNO, PWM_IN_TIMER_4_DUTYCYCLE_CHANNELNO},
		{PWM_IN_TIMER_5, PWM_IN_PIN_5, PWM_IN_TIMER_5_PERIOD_CHANNELNO, PWM_IN_TIMER_5_DUTYCYCLE_CHANNELNO},
};

}


#endif /* COLIBRIPWM_H_ */
