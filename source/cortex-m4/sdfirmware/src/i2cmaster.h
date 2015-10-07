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

#ifndef I2CMASTER_H_
#define I2CMASTER_H_

#include <stdint.h>
#include <vector>
#include <stdexcept>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f429xx.h"
#include "gpiopin.h"

class I2CMaster
{
public:
	I2CMaster(
		I2C_TypeDef* i2c_device = I2C1,
		uint32_t clock_speed = 400000,
		uint32_t duty_cycle = I2C_DUTYCYCLE_2,
		uint32_t addressing_mode = I2C_ADDRESSINGMODE_7BIT,
		uint32_t timeout = 250,
		std::vector<GPIOPin> data_pins = {});
	virtual ~I2CMaster();
	void init();
	void deinit();
	void reinit();
	void read(uint16_t dev_addr, uint16_t reg_addr, uint8_t* buffer, uint16_t nbytes) throw (std::exception);
	uint8_t read8(uint16_t dev_addr, uint16_t reg_addr) throw (std::exception);
	uint16_t read16(uint16_t dev_addr, uint16_t reg_addr) throw (std::exception);
	uint32_t read32(uint16_t dev_addr, uint16_t reg_addr) throw (std::exception);
	void write(uint16_t dev_addr, uint16_t reg_addr, uint8_t* buffer, uint16_t nbytes) throw (std::exception);
	void write8(uint16_t dev_addr, uint16_t reg_addr, uint8_t data) throw (std::exception);
	void write16(uint16_t dev_addr, uint16_t reg_addr, uint16_t data) throw (std::exception);
	void write32(uint16_t dev_addr, uint16_t reg_addr, uint32_t data) throw (std::exception);

protected:
	I2C_HandleTypeDef handle_;
	uint32_t timeout_;
};

#endif /* I2CMASTER_H_ */
