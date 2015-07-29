/*
 * gpiopin.cpp
 *
 *  Created on: Nov 30, 2014
 *      Author: mstoilov
 */

#include "gpiopin.h"

#ifndef GET_GPIO_SOURCE
#define GET_GPIO_SOURCE(__GPIOx__) \
(((uint32_t)(__GPIOx__) == ((uint32_t)GPIOA_BASE))? (uint32_t)0 :\
 ((uint32_t)(__GPIOx__) == ((uint32_t)(GPIOA_BASE + 0x0400)))? (uint32_t)1 :\
 ((uint32_t)(__GPIOx__) == ((uint32_t)(GPIOA_BASE + 0x0800)))? (uint32_t)2 :\
 ((uint32_t)(__GPIOx__) == ((uint32_t)(GPIOA_BASE + 0x0C00)))? (uint32_t)3 :\
 ((uint32_t)(__GPIOx__) == ((uint32_t)(GPIOA_BASE + 0x1000)))? (uint32_t)4 :\
 ((uint32_t)(__GPIOx__) == ((uint32_t)(GPIOA_BASE + 0x1400)))? (uint32_t)5 :\
 ((uint32_t)(__GPIOx__) == ((uint32_t)(GPIOA_BASE + 0x1800)))? (uint32_t)6 :\
 ((uint32_t)(__GPIOx__) == ((uint32_t)(GPIOA_BASE + 0x1C00)))? (uint32_t)7 :\
 ((uint32_t)(__GPIOx__) == ((uint32_t)(GPIOA_BASE + 0x2000)))? (uint32_t)8 :\
 ((uint32_t)(__GPIOx__) == ((uint32_t)(GPIOA_BASE + 0x2400)))? (uint32_t)9 : (uint32_t)10)
#endif

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
	pn_ = (PinName)((GET_GPIO_SOURCE(gpio_port) << 8) | ((uint32_t)pinnum & 0xFF));
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
		, pn_(pn)
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

void GPIOPin::cleanup()
{
	HAL_GPIO_DeInit(gpio_port_, pinnum_);
}
