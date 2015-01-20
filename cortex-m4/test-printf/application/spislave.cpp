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


//extern "C" void EXTI4_IRQHandler(void)
//{
//	SPISlave* slave = (SPISlave*)&g_spislave[4]->handle_;
//	uint16_t GPIO_Pin = ((uint16_t)1) << 4;
//	if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != RESET) {
//		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
//		if (slave)
//			slave->SPI_ChipSelect();
//	}
//}


void SPISlave::vector_handler(uint8_t device)
{
	if (g_spislave[device])
		IRQHandler(&g_spislave[device]->handle_);
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
	HAL_NVIC_SetPriority(SPIx_DMA_TX_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ (SPIx_DMA_TX_IRQn);

	/* NVIC configuration for DMA transfer complete interrupt (SPI3_RX) */
	HAL_NVIC_SetPriority(SPIx_DMA_RX_IRQn, 0, 0);
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
	((SPISlave*)&g_spislave[4]->handle_)->SPI_ResetHandle();
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

SPISlave::SPISlave(SPI_TypeDef* spi_device, uint32_t clk_prescale, uint32_t timeout, const std::vector<GPIOPin>& data_pins, const std::vector<GPIOPin>& cs_pins)
	: timeout_(timeout)
	, data_pins_(data_pins)
	, cs_pins_(cs_pins)
	, cs_interrupt_(NULL) //new DigitalIn(PE_4, DigitalIn::PullNone, DigitalIn::InterruptRisingFalling))
{
	for (auto& data_pin : data_pins_)
		data_pin.init();
	for (auto& cs_pin : cs_pins_) {
		cs_pin.init();
		HAL_GPIO_WritePin(cs_pin.gpio_port_, cs_pin.Pin, GPIO_PIN_SET);
	}
//	EnableEXTI(PE_4);
//	HAL_NVIC_SetPriority(EXTI4_IRQn, 15, 0);
//	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	memset(&handle_, 0, sizeof(handle_));
	memset(rxdata_, 0, sizeof(rxdata_));
	memset(txdata_, 0, sizeof(txdata_));
	strncpy(txdata_, "From SPI Slave", sizeof(txdata_) - 1);
//	cs_interrupt_->callback(this, &SPISlave::SPI_ChipSelect);

	/* SPI configuration -----------------------------------------------------*/
	handle_.Instance = spi_device;
	/*
	 * if PCLK2 frequency is set to 90 MHz and clk_prescale = SPI_BAUDRATEPRESCALER_16
	 * SPI baudrate is set to 5.6 MHz (PCLK2/SPI_BaudRatePrescaler = 90/16 = 5.625 MHz)
	 */
	handle_.Init.BaudRatePrescaler = clk_prescale;
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

	/*##-3- Configure the NVIC for SPI #########################################*/
	/* NVIC for SPI */
	HAL_NVIC_SetPriority(SPI4_IRQn, 15, 1);
	HAL_NVIC_EnableIRQ(SPI4_IRQn);
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
//	delete cs_interrupt_;
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
	trace_printf("SPISlave::RxTxError ...\n");
}

void SPISlave::Start()
{
	int i = 0;
	while (1) {
		if (HAL_SPI_GetState(&handle_) == HAL_SPI_STATE_READY) {
			memset(txdata_, 0, sizeof(txdata_));
			snprintf(txdata_, sizeof(txdata_) - 1, "From SPI:%d*******", i++);
			trace_printf("SPISlave::Start: %s\n", txdata_);
			HAL_SPI_TransmitReceive_DMA(&handle_, (uint8_t*)txdata_, (uint8_t *)rxdata_, 15);
		}
	}
}

void SPISlave::SPI_ChipSelect()
{
//	SPI_ResetHandle();
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
  * @brief This function handles SPI Communication Timeout.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *                the configuration information for SPI module.
  * @retval HAL status
  */
HAL_StatusTypeDef SPISlave::SPI_WaitOnFlagUntilTimeout(uint32_t Flag, FlagStatus Status, uint32_t Timeout)
{
	SPI_HandleTypeDef *hspi = &this->handle_;
	uint32_t tickstart = 0;

	/* Get tick */
	tickstart = HAL_GetTick();

	/* Wait until flag is set */
	if (Status == RESET) {
		while (__HAL_SPI_GET_FLAG(hspi, Flag) == RESET) {
			if (Timeout != HAL_MAX_DELAY) {
				if ((Timeout == 0) || ((HAL_GetTick() - tickstart) > Timeout)) {
					/* Disable the SPI and reset the CRC: the CRC value should be cleared
					 on both master and slave sides in order to resynchronize the master
					 and slave for their respective CRC calculation */

					/* Disable TXE, RXNE and ERR interrupts for the interrupt process */
					__HAL_SPI_DISABLE_IT(hspi,
							(SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));

					/* Disable SPI peripheral */
					__HAL_SPI_DISABLE(hspi);

					/* Reset CRC Calculation */
					if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLED) {
						__HAL_SPI_RESET_CRC(hspi);
					}

					hspi->State = HAL_SPI_STATE_READY;

					/* Process Unlocked */
					__HAL_UNLOCK(hspi);

					return HAL_TIMEOUT;
				}
			}
		}
	} else {
		while (__HAL_SPI_GET_FLAG(hspi, Flag) != RESET) {
			if (Timeout != HAL_MAX_DELAY) {
				if ((Timeout == 0) || ((HAL_GetTick() - tickstart) > Timeout)) {
					/* Disable the SPI and reset the CRC: the CRC value should be cleared
					 on both master and slave sides in order to resynchronize the master
					 and slave for their respective CRC calculation */

					/* Disable TXE, RXNE and ERR interrupts for the interrupt process */
					__HAL_SPI_DISABLE_IT(hspi,
							(SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));

					/* Disable SPI peripheral */
					__HAL_SPI_DISABLE(hspi);

					/* Reset CRC Calculation */
					if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLED) {
						__HAL_SPI_RESET_CRC(hspi);
					}

					hspi->State = HAL_SPI_STATE_READY;

					/* Process Unlocked */
					__HAL_UNLOCK(hspi);

					return HAL_TIMEOUT;
				}
			}
		}
	}
	return HAL_OK;
}


void SPISlave::SPI_TxISR()
{
	SPI_HandleTypeDef *hspi = &this->handle_;

	if (hspi->TxXferCount == 0) {
		SPI_TxCloseIRQHandler();
		return;
	}

	/* Transmit data in 8 Bit mode */
	if (hspi->Init.DataSize == SPI_DATASIZE_8BIT) {
		hspi->Instance->DR = (*hspi->pTxBuffPtr++);
	}
	/* Transmit data in 16 Bit mode */
	else {
		hspi->Instance->DR = *((uint16_t*) hspi->pTxBuffPtr);
		hspi->pTxBuffPtr += 2;
	}
	hspi->TxXferCount--;

	if (hspi->TxXferCount == 0) {
		if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLED) {
			/* calculate and transfer CRC on Tx line */
			hspi->Instance->CR1 |= SPI_CR1_CRCNEXT;
		}
	}
}

