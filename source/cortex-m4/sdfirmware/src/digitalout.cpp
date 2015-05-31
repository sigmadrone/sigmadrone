/*
 * digitalout.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: mstoilov
 */

#include "digitalout.h"


DigitalOut::DigitalOut(PinName pin, OutputMode omode, PullMode pmode, int value)
	: pin_(STM_PIN(pin))
	, GPIOx_((GPIO_TypeDef *) (GPIOA_BASE + 0x0400 * STM_PORT(pin)))
{
	GPIO_InitTypeDef GPIO_InitStructure;

	clock_enable(STM_PORT(pin));
	GPIO_InitStructure.Alternate = 0;
	GPIO_InitStructure.Pin = ((uint32_t)0x01) << pin_;
	if (omode == OutputPP)
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	else
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	if (pmode == PullUp)
		GPIO_InitStructure.Pull = GPIO_PULLUP;
	else if (pmode == PullDown)
		GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	else
		GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx_, &GPIO_InitStructure);
	write(value);
}

DigitalOut::~DigitalOut()
{
	HAL_GPIO_DeInit(GPIOx_, 1 << pin_);
}

void DigitalOut::clock_enable(unsigned int port)
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
		case 8:	__GPIOJ_CLK_ENABLE(); break;
		case 9:	__GPIOK_CLK_ENABLE(); break;
	}
}

void DigitalOut::write(int value)
{
	HAL_GPIO_WritePin(GPIOx_, 1 << pin_, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

int DigitalOut::read()
{
	return (int)HAL_GPIO_ReadPin(GPIOx_, 1 << pin_);
}

void DigitalOut::toggle()
{
	HAL_GPIO_TogglePin(GPIOx_, 1 << pin_);
}
