#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include "uart.h"
#include "diag/Trace.h"
#include "colibritrace.h"

static UART* g_uart[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };


void UART::dma_config()
{
	UART_HandleTypeDef *huart = &this->handle_;

	if (dma_device_ == DMA2)
		__DMA2_CLK_ENABLE();
	else if (dma_device_ == DMA1)
		__DMA1_CLK_ENABLE();
	else {
		throw std::runtime_error("UART::dma_config(): Invalid DMA device");
	}

	memset(&hdma_rx_, 0, sizeof(hdma_rx_));
	memset(&hdma_tx_, 0, sizeof(hdma_tx_));
	hdma_tx_.Instance                 = tx_dma_stream_;
	hdma_tx_.Init.Channel             = tx_dma_channel_;
	hdma_tx_.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	hdma_tx_.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_tx_.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_tx_.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_tx_.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_tx_.Init.Mode                = DMA_NORMAL;
	hdma_tx_.Init.Priority            = DMA_PRIORITY_LOW;
	hdma_tx_.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_tx_.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_tx_.Init.MemBurst            = DMA_MBURST_SINGLE;
	hdma_tx_.Init.PeriphBurst         = DMA_PBURST_SINGLE;
	HAL_DMA_Init (&hdma_tx_);

	/* Associate the initialized DMA handle to the the SPI handle */
	__HAL_LINKDMA(huart, hdmatx, hdma_tx_);

	/* Configure the DMA handler for Transmission process */
	hdma_rx_.Instance                 = rx_dma_stream_;
	hdma_rx_.Init.Channel             = rx_dma_channel_;
	hdma_rx_.Init.Direction           = DMA_PERIPH_TO_MEMORY;
	hdma_rx_.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_rx_.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_rx_.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_rx_.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_rx_.Init.Mode                = DMA_CIRCULAR;
	hdma_rx_.Init.Priority            = DMA_PRIORITY_LOW;
	hdma_rx_.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_rx_.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_rx_.Init.MemBurst            = DMA_MBURST_SINGLE;
	hdma_rx_.Init.PeriphBurst         = DMA_PBURST_SINGLE;
	HAL_DMA_Init (&hdma_rx_);

	/* Associate the initialized DMA handle to the the SPI handle */
	__HAL_LINKDMA(huart, hdmarx, hdma_rx_);
}

