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


//#include "stm32f4xx_hal_cortex.h"
#include <assert.h>
#include "digitalin.h"

static DigitalIn* g_interrupt[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

void DigitalIn::vector_handler(uint8_t line)
{
	uint16_t GPIO_Pin = 1 << line;
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != RESET) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
		if (g_interrupt[line])
			g_interrupt[line]->callback_.call();
	}
}

DigitalIn::DigitalIn(PinName pin, PullMode pmode, InterruptMode imode)
	: pin_(STM_PIN(pin))
	, GPIOx_((GPIO_TypeDef *) (GPIOA_BASE + 0x0400 * STM_PORT(pin)))
	, state_(-1)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	clock_enable(STM_PORT(pin));
	GPIO_InitStructure.Pin = ((uint32_t)0x01) << pin_;
	if (imode == InterruptRising)
		GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	else if (imode == InterruptFalling)
		GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
	else if (imode == InterruptRisingFalling)
		GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
	else
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	if (pmode == PullUp)
		GPIO_InitStructure.Pull = GPIO_PULLUP;
	else if (pmode == PullDown)
		GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	else
		GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx_, &GPIO_InitStructure);

	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	if (imode != InterruptNone && pin_ < 16) {
		assert(g_interrupt[pin_] == NULL);
		g_interrupt[pin_] = this;
		switch (pin_) {
		case 0:
			HAL_NVIC_SetPriority(EXTI0_IRQn, 7, 0);
			HAL_NVIC_EnableIRQ(EXTI0_IRQn);
			break;
		case 1:
			HAL_NVIC_SetPriority(EXTI1_IRQn, 7, 0);
			HAL_NVIC_EnableIRQ(EXTI1_IRQn);
			break;
		case 2:
			HAL_NVIC_SetPriority(EXTI2_IRQn, 7, 0);
			HAL_NVIC_EnableIRQ(EXTI2_IRQn);
			break;
		case 3:
			HAL_NVIC_SetPriority(EXTI3_IRQn, 7, 0);
			HAL_NVIC_EnableIRQ(EXTI3_IRQn);
			break;
		case 4:
			HAL_NVIC_SetPriority(EXTI4_IRQn, 7, 0);
			HAL_NVIC_EnableIRQ(EXTI4_IRQn);
			break;
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			HAL_NVIC_SetPriority(EXTI9_5_IRQn, 7, 0);
			HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
			break;
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			HAL_NVIC_SetPriority(EXTI15_10_IRQn, 7, 0);
			HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
			break;
		}
	}

}

DigitalIn::~DigitalIn()
{
	if (pin_ < 16 && g_interrupt[pin_] == this)
		g_interrupt[pin_] = 0;
	HAL_GPIO_DeInit(GPIOx_, 1 << pin_);
}

void DigitalIn::clock_enable(unsigned int port)
{
	switch (port) {
		case 0:	__GPIOA_CLK_ENABLE(); break;
		case 1:	__GPIOB_CLK_ENABLE(); break;
		case 2:	__GPIOC_CLK_ENABLE(); break;
		case 3:	__GPIOD_CLK_ENABLE(); break;
		case 4:	__GPIOE_CLK_ENABLE(); break;
		case 5:	__GPIOF_CLK_ENABLE(); break;
		case 6:	__GPIOG_CLK_ENABLE(); break;
		case 7:	__GPIOH_CLK_ENABLE(); break;
		case 8:	__GPIOI_CLK_ENABLE(); break;
		case 9:	__GPIOJ_CLK_ENABLE(); break;
		case 10:__GPIOK_CLK_ENABLE(); break;
	}
}

int DigitalIn::read()
{
	return (int)HAL_GPIO_ReadPin(GPIOx_, 1 << pin_);
}

bool DigitalIn::poll_edge(InterruptMode edge)
{
	bool ret = false;
	int state = read();

	if (state_ < 0) {
		state_ = state;
		return false;
	}
	switch (edge) {
	case InterruptNone:

		break;

	case InterruptFalling:
		if (state_ == 1 && state == 0) {
			callback_.call();
			ret = true;
		}
		break;

	case InterruptRising:
		if (state_ == 0 && state == 1) {
			callback_.call();
			ret = true;
		}
		break;

	case InterruptRisingFalling:
		if ((state_ == 1 && state == 0) || (state_ == 0 && state == 1)) {
			callback_.call();
			ret = true;
		}
		break;
	default:
		break;
	}
	state_ = state;
	return ret;
}
