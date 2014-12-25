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
	unsigned long portnum = (((unsigned long)gpio_port_) - GPIOA_BASE) / 0x0400;
	switch (portnum) {
		case 0:	__GPIOA_CLK_ENABLE(); break;
		case 1:	__GPIOB_CLK_ENABLE(); break;
		case 2:	__GPIOC_CLK_ENABLE(); break;
		case 3:	__GPIOD_CLK_ENABLE(); break;
#ifdef __GPIOE_CLK_ENABLE
		case 4:	__GPIOE_CLK_ENABLE(); break;
#endif
#ifdef __GPIOF_CLK_ENABLE
		case 5:	__GPIOF_CLK_ENABLE(); break;
#endif
#ifdef __GPIOG_CLK_ENABLE
		case 6:	__GPIOG_CLK_ENABLE(); break;
#endif
#ifdef __GPIOH_CLK_ENABLE
		case 7:	__GPIOH_CLK_ENABLE(); break;
#endif
#ifdef __GPIOJ_CLK_ENABLE
		case 8:	__GPIOJ_CLK_ENABLE(); break;
#endif
#ifdef __GPIOK_CLK_ENABLE
		case 9:	__GPIOK_CLK_ENABLE(); break;
#endif
	}
	HAL_GPIO_Init(gpio_port_, (GPIO_InitTypeDef*)this);
}
