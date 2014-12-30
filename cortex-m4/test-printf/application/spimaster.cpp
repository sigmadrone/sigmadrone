/*
 * spimaster.cpp
 *
 *  Created on: Dec 23, 2014
 *      Author: mstoilov
 */

#include <cstring>
#include <stdexcept>
#include "spimaster.h"

SPIMaster::SPIMaster(SPI_TypeDef* spi_device, uint32_t timeout, const std::vector<GPIOPin>& data_pins, const std::vector<GPIOPin>& cs_pins)
	: timeout_(timeout)
	, data_pins_(data_pins)
	, cs_pins_(cs_pins)
{
	for (auto& data_pin : data_pins_)
		data_pin.init();
	for (auto& cs_pin : cs_pins_)
		cs_pin.init();
	memset(&handle_, 0, sizeof(handle_));

	/* SPI configuration -----------------------------------------------------*/
	handle_.Instance = spi_device;
	/* SPI baudrate is set to 5.6 MHz (PCLK2/SPI_BaudRatePrescaler = 90/16 = 5.625 MHz)
	 to verify these constraints:
	 - ILI9341 LCD SPI interface max baudrate is 10MHz for write and 6.66MHz for read
	 - l3gd20 SPI interface max baudrate is 10MHz for write/read
	 - PCLK2 frequency is set to 90 MHz
	 */
	handle_.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;

	/* On STM32F429I-Discovery, LCD ID cannot be read then keep a common configuration */
	/* for LCD and GYRO (SPI_DIRECTION_2LINES) */
	/* Note: To read a register a LCD, SPI_DIRECTION_1LINE should be set */
	handle_.Init.Direction = SPI_DIRECTION_2LINES;
	handle_.Init.CLKPhase = SPI_PHASE_1EDGE;
	handle_.Init.CLKPolarity = SPI_POLARITY_LOW;
	handle_.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
	handle_.Init.CRCPolynomial = 7;
	handle_.Init.DataSize = SPI_DATASIZE_8BIT;
	handle_.Init.FirstBit = SPI_FIRSTBIT_MSB;
	handle_.Init.NSS = SPI_NSS_SOFT;
	handle_.Init.TIMode = SPI_TIMODE_DISABLED;
	handle_.Init.Mode = SPI_MODE_MASTER;

//	SPIx_MspInit (&SpiHandle);
	HAL_SPI_Init(&handle_);
}

SPIMaster::~SPIMaster()
{
	HAL_SPI_DeInit(&handle_);
}

uint8_t SPIMaster::spi_write_read(uint8_t Byte)
{
	uint8_t receivedbyte = 0;

	/* Send a Byte through the SPI peripheral */
	/* Read byte from the SPI bus */
	if (HAL_SPI_TransmitReceive(&handle_, (uint8_t*) &Byte, (uint8_t*) &receivedbyte, 1, timeout_) != HAL_OK) {
		throw std::runtime_error("SPI timeout");
	}
	return receivedbyte;
}

void SPIMaster::spi_chip_select(uint8_t chip, bool select)
{
	if (chip < cs_pins_.size()) {
		HAL_GPIO_WritePin(cs_pins_[chip].gpio_port_, cs_pins_[chip].Pin, select ? GPIO_PIN_RESET : GPIO_PIN_SET);
	}
}

void SPIMaster::read(uint8_t cs, uint8_t addr, uint8_t* buffer, uint16_t nbytes)
{
	uint8_t dummybuffer[64];

	memset(dummybuffer, 0, sizeof(dummybuffer));

	/* Set chip select Low at the start of the transmission */
	spi_chip_select(cs, true);

	/* Send the Address of the indexed register */
	if (HAL_SPI_TransmitReceive(&handle_, (uint8_t*) &addr, (uint8_t*) dummybuffer, 1, timeout_) != HAL_OK) {
		throw std::runtime_error("SPI timeout");
	}

	/* Read the data from the device */
	while (nbytes > 0) {
		uint32_t rxbytes = (nbytes > sizeof(dummybuffer)) ? sizeof(dummybuffer) : nbytes;
		if (HAL_SPI_TransmitReceive(&handle_, (uint8_t*) dummybuffer, (uint8_t*) buffer, rxbytes, timeout_) != HAL_OK) {
			throw std::runtime_error("SPI timeout");
		}
		buffer += rxbytes;
		nbytes -= rxbytes;
	}

	/* Set chip select High at the end of the transmission */
	spi_chip_select(cs, false);
}

void SPIMaster::write(uint8_t cs, uint8_t addr, uint8_t* buffer, uint16_t nbytes)
{
	uint8_t dummybuffer[64];

	memset(dummybuffer, 0, sizeof(dummybuffer));

	/* Set chip select Low at the start of the transmission */
	spi_chip_select(cs, true);

	/* Send the Address of the indexed register */
	if (HAL_SPI_TransmitReceive(&handle_, (uint8_t*) &addr, (uint8_t*) dummybuffer, 1, timeout_) != HAL_OK) {
		throw std::runtime_error("SPI timeout");
	}

	/* Send the data that will be written into the device */
	while (nbytes > 0) {
		uint32_t txbytes = (nbytes > sizeof(dummybuffer)) ? sizeof(dummybuffer) : nbytes;
		if (HAL_SPI_TransmitReceive(&handle_, (uint8_t*) buffer, (uint8_t*) dummybuffer, txbytes, timeout_) != HAL_OK) {
			throw std::runtime_error("SPI timeout");
		}
		buffer += txbytes;
		nbytes -= txbytes;
	}

	/* Set chip select High at the end of the transmission */
	spi_chip_select(cs, false);
}

uint8_t SPIMaster::read_reg8(uint8_t cs, uint8_t reg)
{
	uint8_t ret;
	read(cs, reg, (uint8_t*)&ret, sizeof(ret));
	return ret;
}

uint16_t SPIMaster::read_reg16(uint8_t cs, uint8_t reg)
{
	uint16_t ret;
	read(cs, reg, (uint8_t*)&ret, sizeof(ret));
	return ret;
}

void SPIMaster::write_reg8(uint8_t cs, uint8_t reg, uint8_t val)
{
	write(cs, reg, (uint8_t*)&val, sizeof(val));
}

void SPIMaster::write_reg16(uint8_t cs, uint8_t reg, uint16_t val)
{
	write(cs, reg, (uint8_t*)&val, sizeof(val));
}

