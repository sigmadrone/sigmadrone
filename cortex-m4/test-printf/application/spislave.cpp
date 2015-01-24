/*
 * spimaster.cpp
 *
 *  Created on: Dec 23, 2014
 *      Author: mstoilov
 */

#include <cstring>
#include <cstdio>
#include <stdexcept>
#include "stm32f4xx_hal.h"
#include "spislave.h"
#include "diag/Trace.h"

#define SPI_TIMEOUT_VALUE  10

static SPISlave* g_spislave[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

#define __HAL_NSS_DISABLE(__HANDLE__) ((__HANDLE__)->Instance->CR1 |=  SPI_CR1_SSI)
#define __HAL_NSS_ENABLE(__HANDLE__) ((__HANDLE__)->Instance->CR1 &=  ~SPI_CR1_SSI)

#define SPIx_FORCE_RESET()               __SPI4_FORCE_RESET()
#define SPIx_RELEASE_RESET()             __SPI4_RELEASE_RESET()
#define DMAx_CLK_ENABLE()                __DMA2_CLK_ENABLE()
/* Definition for SPIx's DMA */
#define SPIx_TX_DMA_CHANNEL              DMA_CHANNEL_4
#define SPIx_TX_DMA_STREAM               DMA2_Stream1
#define SPIx_RX_DMA_CHANNEL              DMA_CHANNEL_4
#define SPIx_RX_DMA_STREAM               DMA2_Stream0

/* Definition for SPIx's NVIC */
#define SPIx_DMA_TX_IRQn                 DMA2_Stream1_IRQn
#define SPIx_DMA_RX_IRQn                 DMA2_Stream0_IRQn
#define SPIx_DMA_TX_IRQHandler           DMA2_Stream1_IRQHandler
#define SPIx_DMA_RX_IRQHandler           DMA2_Stream0_IRQHandler


extern "C" void EXTI4_IRQHandler(void)
{
	SPISlave* slave = (SPISlave*)&g_spislave[4]->handle_;
	uint16_t GPIO_Pin = ((uint16_t)1) << 4;
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != RESET) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
		if (slave)
			slave->SPI_ChipSelect();
	}
}

