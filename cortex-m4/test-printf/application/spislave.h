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
#include "stm32f429xx.h"
#include "gpiopin.h"


class SPISlave
{
protected:
	SPI_HandleTypeDef handle_;
	uint32_t timeout_;
	std::vector<GPIOPin> data_pins_;
	std::vector<GPIOPin> cs_pins_;
	char rxdata_[32];
	char txdata_[32];

	static void RxISR(struct __SPI_HandleTypeDef * hspi); /* function pointer on Rx ISR */
	static void TxISR(struct __SPI_HandleTypeDef * hspi); /* function pointer on Tx ISR */
	void RxISR();
	void TxISR();

public:
	SPISlave(SPI_TypeDef* spi_device = SPI5, uint32_t clk_prescale = SPI_BAUDRATEPRESCALER_16, uint32_t timeout = 0x1000, const std::vector<GPIOPin>& data_pins = {}, const std::vector<GPIOPin>& cs_pins = {});
	~SPISlave();
	void Start();

public:
	/*
	 * This is not really public APIs.
	 */
	static void vector_handler(uint8_t device);
	void RxTxError();

};

#endif /* _SPISLAVE_H_ */
