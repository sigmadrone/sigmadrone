/*
 * gpiopin.h
 *
 *  Created on: Nov 30, 2014
 *      Author: mstoilov
 */

#ifndef GPIOINIT_H_
#define GPIOINIT_H_

#include "stm32f4xx_hal.h"
#include "pinnames.h"

class GPIOPin : public GPIO_InitTypeDef {
public:
	GPIOPin(GPIO_TypeDef* gpio_port,
			uint16_t pin,				// pin number (0..15)
			uint32_t mode,				// GPIO_mode_define
			uint32_t pull,				// GPIO_pull_define
			uint32_t speed,				// GPIO_speed_define
			uint32_t af					// Peripheral to be connected to the selected pin (Alternative function)
			);
	GPIOPin(PinName pn,
			uint32_t mode,				// GPIO_mode_define
			uint32_t pull,				// GPIO_pull_define
			uint32_t speed,				// GPIO_speed_define
			uint32_t af				// Peripheral to be connected to the selected pin (Alternative function)
			);
	~GPIOPin();

	void init();
	void cleanup();
public:
	GPIO_TypeDef* gpio_port_;
	uint16_t pinnum_;
	PinName pn_;
};

#endif /* GPIOINIT_H_ */
