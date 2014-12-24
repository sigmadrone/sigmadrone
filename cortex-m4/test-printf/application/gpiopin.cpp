/*
 * gpiopin.cpp
 *
 *  Created on: Nov 30, 2014
 *      Author: mstoilov
 */

#include "gpiopin.h"

GPIOPin::GPIOPin(
		GPIO_TypeDef* gpio_port,
		uint16_t pinnum,			// pin number (0..15)
		uint32_t mode,				// GPIO_mode_define
		uint32_t pull,				// GPIO_pull_define
		uint32_t speed,				// GPIO_speed_define
		uint32_t af					// Peripheral to be connected to the selected pin (Alternative function)
		)
{
	Pin = 1 << pinnum;
	Mode = mode;
	Pull = pull;
	Speed = speed;
	Alternate = af;
	gpio_port_ = gpio_port;
	pinnum_ = pinnum;
}

GPIOPin::GPIOPin(
		PinName pn,
		uint32_t mode,				// GPIO_mode_define
		uint32_t pull,				// GPIO_pull_define
		uint32_t speed,				// GPIO_speed_define
		uint32_t af					// Peripheral to be connected to the selected pin (Alternative function)
		)
		: gpio_port_((GPIO_TypeDef *) (GPIOA_BASE + 0x0400 * STM_PORT(pn)))
		, pinnum_(STM_PIN(pn))
{
	Pin = 1 << pinnum_;
	Mode = mode;
	Pull = pull;
	Speed = speed;
	Alternate = af;
}

GPIOPin::~GPIOPin()
{
}

void GPIOPin::init()
{
	if (gpio_port_ == GPIOA)
		__GPIOA_CLK_ENABLE();
	else if (gpio_port_ == GPIOB)
		__GPIOB_CLK_ENABLE();
	else if (gpio_port_ == GPIOC)
		__GPIOC_CLK_ENABLE();
	else if (gpio_port_ == GPIOD)
		__GPIOD_CLK_ENABLE();
	else if (gpio_port_ == GPIOE)
		__GPIOE_CLK_ENABLE();
	else if (gpio_port_ == GPIOF)
		__GPIOF_CLK_ENABLE();
	else if (gpio_port_ == GPIOG)
		__GPIOG_CLK_ENABLE();
	else if (gpio_port_ == GPIOH)
		__GPIOH_CLK_ENABLE();
	else if (gpio_port_ == GPIOI)
		__GPIOI_CLK_ENABLE();
	else if (gpio_port_ == GPIOJ)
		__GPIOJ_CLK_ENABLE();
	else if (gpio_port_ == GPIOK)
		__GPIOK_CLK_ENABLE();
	HAL_GPIO_Init(gpio_port_, (GPIO_InitTypeDef*)this);
}
