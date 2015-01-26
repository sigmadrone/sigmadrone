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
	SPISlave(SPI_TypeDef* spi_device = SPI5, uint32_t bufsize = 128, uint32_t timeout = 0x1000, int pin_index = -1, const std::vector<GPIOPin>& data_pins = {});
	~SPISlave();
	void start();
	void stop();
	void transmit(const uint8_t* buf, uint32_t size);
	void receive(uint8_t* buf, uint32_t size);

	HAL_SPI_StateTypeDef GetState();
	HAL_SPI_ErrorTypeDef GetError();

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

protected:
	void reset_handle();
	void dma_config();
	void exti_config(PinName pin);

public:
	/*
	 * These are not really public APIs.
	 */
	void spi_error_callback();
	void spi_chip_select();
};

#endif /* _SPISLAVE_H_ */
