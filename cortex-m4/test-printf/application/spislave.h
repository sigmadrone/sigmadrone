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
	unsigned int active_rx_;
	unsigned int active_tx_;

protected:
	uint32_t timeout_;
	int cs_index_;
	std::vector<GPIOPin> data_pins_;
	char rxdata_[2][128];
	char txdata_[2][128];

	DMA_HandleTypeDef hdma_tx;
	DMA_HandleTypeDef hdma_rx;

public:
	void SPI_ResetHandle();
	void SPI_ChipSelect();
	void DMAConfig();
	void EnableEXTI(PinName pin);

public:
	SPISlave(SPI_TypeDef* spi_device = SPI5, uint32_t timeout = 0x1000, int pin_index = -1, const std::vector<GPIOPin>& data_pins = {});
	~SPISlave();
	void Start();
	void Stop();
	HAL_SPI_StateTypeDef GetState();
	HAL_SPI_ErrorTypeDef GetError();

public:
	/*
	 * This is not really public APIs.
	 */
	void RxTxError();
};

#endif /* _SPISLAVE_H_ */