/**
  * @brief  Interrupt Handler to receive amount of data in no-blocking mode
  * @param  None
  * @retval void
  */
void SPISlave::SPI_RxISR()
{
	SPI_HandleTypeDef *hspi = &this->handle_;
	/* Receive data in 8 Bit mode */
	if (hspi->Init.DataSize == SPI_DATASIZE_8BIT) {
		(*hspi->pRxBuffPtr++) = hspi->Instance->DR;
	}
	/* Receive data in 16 Bit mode */
	else {
		*((uint16_t*) hspi->pRxBuffPtr) = hspi->Instance->DR;
		hspi->pRxBuffPtr += 2;
	}
	hspi->RxXferCount--;

	/* Enable CRC Transmission */
	if ((hspi->RxXferCount == 1) && (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLED)) {
		/* Set CRC Next to calculate CRC on Rx side */
		hspi->Instance->CR1 |= SPI_CR1_CRCNEXT;
	}

	/* Enable CRC Transmission */
	if ((hspi->RxXferCount == 1) && (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLED)) {
		/* Set CRC Next to calculate CRC on Rx side */
		hspi->Instance->CR1 |= SPI_CR1_CRCNEXT;
	}

	if (hspi->RxXferCount == 0) {
		SPI_RxCloseIRQHandler();
	}
}

