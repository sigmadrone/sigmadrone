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

#include "pca9685.h"
#include "colibrihwmap.h"
#include "FreeRTOS.h"
#include "task.h"

const Frequency PCA9685::internal_clock_ = Frequency::from_megahertz(25);
const Frequency PCA9685::max_pwm_rate = Frequency::from_hertz(1526);
const Frequency PCA9685::min_pwm_rate = Frequency::from_hertz(24);

#define PCA9685_I2C_ADDRESS 0xE0

/* See 7.3 Register definitions */
#define PCA9685_MODE1_REG 		0x00
#define PCA9685_MODE2_REG 		0x01
#define PCA9685_ALLCALLADR_REG 	0x05
#define PCA9685_PRESCALE_REG 	0xFE

/* See 7.3.1 Mode register 1, MODE1 */
#define PCA9685_MODE1_RESTART 0x80
#define PCA9685_MODE1_EXTCLK 0x40
#define PCA9685_MODE1_AI 0x20
#define PCA9685_MODE1_SLEEP 0x10
#define PCA9685_MODE1_SUB1 0x08
#define PCA9685_MODE1_SUB2 0x04
#define PCA9685_MODE1_SUB3 0x02
#define PCA9685_MODE1_ALLCALL 0x01

/* 7.3.2 Mode register 2, MODE2 */
#define PCA9685_MODE2_INVRT 0x10
#define PCA9685_MODE2_OCH 0x08
#define PCA9685_MODE2_OUTDRV 0x04
#define PCA9685_MODE2_OUTNE 0x03 //Actually a mask of bits. Paragraph 7.3.2 Mode register 2, MODE2

#define LED0_ON_L_REG  0x6
#define LED0_ON_H_REG  0x7
#define LED0_OFF_L_REG 0x8
#define LED0_OFF_H_REG 0x9

static void delay_msec(unsigned int msec)
{
	vTaskDelay(msec / portTICK_RATE_MS);
}

PCA9685::PCA9685(
		I2CMaster& i2c,
		bool enable_output,
		const Frequency& default_pwm_rate) : i2c_(i2c), enable_output_(PCA9685_OUTPUT_ENABLE_PIN)
{
	set_sleep_mode(false);
	set_pwm_rate(default_pwm_rate);
	enable_disable_output(enable_output);
}

void PCA9685::set_sleep_mode(bool sleep)
{
	uint8_t mode1 = read_register(PCA9685_MODE1_REG);
	mode1 = sleep ? (mode1 | PCA9685_MODE1_SLEEP) : (mode1 & ~PCA9685_MODE1_SLEEP);
	write_register(PCA9685_MODE1_REG, mode1);
	delay_msec(1);
}

void PCA9685::set_pwm_rate(const Frequency& pwm_rate)
{
	uint8_t prescaler = calculate_prescaler(pwm_rate);
	uint8_t prev_mode = read_register(PCA9685_MODE1_REG);
	uint8_t newmode = (prev_mode & !PCA9685_MODE1_RESTART) | PCA9685_MODE1_SLEEP; // Don't change any MODE1 bit except set SLEEP bit 1 and RESTART bit goes 0
    write_register(PCA9685_MODE1_REG, newmode); // go to sleep, shutting the oscillator off
    write_register(PCA9685_PRESCALE_REG, prescaler); // set the prescaler
    write_register(PCA9685_MODE1_REG, prev_mode); // restore the previous mode
    delay_msec(1); // always good idea to wait, so the hw can absorb the changes
	write_register(PCA9685_MODE1_REG, prev_mode | PCA9685_MODE1_ALLCALL | PCA9685_MODE1_AI | PCA9685_MODE1_RESTART);
	pwm_period_ = pwm_rate.period();
}

Frequency PCA9685::get_pwm_rate() const
{
	return pwm_period_.to_frequency();
}

void PCA9685::set_duty_cycle(uint32_t channel_no, const TimeSpan& pulse_length)
{
	uint16_t off = static_cast<uint16_t>((pulse_length * (PCA9685::max_on_off_value+1)) / pwm_period_);
	set_duty_cycle(channel_no, 0, off < max_on_off_value ? off : max_on_off_value);
}

void PCA9685::set_duty_cycle(uint32_t channel_no, uint32_t on, uint32_t off)
{
	if (on > off) {
		on = off;
	}
	if (off > max_on_off_value) {
		off = max_on_off_value;
	}
	uint8_t txbuff[4] = {
			static_cast<uint8_t>((on & 0xFF)),
			static_cast<uint8_t>((on >> 8) & 0xFF),
			static_cast<uint8_t>((off & 0xFF)),
			static_cast<uint8_t>((off >> 8) & 0xFF)
	};
	i2c_.write(PCA9685_I2C_ADDRESS, led_register_address(channel_no), txbuff, sizeof(txbuff));
}

void PCA9685::get_duty_cycle(uint32_t channel_no, uint32_t* on, uint32_t* off)
{
	if (on) {
		*on = i2c_.read16(PCA9685_I2C_ADDRESS, led_register_address(channel_no));
	}
	if (off) {
		*off = i2c_.read16(PCA9685_I2C_ADDRESS, led_register_address(channel_no) + 2);
	}
}

void PCA9685::enable_disable_output(bool enable)
{
	enable_output_.write(!enable);
}

uint8_t PCA9685::read_register(uint8_t reg_address)
{
	return i2c_.read8(PCA9685_I2C_ADDRESS, reg_address);
}

void PCA9685::write_register(uint8_t reg_address, uint8_t reg_value)
{
	i2c_.write8(PCA9685_I2C_ADDRESS, reg_address, reg_value);
}

uint8_t PCA9685::calculate_prescaler(const Frequency& pwm_rate)
{
	uint8_t prescaler = static_cast<uint8_t>(internal_clock_/(pwm_rate * 4096) - 1);
	return prescaler > 3 ? prescaler : 3;
}

uint8_t PCA9685::led_register_address(uint32_t channel_no)
{
	return LED0_ON_L_REG + channel_no * 4;
}
