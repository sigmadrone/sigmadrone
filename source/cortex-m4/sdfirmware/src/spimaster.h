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

#ifndef _SPIMASTER_H_
#define _SPIMASTER_H_

#include <stdint.h>
#include <vector>
#include "stm32f4xx_hal.h"
#include "stm32f429xx.h"
#include "gpiopin.h"

class SPIMaster
{
public:
	SPIMaster(SPI_TypeDef* spi_device = SPI5, uint32_t clk_prescale = SPI_BAUDRATEPRESCALER_16, uint32_t timeout = 0x1000, const std::vector<GPIOPin>& data_pins = {}, const std::vector<GPIOPin>& cs_pins = {});
	virtual ~SPIMaster();

	void read(uint8_t cs, uint8_t* buffer, uint16_t nbytes);
	void write(uint8_t cs, uint8_t* buffer, uint16_t nbytes);
	void read_reg(uint8_t cs, uint8_t reg, uint8_t* buffer, uint16_t nbytes);
	void write_reg(uint8_t cs, uint8_t reg, uint8_t* buffer, uint16_t nbytes);
	uint8_t read_reg8(uint8_t cs, uint8_t reg);
	uint16_t read_reg16(uint8_t cs, uint8_t reg);
	uint32_t read_reg24(uint8_t cs, uint8_t reg);
	void write_reg8(uint8_t cs, uint8_t reg, uint8_t val);
	void write_reg16(uint8_t cs, uint8_t reg, uint16_t val);

protected:
	uint8_t spi_write_read(uint8_t Byte = 0x0);
	void spi_chip_select(uint8_t chip, bool select);

protected:
	SPI_HandleTypeDef handle_;
	uint32_t timeout_;
	std::vector<GPIOPin> data_pins_;
	std::vector<GPIOPin> cs_pins_;
};

#endif /* _SPIMASTER_H_ */
