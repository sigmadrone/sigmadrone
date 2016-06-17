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

#ifndef PCA9685_H_
#define PCA9685_H_

#include "i2cmaster.h"
#include "digitalout.h"
#include "units.h"


/*
I2CMaster i2c2(I2C2, 400000, I2C_DUTYCYCLE_2, I2C_ADDRESSINGMODE_7BIT, 750, {
				{I2C2_SCL_PIN, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF4_I2C2},
				{I2C2_SDA_PIN, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF4_I2C2},
		});
*/

class PCA9685
{
public:
	PCA9685(I2CMaster& i2c,
			bool enable_output,
			const Frequency& default_pwm_rate = Frequency::from_hertz(400));
	~PCA9685() = default;

	void set_duty_cycle(uint32_t channel_no, uint32_t on, uint32_t off);
	void set_duty_cycle(uint32_t channel_no, const TimeSpan& pulse_length);
	void get_duty_cycle(uint32_t channel_no, uint32_t* on, uint32_t* off);
	void set_pwm_rate(const Frequency& pwm_rate);
	void set_pwm_period(const TimeSpan& pwm_period) { set_pwm_rate(pwm_period.to_frequency()); }
	Frequency get_pwm_rate() const;
	void enable_disable_output(bool enable);

	static const Frequency internal_clock_;
	static const Frequency max_pwm_rate;
	static const Frequency min_pwm_rate;

	static const uint32_t max_on_off_value = 4095;
	static const uint32_t channel_count = 8;

private:
	void set_sleep_mode(bool sleep);
	uint8_t read_register(uint8_t reg_address);
	void write_register(uint8_t reg_address, uint8_t reg_value);
	static uint8_t calculate_prescaler(const Frequency& pwm_rate);
	static uint8_t led_register_address(uint32_t channel_no);

private:
	I2CMaster& i2c_;
	TimeSpan pwm_period_;
	DigitalOut enable_output_;
};

#endif /* PCA9685_H_ */