void SPISlave::DMAConfig()
{
	SPI_HandleTypeDef *hspi = &this->handle_;

	/* Enable DMA1 clock */
	DMAx_CLK_ENABLE();

	/*##-3- Configure the DMA streams ##########################################*/
	/* Configure the DMA handler for Transmission process */
	memset(&hdma_rx, 0, sizeof(hdma_rx));
	memset(&hdma_tx, 0, sizeof(hdma_tx));
	hdma_tx.Instance = SPIx_TX_DMA_STREAM;

	hdma_tx.Init.Channel = SPIx_TX_DMA_CHANNEL;
	hdma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_tx.Init.Mode = DMA_NORMAL;
	hdma_tx.Init.Priority = DMA_PRIORITY_LOW;
	hdma_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdma_tx.Init.MemBurst = DMA_MBURST_INC4;
	hdma_tx.Init.PeriphBurst = DMA_PBURST_INC4;

	HAL_DMA_Init (&hdma_tx);

	/* Associate the initialized DMA handle to the the SPI handle */
	__HAL_LINKDMA(hspi, hdmatx, hdma_tx);

	/* Configure the DMA handler for Transmission process */
	hdma_rx.Instance = SPIx_RX_DMA_STREAM;

	hdma_rx.Init.Channel = SPIx_RX_DMA_CHANNEL;
	hdma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_rx.Init.Mode = DMA_NORMAL;
	hdma_rx.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdma_rx.Init.MemBurst = DMA_MBURST_INC4;
	hdma_rx.Init.PeriphBurst = DMA_PBURST_INC4;

	HAL_DMA_Init (&hdma_rx);

	/* Associate the initialized DMA handle to the the SPI handle */
	__HAL_LINKDMA(hspi, hdmarx, hdma_rx);

	/*##-4- Configure the NVIC for DMA #########################################*/
	/* NVIC configuration for DMA transfer complete interrupt (SPI3_TX) */
	HAL_NVIC_SetPriority(SPIx_DMA_TX_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ (SPIx_DMA_TX_IRQn);

	/* NVIC configuration for DMA transfer complete interrupt (SPI3_RX) */
	HAL_NVIC_SetPriority(SPIx_DMA_RX_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ (SPIx_DMA_RX_IRQn);
}

/**
 * @brief  This function handles DMA Rx interrupt request.
 * @param  None
 * @retval None
 */
extern "C" void SPIx_DMA_RX_IRQHandler(void)
{
//	NVIC_ClearPendingIRQ(SPIx_DMA_RX_IRQn);
	HAL_DMA_IRQHandler(g_spislave[4]->handle_.hdmarx);
}

/**
 * @brief  This function handles DMA Tx interrupt request.
 * @param  None
 * @retval None
 */
extern "C" void SPIx_DMA_TX_IRQHandler(void)
{
//	NVIC_ClearPendingIRQ(SPIx_DMA_TX_IRQn);
	HAL_DMA_IRQHandler(g_spislave[4]->handle_.hdmatx);
}


extern "C" void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	SPISlave* slave = (SPISlave*) hspi;
	for (size_t i = 0; i < sizeof(g_spislave)/sizeof(g_spislave[0]); i++) {
		if (slave == g_spislave[i]) {
			slave->RxTxError();
			break;
		}
	}
}

SPISlave::SPISlave(SPI_TypeDef* spi_device, uint32_t timeout, int cs_index, const std::vector<GPIOPin>& data_pins)
	: serial_(0)
	, run_(false)
	, active_rx_(0)
	, active_tx_(0)
	, timeout_(timeout)
	, cs_index_(cs_index)
	, data_pins_(data_pins)
{
	for (auto& data_pin : data_pins_)
		data_pin.init();
	if (cs_index_ >= 0)
		EnableEXTI(data_pins_[cs_index_].pn_);
	HAL_NVIC_SetPriority(EXTI4_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	memset(&handle_, 0, sizeof(handle_));
	memset(rxdata_, 0, sizeof(rxdata_));
	memset(txdata_, 0, sizeof(txdata_));

	/* SPI configuration -----------------------------------------------------*/
	handle_.Instance = spi_device;
	handle_.Init.BaudRatePrescaler = 0;
	handle_.Init.Direction = SPI_DIRECTION_2LINES;
	handle_.Init.CLKPhase = SPI_PHASE_1EDGE;
	handle_.Init.CLKPolarity = SPI_POLARITY_LOW;
	handle_.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
	handle_.Init.CRCPolynomial = 7;
	handle_.Init.DataSize = SPI_DATASIZE_8BIT;
	handle_.Init.FirstBit = SPI_FIRSTBIT_MSB;
	handle_.Init.NSS = SPI_NSS_HARD_INPUT;
	handle_.Init.TIMode = SPI_TIMODE_DISABLED;
	handle_.Init.Mode = SPI_MODE_SLAVE;

	if (spi_device == SPI1) {
		__SPI1_CLK_ENABLE();
		g_spislave[1] = this;
	} else if (spi_device == SPI2) {
		__SPI2_CLK_ENABLE();
		g_spislave[2] = this;
	} else if (spi_device == SPI3) {
		__SPI3_CLK_ENABLE();
		g_spislave[3] = this;
	} else if (spi_device == SPI4) {
		__SPI4_CLK_ENABLE();
		g_spislave[4] = this;
	} else if (spi_device == SPI5) {
		__SPI5_CLK_ENABLE();
		g_spislave[5] = this;
	} else if (spi_device == SPI6) {
		__SPI6_CLK_ENABLE();
		g_spislave[6] = this;
	}
	HAL_SPI_Init(&handle_);
	DMAConfig();
}

SPISlave::~SPISlave()
{
	SPI_TypeDef* spi_device = handle_.Instance;
	if (spi_device == SPI1) {
		__SPI1_CLK_DISABLE();
		g_spislave[1] = 0;
	} else if (spi_device == SPI2) {
		__SPI2_CLK_DISABLE();
		g_spislave[2] = 0;
	} else if (spi_device == SPI3) {
		__SPI3_CLK_DISABLE();
		g_spislave[3] = 0;
	} else if (spi_device == SPI4) {
		__SPI4_CLK_DISABLE();
		g_spislave[4] = 0;
	} else if (spi_device == SPI5) {
		__SPI5_CLK_DISABLE();
		g_spislave[5] = 0;
	} else if (spi_device == SPI6) {
		__SPI6_CLK_DISABLE();
		g_spislave[6] = 0;
	}
	HAL_SPI_DeInit(&handle_);
}

void SPISlave::EnableEXTI(PinName pin)
{
	uint32_t position = STM_PIN(pin);
	uint32_t ioposition = ((uint32_t)0x01) << position;
	uint32_t temp = 0x00;

	/* Enable SYSCFG Clock */
	__SYSCFG_CLK_ENABLE();

	temp = SYSCFG->EXTICR[position >> 2];
	temp &= ~(((uint32_t) 0x0F) << (4 * (position & 0x03)));
	temp |= ((uint32_t) (STM_PORT(pin)) << (4 * (position & 0x03)));
	SYSCFG->EXTICR[position >> 2] = temp;

	/* Clear EXTI line configuration */
	temp = EXTI->IMR;
	temp &= ~((uint32_t) ioposition);
	temp |= ioposition;
	EXTI->IMR = temp;

	/* Clear EVT configuration */
	temp = EXTI->EMR;
	temp &= ~((uint32_t) ioposition);
	EXTI->EMR = temp;

	/* Clear Rising edge configuration */
	temp = EXTI->RTSR;
	temp &= ~((uint32_t) ioposition);
	temp |= ioposition;
	EXTI->RTSR = temp;

//	/* Clear Falling edge configuration */
//	temp = EXTI->FTSR;
//	temp &= ~((uint32_t) ioposition);
//	temp |= ioposition;
//	EXTI->FTSR = temp;
}

void SPISlave::RxTxError()
{
//	trace_printf("SPISlave::RxTxError ...\n");
}

void SPISlave::Start()
{
	run_ = true;
	SPI_ChipSelect();
}

void SPISlave::Stop()
{
	run_ = false;
	SPI_ChipSelect();
}


void SPISlave::SPI_ChipSelect()
{
	int active_rx = active_rx_ % 2;
	int active_tx = active_tx_ % 2;
	HAL_DMA_Abort(handle_.hdmarx);
	HAL_DMA_Abort(handle_.hdmatx);
	SPI_ResetHandle();
	HAL_DMA_Init (&hdma_rx);
	HAL_DMA_Init (&hdma_tx);
	if (!run_)
		return;
	memset(txdata_, 0, sizeof(txdata_));
	snprintf(txdata_[active_tx], sizeof(txdata_[0]) - 1, "SPI:%u*******************", (unsigned int)serial_++);
//			trace_printf("SPISlave::Start: %s\n", txdata_);
	handle_.Instance->DR = *((uint8_t*)txdata_[active_tx]);
	HAL_SPI_TransmitReceive_DMA(&handle_, (uint8_t*)txdata_[active_tx] + 1, (uint8_t *)rxdata_[active_rx], sizeof(rxdata_[0]) - 1);
}

/**
  * @brief  Return the SPI state
  * @param  None
  * @retval HAL state
  */
HAL_SPI_StateTypeDef SPISlave::GetState()
{
	return handle_.State;
}

/**
  * @brief  Return the SPI error code
  * @param  None
  * @retval SPI Error Code
  */
HAL_SPI_ErrorTypeDef SPISlave::GetError()
{
	return handle_.ErrorCode;
}

/**
* @brief  Interrupt Handler to close Tx transfer
* @param  None
* @retval void
*/
void SPISlave::SPI_ResetHandle()
{
	SPI_HandleTypeDef *hspi = &this->handle_;

	/* Disable TXE interrupt */
	__HAL_SPI_DISABLE_IT(hspi, (SPI_IT_TXE ));

	/* Disable RXNE and ERR interrupt */
	__HAL_SPI_DISABLE_IT(hspi, (SPI_IT_RXNE));

	/* Disable ERR interrupt */
	__HAL_SPI_DISABLE_IT(hspi, (SPI_IT_ERR));

	__HAL_SPI_CLEAR_OVRFLAG(hspi);

	/* Reset CRC Calculation */
	__HAL_SPI_RESET_CRC(hspi);

	/* Disable SPI peripheral */
	__HAL_SPI_DISABLE(hspi);

	handle_.State = HAL_SPI_STATE_READY;
}
