/*
 * spimaster.cpp
 *
 *  Created on: Dec 23, 2014
 *      Author: mstoilov
 */

#include <cstring>
#include <stdexcept>
#include "spislave.h"
#include "diag/Trace.h"

static SPISlave* g_spislave[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

void SPISlave::vector_handler(uint8_t device)
{
	if (g_spislave[device])
		HAL_SPI_IRQHandler(&g_spislave[device]->handle_);
}

extern "C" void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	SPISlave* slave = (SPISlave*) hspi;
	for (size_t i = 0; i < sizeof(g_spislave)/sizeof(g_spislave[0]); i++) {
		if (slave == g_spislave[i]) {
			slave->RxTxError();
			break;
		}
	}
}

SPISlave::SPISlave(SPI_TypeDef* spi_device, uint32_t clk_prescale, uint32_t timeout, const std::vector<GPIOPin>& data_pins, const std::vector<GPIOPin>& cs_pins)
	: timeout_(timeout)
	, data_pins_(data_pins)
	, cs_pins_(cs_pins)
{
	for (auto& data_pin : data_pins_)
		data_pin.init();
	for (auto& cs_pin : cs_pins_) {
		cs_pin.init();
		HAL_GPIO_WritePin(cs_pin.gpio_port_, cs_pin.Pin, GPIO_PIN_SET);
	}
	memset(&handle_, 0, sizeof(handle_));
	memset(rxdata_, 0, sizeof(rxdata_));
	memset(txdata_, 0, sizeof(txdata_));
	strncpy(txdata_, "From SPI Slave", sizeof(txdata_) - 1);

	/* SPI configuration -----------------------------------------------------*/
	handle_.Instance = spi_device;
	/*
	 * if PCLK2 frequency is set to 90 MHz and clk_prescale = SPI_BAUDRATEPRESCALER_16
	 * SPI baudrate is set to 5.6 MHz (PCLK2/SPI_BaudRatePrescaler = 90/16 = 5.625 MHz)
	 */
	handle_.Init.BaudRatePrescaler = clk_prescale;
	handle_.Init.Direction = SPI_DIRECTION_2LINES;
	handle_.Init.CLKPhase = SPI_PHASE_1EDGE;
	handle_.Init.CLKPolarity = SPI_POLARITY_LOW;
	handle_.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
	handle_.Init.CRCPolynomial = 7;
	handle_.Init.DataSize = SPI_DATASIZE_8BIT;
	handle_.Init.FirstBit = SPI_FIRSTBIT_MSB;
	handle_.Init.NSS = SPI_NSS_SOFT;
	handle_.Init.TIMode = SPI_TIMODE_DISABLED;
	handle_.Init.Mode = SPI_MODE_SLAVE;

	if (spi_device == SPI1) {
		__SPI1_CLK_ENABLE();
		g_spislave[1] = this;
	} else if (spi_device == SPI2) {
		__SPI2_CLK_ENABLE();
		g_spislave[2] = this;
	} else if (spi_device == SPI3) {
		__SPI3_CLK_ENABLE();
		g_spislave[3] = this;
	} else if (spi_device == SPI4) {
		__SPI4_CLK_ENABLE();
		g_spislave[4] = this;
	} else if (spi_device == SPI5) {
		__SPI5_CLK_ENABLE();
		g_spislave[5] = this;
	} else if (spi_device == SPI6) {
		__SPI6_CLK_ENABLE();
		g_spislave[6] = this;
	}
	HAL_SPI_Init(&handle_);
}

SPISlave::~SPISlave()
{
	SPI_TypeDef* spi_device = handle_.Instance;
	if (spi_device == SPI1) {
		__SPI1_CLK_DISABLE();
		g_spislave[1] = 0;
	} else if (spi_device == SPI2) {
		__SPI2_CLK_DISABLE();
		g_spislave[2] = 0;
	} else if (spi_device == SPI3) {
		__SPI3_CLK_DISABLE();
		g_spislave[3] = 0;
	} else if (spi_device == SPI4) {
		__SPI4_CLK_DISABLE();
		g_spislave[4] = 0;
	} else if (spi_device == SPI5) {
		__SPI5_CLK_DISABLE();
		g_spislave[5] = 0;
	} else if (spi_device == SPI6) {
		__SPI6_CLK_DISABLE();
		g_spislave[6] = 0;
	}
	HAL_SPI_DeInit(&handle_);
}

void SPISlave::RxISR(struct __SPI_HandleTypeDef * hspi)
{
	SPISlave *pThis = (SPISlave*)hspi;
	pThis->RxISR();
}

void SPISlave::TxISR(struct __SPI_HandleTypeDef * hspi)
{
	SPISlave *pThis = (SPISlave*)hspi;
	pThis->TxISR();
}

void SPISlave::RxISR()
{
	trace_printf("SPISlave::RxISR ...\n");
}

void SPISlave::TxISR()
{
	trace_printf("SPISlave::TxISR ...\n");
}

void SPISlave::RxTxError()
{
	trace_printf("SPISlave::RxTxError ...\n");
}

void SPISlave::Start()
{
	HAL_SPI_TransmitReceive_IT(&handle_, (uint8_t*)txdata_, (uint8_t *)rxdata_, sizeof(txdata_));
}
