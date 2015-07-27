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

	// TODO Auto-generated constructor stub
	handle_.Instance = i2c_device;

	handle_.Init.AddressingMode = addressing_mode;
	handle_.Init.ClockSpeed = clock_speed;
	handle_.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	handle_.Init.DutyCycle = duty_cycle;
	handle_.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	handle_.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	handle_.Init.OwnAddress1 = 0x00;
	handle_.Init.OwnAddress2 = 0xFE;
	HAL_I2C_Init(&handle_);
}

I2CMaster::~I2CMaster()
{
	HAL_I2C_DeInit(&handle_);
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
