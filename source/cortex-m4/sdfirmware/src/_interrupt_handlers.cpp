/*
 *  Sigmadrone
 *  Copyright (c) 2013-2015 The Sigmadrone Developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Martin Stoilov <martin@sigmadrone.org>
 *  Svetoslav Vassilev <svassilev@sigmadrone.org>
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
#include "colibrihwmap.h"

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
	DigitalIn::vector_handler(0);
}

extern "C" void EXTI2_IRQHandler(void)
{
	DigitalIn::vector_handler(2);
}

extern "C" void EXTI3_IRQHandler(void)
{
	DigitalIn::vector_handler(3);
}

extern "C" void EXTI4_IRQHandler(void)
{
	SPISlave::spi_chipselect_handler(4);
}

extern "C" void EXTI15_10_IRQHandler(void)
{
	DigitalIn::vector_handler(12);
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

extern "C" void EmergencyShutdown_Handler(void)
{
	DigitalOut(SHUTDOWN_PIN).write(0);
}

