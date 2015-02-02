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
#include "circbuffer.h"

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
	size_t bufsize_;
	uint32_t timeout_;
	std::vector<GPIOPin> data_pins_;
	uint8_t* rx_buffer_[2];
	uint8_t* tx_buffer_[2];
	CircBuffer txbuf_;
	CircBuffer rxbuf_;

public:
	UART(const std::vector<GPIOPin>& data_pins = {},
			USART_TypeDef* uart_device = USART1,
			DMA_TypeDef *dma_device = DMA2,
			DMA_Stream_TypeDef *tx_dma_stream = DMA2_Stream7,
			uint32_t tx_dma_channel = DMA_CHANNEL_4,
			DMA_Stream_TypeDef *rx_dma_stream = DMA2_Stream5,
			uint32_t rx_dma_channel = DMA_CHANNEL_4,
			uint32_t bufsize = 64,
			uint32_t timeout = 250
			);
	~UART();
	size_t transmit(const uint8_t* buf, size_t size);
	size_t receive(uint8_t* buf, size_t size);
	static void uart_dmatx_handler(unsigned int device);
	static void uart_dmarx_handler(unsigned int device);
	HAL_UART_StateTypeDef get_state();
	void uart_dmatx_complete();
	void uart_dmarx_complete();
	void uart_dmatx_half_complete();
	void uart_dmarx_half_complete();
	void uart_dmatx_start();
	void uart_dmarx_start();

protected:
	void dma_config();
};

#endif
