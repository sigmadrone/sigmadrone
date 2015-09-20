/*
 * i2cmaster.cpp
 *
 *  Created on: Jul 26, 2015
 *      Author: mstoilov
 */

#include "i2cmaster.h"

I2CMaster::I2CMaster(
	I2C_TypeDef* i2c_device,
	uint32_t clock_speed,
	uint32_t duty_cycle,
	uint32_t addressing_mode,
	uint32_t timeout,
	std::vector<GPIOPin> data_pins
	)
	: timeout_(timeout)
{
	for (auto& data_pin : data_pins)
		data_pin.init();

	if (i2c_device == I2C1) {
		__HAL_RCC_I2C1_CLK_ENABLE();
	} else if (i2c_device == I2C2) {
		__HAL_RCC_I2C2_CLK_ENABLE();
	} else if (i2c_device == I2C3) {
		__HAL_RCC_I2C3_CLK_ENABLE();
	}

	handle_.Instance = i2c_device;

	handle_.Init.AddressingMode = addressing_mode;
	handle_.Init.ClockSpeed = clock_speed;
	handle_.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	handle_.Init.DutyCycle = duty_cycle;
	handle_.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	handle_.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	handle_.Init.OwnAddress1 = 0x00;
	handle_.Init.OwnAddress2 = 0xFE;
	init();
}

I2CMaster::~I2CMaster()
{
	HAL_I2C_DeInit(&handle_);
}

void I2CMaster::init()
{
	HAL_I2C_Init(&handle_);
}

void I2CMaster::deinit()
{
	HAL_I2C_DeInit(&handle_);
}

void I2CMaster::reinit()
{
	deinit();
	init();
}

void I2CMaster::read(uint16_t dev_addr, uint16_t reg_addr, uint8_t* buffer, uint16_t nbytes) throw (std::exception)
{
	if (HAL_I2C_Mem_Read(&handle_, (uint16_t)dev_addr, (uint16_t)reg_addr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)buffer, nbytes, timeout_) != HAL_OK)
		throw std::runtime_error("HAL_I2C_Mem_Read failed");
}

void I2CMaster::write(uint16_t dev_addr, uint16_t reg_addr, uint8_t* buffer, uint16_t nbytes) throw (std::exception)
{
	if (HAL_I2C_Mem_Write(&handle_, (uint16_t)dev_addr, (uint16_t)reg_addr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)buffer, nbytes, timeout_) != HAL_OK)
		throw std::runtime_error("HAL_I2C_Mem_Write failed");
}

uint8_t I2CMaster::read8(uint16_t dev_addr, uint16_t reg_addr) throw (std::exception)
{
	uint8_t ret = 0;
	read(dev_addr, reg_addr, &ret, sizeof(ret));
	return ret;
}

uint16_t I2CMaster::read16(uint16_t dev_addr, uint16_t reg_addr) throw (std::exception)
{
	uint16_t ret = 0;
	read(dev_addr, reg_addr, (uint8_t*)&ret, sizeof(ret));
	return ret;
}

uint32_t I2CMaster::read32(uint16_t dev_addr, uint16_t reg_addr) throw (std::exception)
{
	uint32_t ret = 0;
	read(dev_addr, reg_addr, (uint8_t*)&ret, sizeof(ret));
	return ret;
}

void I2CMaster::write8(uint16_t dev_addr, uint16_t reg_addr, uint8_t data) throw (std::exception)
{
	write(dev_addr, reg_addr, (uint8_t*)&data, sizeof(data));
}

void I2CMaster::write16(uint16_t dev_addr, uint16_t reg_addr, uint16_t data) throw (std::exception)
{
	write(dev_addr, reg_addr, (uint8_t*)&data, sizeof(data));
}

void I2CMaster::write32(uint16_t dev_addr, uint16_t reg_addr, uint32_t data) throw (std::exception)
{
	write(dev_addr, reg_addr, (uint8_t*)&data, sizeof(data));
}
