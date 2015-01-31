#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include "uart.h"

void UART::dma_config()
{
	UART_HandleTypeDef *huart = &this->handle_;

	if (dma_device_ == DMA2)
		__DMA2_CLK_ENABLE();
	else if (dma_device_ == DMA1)
		__DMA1_CLK_ENABLE();
	else
		throw std::runtime_error("UART::dma_config(): Invalid DMA device");

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
	__HAL_LINKDMA(huart, hdmatx, hdma_tx_);

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
	__HAL_LINKDMA(huart, hdmarx, hdma_rx_);
}



UART::UART(const std::vector<GPIOPin>& data_pins,
		USART_TypeDef* usart_device,
		DMA_TypeDef *dma_device,
		DMA_Stream_TypeDef *tx_dma_stream,
		uint32_t tx_dma_channel,
		DMA_Stream_TypeDef *rx_dma_stream,
		uint32_t rx_dma_channel,
		uint32_t bufsize,
		uint32_t timeout)
{

}

UART::~UART()
{

}

void UART::start()
{

}

void UART::stop()
{

}

void UART::transmit(const uint8_t* buf, uint32_t size)
{

}

void UART::receive(uint8_t* buf, uint32_t size)
{

}
