/*
 * spislave.h
 *
 *  Created on: Dec 23, 2014
 *      Author: mstoilov
 */

#ifndef _SPISLAVE_H_
#define _SPISLAVE_H_

#include <stdint.h>
#include <vector>
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f429xx.h"
#include "gpiopin.h"
#include "pinnames.h"


class SPISlave
{
public:
	SPISlave(const std::vector<GPIOPin>& data_pins = {},
			int cs_index = -1,
			SPI_TypeDef* spi_device = SPI4,
			DMA_TypeDef *dma_device = DMA2,
			DMA_Stream_TypeDef *tx_dma_stream = DMA2_Stream1,
			uint32_t tx_dma_channel = DMA_CHANNEL_4,
			DMA_Stream_TypeDef *rx_dma_stream = DMA2_Stream0,
			uint32_t rx_dma_channel = DMA_CHANNEL_4,
			uint32_t bufsize = 256,
			uint32_t timeout = 0x1000
			);
	~SPISlave();
	void start();
	void stop();
	void transmit(const uint8_t* buf, uint32_t size);
	void receive(uint8_t* buf, uint32_t size);

public:
	SPI_HandleTypeDef handle_;

protected:
	uint32_t serial_;
	bool run_;
	size_t bufsize_;
	uint32_t timeout_;
	int cs_index_;
	std::vector<GPIOPin> data_pins_;
	uint8_t* rx_buffer_[2];
	uint8_t* tx_buffer_[2];
	DMA_HandleTypeDef hdma_tx_;
	DMA_HandleTypeDef hdma_rx_;
	DMA_TypeDef *dma_device_;
	DMA_Stream_TypeDef *tx_dma_stream_;
	uint32_t tx_dma_channel_;
	DMA_Stream_TypeDef *rx_dma_stream_;
	uint32_t rx_dma_channel_;


protected:
	void reset_handle();
	void dma_config();
	void exti_config(PinName pin);

public:
	/*
	 * These are not really public APIs.
	 */
	void spi_error_callback();
	void spi_chipselect(bool select);

	static void spi_chipselect_handler(unsigned int device, bool select);
	static void spi_dmatx_handler(unsigned int device);
	static void spi_dmarx_handler(unsigned int device);

};

#endif /* _SPISLAVE_H_ */