UART::UART(const std::vector<GPIOPin>& data_pins,
		USART_TypeDef* uart_device,
		DMA_TypeDef *dma_device,
		DMA_Stream_TypeDef *tx_dma_stream,
		uint32_t tx_dma_channel,
		DMA_Stream_TypeDef *rx_dma_stream,
		uint32_t rx_dma_channel,
		uint32_t hwflowctrl,
		uint32_t baudrate,
		uint32_t timeout
		)
	: dma_device_(dma_device)
	, tx_dma_stream_(tx_dma_stream)
	, tx_dma_channel_(tx_dma_channel)
	, rx_dma_stream_(rx_dma_stream)
	, rx_dma_channel_(rx_dma_channel)
	, timeout_(timeout)
	, data_pins_(data_pins)
	, txbuf_(12)
	, rxbuf_(12)
	, cache_head_(cache_)
	, cache_tail_(cache_)
{
	for (auto& data_pin : data_pins_)
		data_pin.init();
	memset(&handle_, 0, sizeof(handle_));

	/*##-1- Configure the UART peripheral ######################################*/
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART1 configured as follow:
	 - Word Length = 8 Bits
	 - Stop Bit = One Stop bit
	 - Parity = None
	 - BaudRate = 9600 baud
	 - Hardware flow control disabled (RTS and CTS signals) */
	handle_.Instance = uart_device;
	handle_.Init.BaudRate = baudrate;
	handle_.Init.WordLength = UART_WORDLENGTH_8B;
	handle_.Init.StopBits = UART_STOPBITS_1;
	handle_.Init.Parity = UART_PARITY_NONE;
	handle_.Init.HwFlowCtl = hwflowctrl;
	handle_.Init.Mode = UART_MODE_TX_RX;
	handle_.Init.OverSampling = USART_CR1_OVER8;
	if (uart_device == USART1) {
		__USART1_CLK_ENABLE();
		HAL_NVIC_SetPriority(USART1_IRQn, 7, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		g_uart[1] = this;
	} else if (uart_device == USART2) {
		__USART2_CLK_ENABLE();
		HAL_NVIC_SetPriority(USART2_IRQn, 7, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
		g_uart[2] = this;
	} else if (uart_device == USART3) {
		__USART3_CLK_ENABLE();
		HAL_NVIC_SetPriority(USART3_IRQn, 7, 0);
		HAL_NVIC_EnableIRQ(USART3_IRQn);
		g_uart[3] = this;
	} else if (uart_device == UART4) {
		__UART4_CLK_ENABLE();
		HAL_NVIC_SetPriority(UART4_IRQn, 7, 0);
		HAL_NVIC_EnableIRQ(UART4_IRQn);
		g_uart[4] = this;
	} else if (uart_device == UART5) {
		__UART5_CLK_ENABLE();
		HAL_NVIC_SetPriority(UART5_IRQn, 7, 0);
		HAL_NVIC_EnableIRQ(UART5_IRQn);
		g_uart[5] = this;
	} else if (uart_device == USART6) {
		__USART6_CLK_ENABLE();
		HAL_NVIC_SetPriority(USART6_IRQn, 7, 0);
		HAL_NVIC_EnableIRQ(USART6_IRQn);
		g_uart[6] = this;
	}
	if (HAL_UART_Init(&handle_) != HAL_OK) {
		throw std::runtime_error("Failed to init UART");
	}
	if (dma_device_ != nullptr) {
		dma_config();
	}
}

UART::~UART()
{
	if (handle_.Instance == USART1) {
		__USART1_CLK_DISABLE();
		g_uart[1] = NULL;
	} else if (handle_.Instance == USART2) {
		__USART2_CLK_DISABLE();
		g_uart[2] = NULL;
	} else if (handle_.Instance == USART3) {
		__USART3_CLK_DISABLE();
		g_uart[3] = NULL;
	} else if (handle_.Instance == UART4) {
		__UART4_CLK_DISABLE();
		g_uart[4] = NULL;
	} else if (handle_.Instance == UART5) {
		__UART5_CLK_DISABLE();
		g_uart[5] = NULL;
	} else if (handle_.Instance == USART6) {
		__USART6_CLK_DISABLE();
		g_uart[6] = NULL;
	}
}

void UART::clear()
{
	rxbuf_.reset();
}

void UART::write(const std::string& str)
{
	write(str.c_str(), str.size());
}

size_t UART::write(const char* buf, size_t size)
{
	const char *bufptr = buf;
	size_t ret = 0, retsize = 0;

	while (size) {
		ret = transmit((uint8_t*)bufptr, size);
		size -= ret;
		bufptr += ret;
		retsize += ret;
	}
	return retsize;
}

size_t UART::readcache(char *buf, size_t size)
{
	int ret = 0;

	for (size_t i = 0; i < size && cache_tail_ < cache_head_; i++) {
		buf[i] = *cache_tail_++;
		ret++;
		if (buf[i] == '\n')
			break;
	}
	return ret;
}

size_t UART::read(char* buf, size_t size)
{
	if (__HAL_USART_GET_FLAG(&handle_, USART_FLAG_ORE) ||
			__HAL_USART_GET_FLAG(&handle_, USART_FLAG_NE) ||
			__HAL_USART_GET_FLAG(&handle_, USART_FLAG_FE) ||
			__HAL_USART_GET_FLAG(&handle_, USART_FLAG_PE)) {
#if 0
		if (__HAL_USART_GET_FLAG(&handle_, USART_FLAG_ORE))
			printf("UART: OVERRUN ERROR DETECTED\n");
		if (__HAL_USART_GET_FLAG(&handle_, USART_FLAG_FE))
			printf("UART: FRAME ERROR DETECTED\n");
		if (__HAL_USART_GET_FLAG(&handle_, USART_FLAG_PE))
			printf("UART: PARITY ERROR DETECTED\n");
		if (__HAL_USART_GET_FLAG(&handle_, USART_FLAG_NE))
			printf("UART: NOISE ERROR DETECTED\n");
#endif
		HAL_UART_DMAStop(&handle_);
		__HAL_USART_CLEAR_PEFLAG(&handle_);
		rxbuf_.reset();
		uart_dmarx_start();
		return 0;
	}

	if (cache_tail_ != cache_head_)
		return readcache(buf, size);
	receive();
	return readcache(buf, size);
}

std::string UART::read()
{
	char buf[32];
	std::string ret;
	size_t count;

	while ((count = read(buf, sizeof(buf))) > 0) {
		ret.append(buf, count);
		if (buf[count - 1] == '\n')
			break;
	}
	return ret;
}

void UART::receive()
{
	rxbuf_.reset_wp((rxbuf_.buffer_size() - handle_.hdmarx->Instance->NDTR) % rxbuf_.buffer_size());
	if (rxbuf_.empty())
		return;
	size_t readsize = rxbuf_.read_size() < sizeof(cache_) ? rxbuf_.read_size() : sizeof(cache_);
	if (readsize) {
		cache_tail_ = cache_head_ = cache_;
		memcpy(cache_, (uint8_t*)rxbuf_.get_read_ptr(), readsize);
		memset((uint8_t*)rxbuf_.get_read_ptr(), '-', readsize);
		rxbuf_.read_update(readsize);
		cache_head_ += readsize;
	}
}

size_t UART::transmit(const uint8_t* buf, size_t size)
{
	while (!txbuf_.empty())
		;
	txbuf_.reset();
	size_t writesize = txbuf_.write_size() < size ? txbuf_.write_size() : size;
	memcpy((uint8_t*)txbuf_.get_write_ptr(), buf, writesize);
	txbuf_.write_update(writesize);
	size -= writesize;
	HAL_UART_Transmit_DMA(&handle_, (uint8_t*)txbuf_.get_read_ptr(), txbuf_.read_size());
	return writesize;
}

void UART::uart_irq_handler(unsigned int device)
{
	if (g_uart[device])
		HAL_UART_IRQHandler(&g_uart[device]->handle_);
}


void UART::uart_dmatx_handler(unsigned int device)
{
	if (g_uart[device])
		HAL_DMA_IRQHandler(g_uart[device]->handle_.hdmatx);
}

void UART::uart_dmarx_handler(unsigned int device)
{
	if (g_uart[device])
		HAL_DMA_IRQHandler(g_uart[device]->handle_.hdmarx);
}

void UART::uart_dmatx_complete()
{
	txbuf_.read_update(txbuf_.read_size());
}

void UART::uart_dmatx_half_complete()
{
}

void UART::uart_dmarx_complete()
{
	rxbuf_.reset_wp((rxbuf_.buffer_size() - handle_.hdmarx->Instance->NDTR) % rxbuf_.buffer_size());
}

void UART::uart_dmarx_half_complete()
{
	rxbuf_.reset_wp((rxbuf_.buffer_size() - handle_.hdmarx->Instance->NDTR) % rxbuf_.buffer_size());
}


void UART::uart_dmatx_start()
{
}

void UART::uart_dmarx_start()
{
	HAL_UART_Receive_DMA(&handle_, (uint8_t*)rxbuf_.get_buffer_ptr(), rxbuf_.buffer_size());
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UART* uart = (UART*)huart;
	uart->uart_dmarx_complete();
}

extern "C" void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	UART* uart = (UART*)huart;
	uart->uart_dmarx_half_complete();
}

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	UART* uart = (UART*)huart;
	uart->uart_dmatx_complete();
}

extern "C" void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	UART* uart = (UART*)huart;
	uart->uart_dmatx_half_complete();
}
