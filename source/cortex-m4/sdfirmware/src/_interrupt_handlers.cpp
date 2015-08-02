/*
 * _interrupt_handlers.cpp
 *
 *  Created on: Aug 1, 2015
 *      Author: svassilev
 */

#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "digitalin.h"
#include "digitalout.h"
#include "stm32f4xx_hal.h"
#include "spimaster.h"
#include "spislave.h"
#include "uart.h"

extern unsigned int __relocated_vectors;

void relocate_interrupt_table()
{
	volatile unsigned int* newtable = &__relocated_vectors;
	volatile unsigned int* oldtable = 0;

	for (size_t i = 0; i < 256; i++) {
		newtable[i] = oldtable[i];
	}

	newtable[SVCall_IRQn + 16] = (unsigned int)vPortSVCHandler;
	newtable[PendSV_IRQn + 16] = (unsigned int)xPortPendSVHandler;
	newtable[SysTick_IRQn + 16] = (unsigned int)xPortSysTickHandler;

	__DMB();
	SCB->VTOR = ((unsigned int)newtable);
	__DSB();
}


extern "C" void EXTI0_IRQHandler(void)
{
	uint32_t mask = portDISABLE_INTERRUPTS();
	DigitalIn::vector_handler(0);
	portCLEAR_INTERRUPT_MASK_FROM_ISR(mask);
}

extern "C" void EXTI2_IRQHandler(void)
{
	uint32_t mask = portDISABLE_INTERRUPTS();
	DigitalIn::vector_handler(2);
	portCLEAR_INTERRUPT_MASK_FROM_ISR(mask);
}

extern "C" void EXTI4_IRQHandler(void)
{
	SPISlave::spi_chipselect_handler(4);
}

extern "C" void DMA2_Stream0_IRQHandler(void)
{
	SPISlave::spi_dmarx_handler(4);
}

extern "C" void DMA2_Stream1_IRQHandler(void)
{
	SPISlave::spi_dmatx_handler(4);
}

extern "C" void DMA2_Stream5_IRQHandler(void)
{
	UART::uart_dmarx_handler(1);
}

extern "C" void DMA2_Stream7_IRQHandler(void)
{
	UART::uart_dmatx_handler(1);
}

extern "C" void USART1_IRQHandler(void)
{
	UART::uart_irq_handler(1);
}

extern "C" void DMA1_Stream1_IRQHandler(void)
{
	UART::uart_dmarx_handler(3);
}

extern "C" void DMA1_Stream3_IRQHandler(void)
{
	UART::uart_dmatx_handler(3);
}

extern "C" void USART3_IRQHandler(void)
{
	UART::uart_irq_handler(3);
}

extern "C" void DMA1_Stream5_IRQHandler(void)
{
	UART::uart_dmarx_handler(2);
}

extern "C" void DMA1_Stream6_IRQHandler(void)
{
	UART::uart_dmatx_handler(2);
}

extern "C" void USART2_IRQHandler(void)
{
	UART::uart_irq_handler(2);
}

