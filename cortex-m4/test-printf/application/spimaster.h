/*
 * spimaster.h
 *
 *  Created on: Dec 23, 2014
 *      Author: mstoilov
 */

#ifndef _SPIMASTER_H_
#define _SPIMASTER_H_

#include <stdint.h>
#include <vector>
#include "stm32f4xx_hal_conf.h"
#include "stm32f429xx.h"
#include "gpiopin.h"

class SPIMaster
{
public:
	SPIMaster(SPI_TypeDef* spi_device = SPI5, uint32_t timeout = 0x1000, const std::vector<GPIOPin>& data_pins = {}, const std::vector<GPIOPin>& cs_pins = {});
	virtual ~SPIMaster();

	void read(uint8_t cs, uint8_t reg, uint8_t* buffer, uint16_t nbytes);
	void write(uint8_t cs, uint8_t reg, uint8_t* buffer, uint16_t nbytes);
	uint8_t read_reg8(uint8_t cs, uint8_t reg);
	uint16_t read_reg16(uint8_t cs, uint8_t reg);
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
