/*
 * spimaster.cpp
 *
 *  Created on: Dec 23, 2014
 *      Author: mstoilov
 */

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include "stm32f4xx_hal.h"
#include "spislave.h"
#include "diag/Trace.h"

#define SPI_TIMEOUT_VALUE  10

static SPISlave* g_spislave[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

//#define SPIx_FORCE_RESET()               __SPI4_FORCE_RESET()
//#define SPIx_RELEASE_RESET()             __SPI4_RELEASE_RESET()

void SPISlave::dma_config()
{
	SPI_HandleTypeDef *hspi = &this->handle_;

	if (dma_device_ == DMA2)
		__DMA2_CLK_ENABLE();
	else if (dma_device_ == DMA1)
		__DMA1_CLK_ENABLE();
	else
		throw std::runtime_error("SPISlave::dma_config(): Invalid DMA device");

	/*##-3- Configure the DMA streams ##########################################*/
	/* Configure the DMA handler for Transmission process */
	memset(&hdma_rx_, 0, sizeof(hdma_rx_));
	memset(&hdma_tx_, 0, sizeof(hdma_tx_));
	hdma_tx_.Instance = tx_dma_stream_;

	hdma_tx_.Init.Channel = tx_dma_channel_;
	hdma_tx_.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_tx_.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tx_.Init.MemInc = DMA_MINC_ENABLE;
	hdma_tx_.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_tx_.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_tx_.Init.Mode = DMA_NORMAL;
	hdma_tx_.Init.Priority = DMA_PRIORITY_LOW;
	hdma_tx_.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_tx_.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdma_tx_.Init.MemBurst = DMA_MBURST_INC4;
	hdma_tx_.Init.PeriphBurst = DMA_PBURST_INC4;

	HAL_DMA_Init (&hdma_tx_);

	/* Associate the initialized DMA handle to the the SPI handle */
	__HAL_LINKDMA(hspi, hdmatx, hdma_tx_);

	/* Configure the DMA handler for Transmission process */
	hdma_rx_.Instance = rx_dma_stream_;

	hdma_rx_.Init.Channel = rx_dma_channel_;
	hdma_rx_.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_rx_.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_rx_.Init.MemInc = DMA_MINC_ENABLE;
	hdma_rx_.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_rx_.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_rx_.Init.Mode = DMA_NORMAL;
	hdma_rx_.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_rx_.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_rx_.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdma_rx_.Init.MemBurst = DMA_MBURST_INC4;
	hdma_rx_.Init.PeriphBurst = DMA_PBURST_INC4;

	HAL_DMA_Init (&hdma_rx_);

	/* Associate the initialized DMA handle to the the SPI handle */
	__HAL_LINKDMA(hspi, hdmarx, hdma_rx_);
}

extern "C" void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	SPISlave* slave = (SPISlave*) hspi;
	for (size_t i = 0; i < sizeof(g_spislave)/sizeof(g_spislave[0]); i++) {
		if (slave == g_spislave[i]) {
			slave->spi_error_callback();
			break;
		}
	}
}

void SPISlave::spi_chipselect_handler(unsigned int device)
{
	SPISlave* slave = (SPISlave*)&g_spislave[device]->handle_;
	uint16_t GPIO_Pin = ((uint16_t)1) << device;
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != RESET) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
		if (slave)
			slave->spi_chipselect(false);
	}
}

void SPISlave::spi_dmatx_handler(unsigned int device)
{
	if (g_spislave[device])
		HAL_DMA_IRQHandler(g_spislave[device]->handle_.hdmatx);
}

void SPISlave::spi_dmarx_handler(unsigned int device)
{
	if (g_spislave[device])
		HAL_DMA_IRQHandler(g_spislave[device]->handle_.hdmarx);
}


