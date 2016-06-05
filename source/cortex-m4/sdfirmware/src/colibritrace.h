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

#ifndef COLIBRITRACE_H_
#define COLIBRITRACE_H_

#include <cassert>
#include "colibrihwmap.h"

namespace colibri {

struct UartTrace {
	static void init(uint32_t baudrate=115200) {
		GPIOPin tx_gpio(TRACE_USART_TX_PIN, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF7_USART1);
		GPIOPin rx_gpio(TRACE_USART_RX_PIN, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF7_USART1);
		static UART uart1({tx_gpio, rx_gpio}, TRACE_USART, 0,
				0,0,0,0,
				UART_HWCONTROL_NONE,
				baudrate,
				250);
	}
};


}


#endif /* COLIBRITRACE_H_ */
