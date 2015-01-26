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
	SPI_HandleTypeDef handle_;
	uint32_t serial_;
	bool run_;
	size_t bufsize_;

protected:
	uint32_t timeout_;
	int cs_index_;
	std::vector<GPIOPin> data_pins_;
	uint8_t* rx_buffer_[2];
	uint8_t* tx_buffer_[2];

	DMA_HandleTypeDef hdma_tx;
	DMA_HandleTypeDef hdma_rx;

protected:
	void SPI_ResetHandle();
	void DMAConfig();
	void EnableEXTI(PinName pin);

public:
	SPISlave(SPI_TypeDef* spi_device = SPI5, uint32_t bufsize = 128, uint32_t timeout = 0x1000, int pin_index = -1, const std::vector<GPIOPin>& data_pins = {});
	~SPISlave();
	void SPI_ChipSelect();
	void Start();
	void Stop();
	void Transmit(const uint8_t* buf, uint32_t size);
	void Receive(uint8_t* buf, uint32_t size);

	HAL_SPI_StateTypeDef GetState();
	HAL_SPI_ErrorTypeDef GetError();

public:
	/*
	 * This is not really public APIs.
	 */
	void RxTxError();
};

#endif /* _SPISLAVE_H_ */
