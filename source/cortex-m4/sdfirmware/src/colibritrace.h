/*
 * colibritrace.h
 *
 *  Created on: Jun 29, 2015
 *      Author: svassilev
 */

#ifndef COLIBRITRACE_H_
#define COLIBRITRACE_H_

#include <cassert>

namespace colibri {

struct UartTrace {
	static void init(uint32_t baudrate=115200) {
		GPIOPin tx_gpio(PG_14, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF8_USART6);
		GPIOPin rx_gpio(PG_9, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF8_USART6);
		static UART uart6({tx_gpio, rx_gpio}, USART6, 0,
				DMA2_Stream7, DMA_CHANNEL_4, DMA2_Stream5, DMA_CHANNEL_4, 250,
				baudrate);
	}
};


}


#endif /* COLIBRITRACE_H_ */
