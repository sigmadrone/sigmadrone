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

#ifndef DIGITALOUT_H_
#define DIGITALOUT_H_

#include "pinnames.h"
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
	DigitalOut(const DigitalOut& rhs) = default;
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
