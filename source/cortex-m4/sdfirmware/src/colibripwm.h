/*
 * colibripwm.h
 *
 *  Created on: Jun 25, 2015
 *      Author: svassilev
 */

#ifndef COLIBRIPWM_H_
#define COLIBRIPWM_H_

#include "rcdefs.h"

namespace colibri {

/*
 * Define the pins and timers specific to colibri schematics. The timers below are
 * reserved and should not be used for anything other than PWM encode/decode functions.
 */

static const uint32_t PWM_OUT_CHANNEL_COUNT = 8;

static const HwTimer::Id PWM_OUT_TIMER_1 = HwTimer::TIMER_1;
static const HwTimer::Id PWM_OUT_TIMER_2 = HwTimer::TIMER_8;

static const PinName PWM_OUT_PIN_1 = PA_8;
static const PinName PWM_OUT_PIN_2 = PA_9;
static const PinName PWM_OUT_PIN_3 = PA_10;
static const PinName PWM_OUT_PIN_4 = PA_11;
static const PinName PWM_OUT_PIN_5 = PC_6;
static const PinName PWM_OUT_PIN_6 = PC_7;
static const PinName PWM_OUT_PIN_7 = PC_8;
static const PinName PWM_OUT_PIN_8 = PC_9;

static const std::vector<PinName> PWM_OUT_PINS_1_4 = {PWM_OUT_PIN_1, PWM_OUT_PIN_2, PWM_OUT_PIN_3, PWM_OUT_PIN_4};
static const std::vector<PinName> PWM_OUT_PINS_5_8 = {PWM_OUT_PIN_5, PWM_OUT_PIN_6, PWM_OUT_PIN_7, PWM_OUT_PIN_8};

static const uint32_t PWM_IN_CHANNEL_COUNT = 4;

static const HwTimer::Id PWM_IN_TIMER_1 = HwTimer::TIMER_2;
static const HwTimer::Id PWM_IN_TIMER_2 = HwTimer::TIMER_3;
static const HwTimer::Id PWM_IN_TIMER_3 = HwTimer::TIMER_4;
static const HwTimer::Id PWM_IN_TIMER_4 = HwTimer::TIMER_5;

static const uint32_t PWM_IN_TIMER_1_PERIOD_CHANNELNO    = 1;
static const uint32_t PWM_IN_TIMER_1_DUTYCYCLE_CHANNELNO = 2;
static const uint32_t PWM_IN_TIMER_2_PERIOD_CHANNELNO    = 1;
static const uint32_t PWM_IN_TIMER_2_DUTYCYCLE_CHANNELNO = 2;
static const uint32_t PWM_IN_TIMER_3_PERIOD_CHANNELNO    = 1;
static const uint32_t PWM_IN_TIMER_3_DUTYCYCLE_CHANNELNO = 2;
static const uint32_t PWM_IN_TIMER_4_PERIOD_CHANNELNO    = 1;
static const uint32_t PWM_IN_TIMER_4_DUTYCYCLE_CHANNELNO = 2;

static const PinName PWM_IN_PIN_1 = PA_5;
static const PinName PWM_IN_PIN_2 = PA_6;
static const PinName PWM_IN_PIN_3 = PD_12;
static const PinName PWM_IN_PIN_4 = PA_0;
static const PinName PWM_IN_PIN_5 = PA_3;

static const std::vector<PwmRxConsts> PWM_RX_CONSTS = {
		{PWM_IN_TIMER_1, PWM_IN_PIN_1, PWM_IN_TIMER_1_PERIOD_CHANNELNO, PWM_IN_TIMER_1_DUTYCYCLE_CHANNELNO},
		{PWM_IN_TIMER_2, PWM_IN_PIN_2, PWM_IN_TIMER_2_PERIOD_CHANNELNO, PWM_IN_TIMER_2_DUTYCYCLE_CHANNELNO},
		{PWM_IN_TIMER_3, PWM_IN_PIN_3, PWM_IN_TIMER_3_PERIOD_CHANNELNO, PWM_IN_TIMER_3_DUTYCYCLE_CHANNELNO},
		{PWM_IN_TIMER_4, PWM_IN_PIN_4, PWM_IN_TIMER_4_PERIOD_CHANNELNO, PWM_IN_TIMER_4_DUTYCYCLE_CHANNELNO},
};

}


#endif /* COLIBRIPWM_H_ */
