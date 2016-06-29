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


#ifndef COLIBRI_HW_MAP
#define COLIBRI_HW_MAP

#include "hwtimer.h"

static const PinName SHUTDOWN_PIN = PB_0;

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

static const HwTimer::Id PWM_IN_TIMER_1 = HwTimer::TIMER_2;
static const HwTimer::Id PWM_IN_TIMER_2 = HwTimer::TIMER_3;
static const HwTimer::Id PWM_IN_TIMER_3 = HwTimer::TIMER_4;
static const HwTimer::Id PWM_IN_TIMER_4 = HwTimer::TIMER_5;
static const HwTimer::Id PWM_IN_TIMER_5 = HwTimer::TIMER_9;

static const uint32_t PWM_IN_TIMER_1_PERIOD_CHANNELNO    = 1;
static const uint32_t PWM_IN_TIMER_1_DUTYCYCLE_CHANNELNO = 2;
static const uint32_t PWM_IN_TIMER_2_PERIOD_CHANNELNO    = 1;
static const uint32_t PWM_IN_TIMER_2_DUTYCYCLE_CHANNELNO = 2;
static const uint32_t PWM_IN_TIMER_3_PERIOD_CHANNELNO    = 1;
static const uint32_t PWM_IN_TIMER_3_DUTYCYCLE_CHANNELNO = 2;
static const uint32_t PWM_IN_TIMER_4_PERIOD_CHANNELNO    = 1;
static const uint32_t PWM_IN_TIMER_4_DUTYCYCLE_CHANNELNO = 2;
static const uint32_t PWM_IN_TIMER_5_PERIOD_CHANNELNO    = 1;
static const uint32_t PWM_IN_TIMER_5_DUTYCYCLE_CHANNELNO = 2;

static const PinName PWM_IN_PIN_1 = PA_5;
static const PinName PWM_IN_PIN_2 = PA_6;
static const PinName PWM_IN_PIN_3 = PD_12;
static const PinName PWM_IN_PIN_4 = PA_0;
static const PinName PWM_IN_PIN_5 = PA_2;

static const PinName GYRO_INT2_PIN = PA_3;
static const PinName GYRO_CS_PIN = PC_1;

static const PinName EXT_GPIO_1_PIN = PH_6;
static const PinName EXT_GPIO_2_PIN = PH_12;
static const PinName EXT_GPIO_3_PIN = PH_13;
static const PinName EXT_GPIO_4_PIN = PH_14;
static const PinName EXT_GPIO_5_PIN = PH_15;
static const PinName EXT_GPIO_6_PIN = PF_6;
static const PinName EXT_GYRO_CS_PIN = EXT_GPIO_1_PIN;

static const PinName MOTOR_POWER_CTRL_PIN = PB_0;

static const PinName GPS_POWER_CTRL_PIN = PG_14;

static const PinName I2C1_SCL_PIN = PB_8;
static const PinName I2C1_SDA_PIN = PB_9;

static const PinName I2C2_SCL_PIN = PB_10;
static const PinName I2C2_SDA_PIN = PB_11;

static const PinName  GPS_USART3_TX_PIN = PC_10;
static const PinName  GPS_USART3_RX_PIN = PC_11;

static const PinName  USB_OTG_LED_PIN = PC_4;
static const PinName  PWR_ON_PIN = PH_11;

static const PinName  USER_LED1_PIN = PI_11;
static const PinName  USER_LED2_PIN = PI_12;
static const PinName  USER_LED3_PIN = PI_13;
static const PinName  USER_LED4_PIN = PI_14;
static const PinName  USER_LED5_PIN = PI_15;

static const PinName  SENSOR_CTRL_PIN = PB_1;

static const PinName USART2_CTS_PIN = PD_3;
static const PinName USART2_RTS_PIN = PD_4;
static const PinName USART2_TX_PIN  = PD_5;
static const PinName USART2_RX_PIN  = PD_6;

static const PinName ACCEL_CS_PIN = PD_7;

static const PinName SPI5_SCK_PIN  = PF_7;
static const PinName SPI5_MISO_PIN = PF_8;
static const PinName SPI5_MOSI_PIN = PF_9;

#define MEMS_SPI_SCK_PIN  SPI5_SCK_PIN
#define MEMS_SPI_MISO_PIN SPI5_MISO_PIN
#define MEMS_SPI_MOSI_PIN SPI5_MOSI_PIN

static const PinName SPI2_SCK_PIN  = PI_1;
static const PinName SPI2_MISO_PIN = PI_2;
static const PinName SPI2_MOSI_PIN = PI_3;

#define EXT_MEMS_SPI_SCK_PIN  SPI2_SCK_PIN
#define EXT_MEMS_SPI_MISO_PIN SPI2_MISO_PIN
#define EXT_MEMS_SPI_MOSI_PIN SPI2_MOSI_PIN

static const PinName USART1_TX_PIN = PB_6;
static const PinName USART1_RX_PIN = PB_7;
#define TRACE_USART_TX_PIN USART1_TX_PIN
#define TRACE_USART_RX_PIN USART1_RX_PIN
#define TRACE_USART USART1

static const PinName USER_SWITCH_1_PIN = PB_2;
static const PinName USER_SWITCH_2_PIN = PG_2;
static const PinName USER_SWITCH_3_PIN = PG_3;
static const PinName USER_SWITCH_4_PIN = PG_6;
static const PinName USER_SWITCH_5_PIN = PG_7;

static const PinName BATTERY_MONITOR_PIN = PC_2;
static const PinName BATTERY_MONITOR_ONOFF_PIN = PH_9;
#define BATTERY_MONITOR_ADC ADC1
#define BATTERY_MONITOR_ADC_CHANNEL ADC_CHANNEL_12

static const PinName PCA9685_OUTPUT_ENABLE_PIN = PB_12;

#endif /* COLIBRI_HW_MAP */