/**
  * @brief  Interrupt Handler to close Rx transfer
  * @param  None
  * @retval void
  */
void SPISlave::SPI_RxCloseIRQHandler()
{
	SPI_HandleTypeDef *hspi = &this->handle_;
	__IO uint16_t tmpreg;

	if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLED) {
		/* Wait until RXNE flag is set to send data */
		if (SPI_WaitOnFlagUntilTimeout(SPI_FLAG_RXNE, RESET, SPI_TIMEOUT_VALUE) != HAL_OK) {
			hspi->ErrorCode = (HAL_SPI_ErrorTypeDef)((unsigned int)hspi->ErrorCode | (unsigned int)HAL_SPI_ERROR_FLAG);
		}

		/* Read CRC to reset RXNE flag */
		tmpreg = hspi->Instance->DR;

		/* Wait until RXNE flag is set to send data */
		if (SPI_WaitOnFlagUntilTimeout(SPI_FLAG_RXNE, SET, SPI_TIMEOUT_VALUE) != HAL_OK) {
			hspi->ErrorCode = (HAL_SPI_ErrorTypeDef)((unsigned int)hspi->ErrorCode | (unsigned int)HAL_SPI_ERROR_FLAG);
		}

		/* Check if CRC error occurred */
		if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_CRCERR) != RESET) {
			hspi->ErrorCode = (HAL_SPI_ErrorTypeDef)((unsigned int)hspi->ErrorCode | (unsigned int)HAL_SPI_ERROR_CRC);

			/* Reset CRC Calculation */
			__HAL_SPI_RESET_CRC(hspi);
		}
	}

	/* Disable RXNE and ERR interrupt */
	__HAL_SPI_DISABLE_IT(hspi, (SPI_IT_RXNE));

	/* if Transmit process is finished */
	if (__HAL_SPI_GET_IT_SOURCE(hspi, SPI_IT_TXE) == RESET) {
		/* Disable ERR interrupt */
		__HAL_SPI_DISABLE_IT(hspi, (SPI_IT_ERR));

		if ((hspi->Init.Mode == SPI_MODE_MASTER)
				&& ((hspi->Init.Direction == SPI_DIRECTION_1LINE)
				|| (hspi->Init.Direction == SPI_DIRECTION_2LINES_RXONLY))) {
			/* Disable SPI peripheral */
			__HAL_SPI_DISABLE(hspi);
		}

		/* Check if Errors has been detected during transfer */
		if (hspi->ErrorCode == HAL_SPI_ERROR_NONE) {
			/* Check if we are in Rx or in Rx/Tx Mode */
			if (hspi->State == HAL_SPI_STATE_BUSY_TX_RX) {
				/* Set state to READY before run the Callback Complete */
				hspi->State = HAL_SPI_STATE_READY;
				HAL_SPI_TxRxCpltCallback(hspi);
			} else {
				/* Set state to READY before run the Callback Complete */
				hspi->State = HAL_SPI_STATE_READY;
				HAL_SPI_RxCpltCallback(hspi);
			}
		} else {
			/* Set state to READY before run the Callback Complete */
			hspi->State = HAL_SPI_STATE_READY;
			/* Call Error call back in case of Error */
			HAL_SPI_ErrorCallback(hspi);
		}
	}
}