SPISlave::SPISlave(
		const std::vector<GPIOPin>& data_pins,
		int cs_index,
		SPI_TypeDef* spi_device,
		DMA_TypeDef *dma_device,
		DMA_Stream_TypeDef *tx_dma_stream,
		uint32_t tx_dma_channel,
		DMA_Stream_TypeDef *rx_dma_stream,
		uint32_t rx_dma_channel,
		uint32_t bufsize,
		uint32_t timeout)
	: serial_(0)
	, run_(false)
	, bufsize_(bufsize)
	, timeout_(timeout)
	, cs_index_(cs_index)
	, data_pins_(data_pins)
	, dma_device_(dma_device)
	, tx_dma_stream_(tx_dma_stream)
	, tx_dma_channel_(tx_dma_channel)
	, rx_dma_stream_(rx_dma_stream)
	, rx_dma_channel_(rx_dma_channel)
{
	for (auto& data_pin : data_pins_)
		data_pin.init();
	if (cs_index_ >= 0)
		exti_config(data_pins_[cs_index_].pn_);

	memset(&handle_, 0, sizeof(handle_));
	rx_buffer_[0] = (uint8_t*)malloc(bufsize_);
	rx_buffer_[1] = (uint8_t*)malloc(bufsize_);
	tx_buffer_[0] = (uint8_t*)malloc(bufsize_);
	tx_buffer_[1] = (uint8_t*)malloc(bufsize_);
	memset(rx_buffer_[0], 0, bufsize_);
	memset(rx_buffer_[1], 0, bufsize_);
	memset(tx_buffer_[0], 0, bufsize_);
	memset(tx_buffer_[1], 0, bufsize_);

	/* SPI configuration -----------------------------------------------------*/
	handle_.Instance = spi_device;
	handle_.Init.BaudRatePrescaler = 0;
	handle_.Init.Direction = SPI_DIRECTION_2LINES;
	handle_.Init.CLKPhase = SPI_PHASE_1EDGE;
	handle_.Init.CLKPolarity = SPI_POLARITY_LOW;
	handle_.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
	handle_.Init.CRCPolynomial = 7;
	handle_.Init.DataSize = SPI_DATASIZE_8BIT;
	handle_.Init.FirstBit = SPI_FIRSTBIT_MSB;
	handle_.Init.NSS = SPI_NSS_HARD_INPUT;
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
	dma_config();
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

void SPISlave::exti_config(PinName pin)
{
	uint32_t position = STM_PIN(pin);
	uint32_t ioposition = ((uint32_t)0x01) << position;
	uint32_t temp = 0x00;

	/* Enable SYSCFG Clock */
	__SYSCFG_CLK_ENABLE();

	temp = SYSCFG->EXTICR[position >> 2];
	temp &= ~(((uint32_t) 0x0F) << (4 * (position & 0x03)));
	temp |= ((uint32_t) (STM_PORT(pin)) << (4 * (position & 0x03)));
	SYSCFG->EXTICR[position >> 2] = temp;

	/* Clear EXTI line configuration */
	temp = EXTI->IMR;
	temp &= ~((uint32_t) ioposition);
	temp |= ioposition;
	EXTI->IMR = temp;

	/* Clear EVT configuration */
	temp = EXTI->EMR;
	temp &= ~((uint32_t) ioposition);
	EXTI->EMR = temp;

	/* Clear Rising edge configuration */
	temp = EXTI->RTSR;
	temp &= ~((uint32_t) ioposition);
	temp |= ioposition;
	EXTI->RTSR = temp;

//	/* Clear Falling edge configuration */
//	temp = EXTI->FTSR;
//	temp &= ~((uint32_t) ioposition);
//	temp |= ioposition;
//	EXTI->FTSR = temp;
}

void SPISlave::spi_error_callback()
{
//	trace_printf("SPISlave::RxTxError ...\n");
}

void SPISlave::start()
{
	run_ = true;
	spi_chipselect(false);
}

void SPISlave::stop()
{
	run_ = false;
	spi_chipselect(false);
}


void SPISlave::spi_chipselect(bool select)
{
	if (select == false) {
		HAL_DMA_Abort(handle_.hdmarx);
		HAL_DMA_Abort(handle_.hdmatx);
		reset_handle();
		HAL_DMA_Init (&hdma_rx_);
		HAL_DMA_Init (&hdma_tx_);
		if (!run_)
			return;
//		memset(tx_buffer_[active_tx], 0, bufsize_);
//		snprintf((char*)tx_buffer_[active_tx], bufsize_ - 1, "SPI:%u*******************", (unsigned int)serial_++);
//		trace_printf("SPISlave::Start: %s\n", (char*)tx_buffer_[active_tx]);
		memcpy(tx_buffer_[1], tx_buffer_[0], bufsize_);
		memcpy(rx_buffer_[0], rx_buffer_[1], bufsize_);
		handle_.Instance->DR = *((uint8_t*)tx_buffer_[1]);
		HAL_SPI_TransmitReceive_DMA(&handle_, ((uint8_t*)tx_buffer_[1]) + 1, ((uint8_t*)rx_buffer_[1]), bufsize_);
	}
}

void SPISlave::transmit(const uint8_t* buf, uint32_t size)
{
	if (size > bufsize_)
		throw std::range_error("SPISlave::Transmit size error");
	__disable_irq();
	memcpy(tx_buffer_[0], buf, size);
	__enable_irq();
}

void SPISlave::receive(uint8_t* buf, uint32_t size)
{
	if (size > bufsize_)
		throw std::range_error("SPISlave::Transmit size error");
	__disable_irq();
	memcpy(buf, rx_buffer_[0], size);
	__enable_irq();
}

/**
* @brief  Interrupt Handler to close Tx transfer
* @param  None
* @retval void
*/
void SPISlave::reset_handle()
{
	SPI_HandleTypeDef *hspi = &this->handle_;

	/* Disable TXE interrupt */
	__HAL_SPI_DISABLE_IT(hspi, (SPI_IT_TXE ));

	/* Disable RXNE and ERR interrupt */
	__HAL_SPI_DISABLE_IT(hspi, (SPI_IT_RXNE));

	/* Disable ERR interrupt */
	__HAL_SPI_DISABLE_IT(hspi, (SPI_IT_ERR));

	__HAL_SPI_CLEAR_OVRFLAG(hspi);

	/* Reset CRC Calculation */
	__HAL_SPI_RESET_CRC(hspi);

	/* Disable SPI peripheral */
	__HAL_SPI_DISABLE(hspi);

	handle_.State = HAL_SPI_STATE_READY;
}
