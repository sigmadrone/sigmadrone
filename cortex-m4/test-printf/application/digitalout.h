/*
 * digitalout.h
 *
 *  Created on: Dec 9, 2014
 *      Author: mstoilov
 */

#ifndef DIGITALOUT_H_
#define DIGITALOUT_H_

#include "../application/pinnames.h"
#include "stm32f4xx_hal.h"

class DigitalOut {

public:
	enum PullMode {
	    PullNone  = 0,
	    PullUp    = 1,
	    PullDown  = 2,
	    PullDefault = PullNone
	};

	enum OutputMode {
		OutputPP = 0,
		OutputOD = 1,
		OutputDefault = OutputPP
	};

	/** Create a DigitalOut connected to the specified pin
	 *
	 *  @param pin DigitalOut pin to connect to
	 *  @param value the initial pin value
	 */
	DigitalOut(PinName pin, OutputMode omode = OutputDefault, PullMode pmode = PullDefault, int value = 0);

	~DigitalOut();

	/** Set the output, specified as 0 or 1 (int)
	 *
	 *  @param value An integer specifying the pin output value,
	 *      0 for logical 0, 1 (or any other non-zero value) for logical 1
	 */
	void write(int value);

	/** Toggle the current output
	 */
	void toggle();

	/** Return the output setting, represented as 0 or 1 (int)
	 *
	 *  @returns
	 *    an integer representing the output setting of the pin,
	 *    0 for logical 0, 1 for logical 1
	 */
	int read();

	/** A shorthand for write()
	 */
	DigitalOut& operator=(int value)
	{
		write(value);
		return *this;
	}

	DigitalOut& operator=(DigitalOut& rhs)
	{
		write(rhs.read());
		return *this;
	}

	/** A shorthand for read()
	 */
	operator int()
	{
		return read();
	}

protected:
	void clock_enable(unsigned int port);

protected:
	uint8_t pin_;
	GPIO_TypeDef *GPIOx_;
};

#endif /* DIGITALOUT_H_ */
