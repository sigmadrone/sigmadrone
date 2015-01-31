#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>
#include <vector>
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f429xx.h"
#include "gpiopin.h"
#include "pinnames.h"


class UART
{
public:
	UART_HandleTypeDef handle_;
	DMA_HandleTypeDef hdma_tx_;
	DMA_HandleTypeDef hdma_rx_;
	DMA_TypeDef *dma_device_;
	DMA_Stream_TypeDef *tx_dma_stream_;
	uint32_t tx_dma_channel_;
	DMA_Stream_TypeDef *rx_dma_stream_;
	uint32_t rx_dma_channel_;

public:
	UART(const std::vector<GPIOPin>& data_pins = {},
			USART_TypeDef* usart_device = USART1,
			DMA_TypeDef *dma_device = DMA2,
			DMA_Stream_TypeDef *tx_dma_stream = DMA2_Stream7,
			uint32_t tx_dma_channel = DMA_CHANNEL_4,
			DMA_Stream_TypeDef *rx_dma_stream = DMA2_Stream5,
			uint32_t rx_dma_channel = DMA_CHANNEL_4,
			uint32_t bufsize = 256,
			uint32_t timeout = 0x1000
			);
	~UART();
	void start();
	void stop();
	void transmit(const uint8_t* buf, uint32_t size);
	void receive(uint8_t* buf, uint32_t size);

protected:
	void dma_config();
};

#endif
