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

/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80)
/* Multiple byte read/write command */
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)

void SPIMaster::read(uint8_t cs, uint8_t ReadAddr, uint8_t* pBuffer, uint8_t NumByteToRead)
{
	/*
	 * This should be moved out of here
	 */
	if (NumByteToRead > 0x01) {
		ReadAddr |= (uint8_t) (READWRITE_CMD | MULTIPLEBYTE_CMD);
	} else {
		ReadAddr |= (uint8_t) READWRITE_CMD;
	}


	/* Set chip select Low at the start of the transmission */
	spi_chip_select(cs, true);

	/* Send the Address of the indexed register */
	spi_write_read(ReadAddr);

	/* Receive the data that will be read from the device (MSB First) */
	while (NumByteToRead > 0x00) {
		/* Send dummy byte (0x00) to generate the SPI clock to Gyroscope (Slave device) */
		*pBuffer = spi_write_read();
		NumByteToRead--;
		pBuffer++;
	}

	/* Set chip select High at the end of the transmission */
	spi_chip_select(cs, false);
}