/**
* @brief  Interrupt Handler to close Tx transfer
* @param  None
* @retval void
*/
void SPISlave::SPI_TxCloseIRQHandler()
{
	SPI_HandleTypeDef *hspi = &this->handle_;

	/* Wait until TXE flag is set to send data */
	if (SPI_WaitOnFlagUntilTimeout(SPI_FLAG_TXE, RESET, SPI_TIMEOUT_VALUE) != HAL_OK) {
		hspi->ErrorCode = (HAL_SPI_ErrorTypeDef)((unsigned int)hspi->ErrorCode | (unsigned int)HAL_SPI_ERROR_FLAG);
	}

	/* Disable TXE interrupt */
	__HAL_SPI_DISABLE_IT(hspi, (SPI_IT_TXE ));

	/* Disable ERR interrupt if Receive process is finished */
	if (__HAL_SPI_GET_IT_SOURCE(hspi, SPI_IT_RXNE) == RESET) {
		__HAL_SPI_DISABLE_IT(hspi, (SPI_IT_ERR));

		/* Wait until Busy flag is reset before disabling SPI */
		if (SPI_WaitOnFlagUntilTimeout(SPI_FLAG_BSY, SET, SPI_TIMEOUT_VALUE) != HAL_OK) {
			hspi->ErrorCode = (HAL_SPI_ErrorTypeDef)((unsigned int)hspi->ErrorCode | (unsigned int)HAL_SPI_ERROR_FLAG);
		}

		/* Clear OVERUN flag in 2 Lines communication mode because received is not read */
		if (hspi->Init.Direction == SPI_DIRECTION_2LINES) {
			__HAL_SPI_CLEAR_OVRFLAG(hspi);
		}

		/* Check if Errors has been detected during transfer */
		if (hspi->ErrorCode == HAL_SPI_ERROR_NONE) {
			/* Check if we are in Tx or in Rx/Tx Mode */
			if (hspi->State == HAL_SPI_STATE_BUSY_TX_RX) {
				/* Set state to READY before run the Callback Complete */
				hspi->State = HAL_SPI_STATE_READY;
				HAL_SPI_TxRxCpltCallback(hspi);
			} else {
				/* Set state to READY before run the Callback Complete */
				hspi->State = HAL_SPI_STATE_READY;
				HAL_SPI_TxCpltCallback(hspi);
			}
		} else {
			/* Set state to READY before run the Callback Complete */
			hspi->State = HAL_SPI_STATE_READY;
			/* Call Error call back in case of Error */
			RxTxError();
		}
	}
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


/**
  * @brief  Transmit and Receive an amount of data in no-blocking mode with Interrupt
  * @param  pTxData: pointer to transmission data buffer
  * @param  pRxData: pointer to reception data buffer to be
  * @param  Size: amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef SPISlave::TransmitReceive_IT(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
	SPI_HandleTypeDef *hspi = &this->handle_;
	uint32_t tmpstate = 0;

	tmpstate = hspi->State;
	if ((tmpstate == HAL_SPI_STATE_READY)
		|| ((hspi->Init.Mode == SPI_MODE_MASTER) && (hspi->Init.Direction == SPI_DIRECTION_2LINES) && (tmpstate == HAL_SPI_STATE_BUSY_RX))) {
		if ((pTxData == NULL ) || (pRxData == NULL ) || (Size == 0)) {
			return HAL_ERROR;
		}

		/* Check the parameters */
		assert_param(IS_SPI_DIRECTION_2LINES(hspi->Init.Direction));

		/* Process locked */
		__HAL_LOCK(hspi);

		/* Don't overwrite in case of HAL_SPI_STATE_BUSY_RX */
		if (hspi->State != HAL_SPI_STATE_BUSY_RX) {
			hspi->State = HAL_SPI_STATE_BUSY_TX_RX;
		}

		/* Configure communication */
		hspi->ErrorCode = HAL_SPI_ERROR_NONE;

		hspi->TxISR = NULL;
		hspi->pTxBuffPtr = pTxData;
		hspi->TxXferSize = Size;
		hspi->TxXferCount = Size;

		hspi->RxISR = NULL;
		hspi->pRxBuffPtr = pRxData;
		hspi->RxXferSize = Size;
		hspi->RxXferCount = Size;

		/* Reset CRC Calculation */
		if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLED) {
			__HAL_SPI_RESET_CRC(hspi);
		}

		SPI_TxISR();

		/* Enable TXE, RXNE and ERR interrupt */
		__HAL_SPI_ENABLE_IT(hspi, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));

		/* Process Unlocked */
		__HAL_UNLOCK(hspi);

		/* Check if the SPI is already enabled */
		if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE) {
			/* Enable SPI peripheral */
			__HAL_SPI_ENABLE(hspi);
		}

		return HAL_OK;
	} else {
		return HAL_BUSY;
	}
}

