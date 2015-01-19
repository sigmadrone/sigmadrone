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
#include "digitalin.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f429xx.h"
#include "gpiopin.h"


class SPISlave
{
public:
	SPI_HandleTypeDef handle_;

protected:
	uint32_t timeout_;
	std::vector<GPIOPin> data_pins_;
	std::vector<GPIOPin> cs_pins_;
	char rxdata_[32];
	char txdata_[32];
	DigitalIn *cs_interrupt_;

	DMA_HandleTypeDef hdma_tx;
	DMA_HandleTypeDef hdma_rx;

public:
	void SPI_TxISR();
	void SPI_RxISR();
	void SPI_RxCloseIRQHandler();
	void SPI_TxCloseIRQHandler();
	void SPI_ResetHandle();
	HAL_StatusTypeDef SPI_WaitOnFlagUntilTimeout(uint32_t Flag, FlagStatus Status, uint32_t Timeout);
	void SPI_ChipSelect();
	void DMAConfig();

public:
	SPISlave(SPI_TypeDef* spi_device = SPI5, uint32_t clk_prescale = SPI_BAUDRATEPRESCALER_16, uint32_t timeout = 0x1000, const std::vector<GPIOPin>& data_pins = {}, const std::vector<GPIOPin>& cs_pins = {});
	~SPISlave();
	void Start();
	HAL_SPI_StateTypeDef GetState();
	HAL_SPI_ErrorTypeDef GetError();
	HAL_StatusTypeDef TransmitReceive_IT(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);

public:
	/*
	 * This is not really public APIs.
	 */
	static void vector_handler(uint8_t device);
	static void IRQHandler(SPI_HandleTypeDef *hspi);
	void RxTxError();
};

#endif /* _SPISLAVE_H_ */
