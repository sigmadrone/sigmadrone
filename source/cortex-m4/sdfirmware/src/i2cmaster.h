/*
 * i2cmaster.h
 *
 *  Created on: Jul 26, 2015
 *      Author: mstoilov
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