/**
  * @brief  This function handles SPI interrupt request.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *                the configuration information for SPI module.
  * @retval HAL status
  */
void SPISlave::IRQHandler(SPI_HandleTypeDef *hspi)
{
	SPISlave *slave = (SPISlave*)hspi;
	uint32_t tmp1 = 0, tmp2 = 0, tmp3 = 0;

	tmp1 = __HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE);
	tmp2 = __HAL_SPI_GET_IT_SOURCE(hspi, SPI_IT_RXNE);
	tmp3 = __HAL_SPI_GET_FLAG(hspi, SPI_FLAG_OVR);
	/* SPI in mode Receiver and Overrun not occurred ---------------------------*/
	if ((tmp1 != RESET) && (tmp2 != RESET) && (tmp3 == RESET)) {
		slave->SPI_RxISR();
		return;
	}

	tmp1 = __HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE);
	tmp2 = __HAL_SPI_GET_IT_SOURCE(hspi, SPI_IT_TXE);
	/* SPI in mode Tramitter ---------------------------------------------------*/
	if ((tmp1 != RESET) && (tmp2 != RESET)) {
		slave->SPI_TxISR();
		return;
	}

	if (__HAL_SPI_GET_IT_SOURCE(hspi, SPI_IT_ERR) != RESET) {
		/* SPI CRC error interrupt occurred ---------------------------------------*/
		if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_CRCERR) != RESET) {
			hspi->ErrorCode = (HAL_SPI_ErrorTypeDef)((unsigned int)hspi->ErrorCode | (unsigned int)HAL_SPI_ERROR_CRC);
			__HAL_SPI_CLEAR_CRCERRFLAG(hspi);
		}

		/* SPI Mode Fault error interrupt occurred --------------------------------*/
		if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_MODF) != RESET) {
			hspi->ErrorCode = (HAL_SPI_ErrorTypeDef)((unsigned int)hspi->ErrorCode | (unsigned int)HAL_SPI_ERROR_MODF);
			__HAL_SPI_CLEAR_MODFFLAG(hspi);
		}

		/* SPI Overrun error interrupt occurred -----------------------------------*/
		if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_OVR) != RESET) {
			if (hspi->State != HAL_SPI_STATE_BUSY_TX) {
				hspi->ErrorCode = (HAL_SPI_ErrorTypeDef)((unsigned int)hspi->ErrorCode | (unsigned int)HAL_SPI_ERROR_OVR);
				__HAL_SPI_CLEAR_OVRFLAG(hspi);
			}
		}

		/* SPI Frame error interrupt occurred -------------------------------------*/
		if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_FRE) != RESET) {
			hspi->ErrorCode = (HAL_SPI_ErrorTypeDef)((unsigned int)hspi->ErrorCode | (unsigned int)HAL_SPI_ERROR_FRE);
			__HAL_SPI_CLEAR_FREFLAG(hspi);
		}

		/* Call the Error call Back in case of Errors */
		if (hspi->ErrorCode != HAL_SPI_ERROR_NONE) {
			hspi->State = HAL_SPI_STATE_READY;
			HAL_SPI_ErrorCallback(hspi);
		}
	}
}


