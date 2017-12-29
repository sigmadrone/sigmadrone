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

#ifndef DIGITALIN_H_
#define DIGITALIN_H_

#include "functionptr.h"
#include "pinnames.h"
#include "stm32f4xx_hal.h"

class DigitalIn {

public:
	enum InterruptMode {
		InterruptNone = 0,
		InterruptRising,
		InterruptFalling,
		InterruptRisingFalling,
		InterruptDefault = InterruptNone
	};

	enum PullMode {
	    PullNone  = 0,
	    PullUp    = 1,
	    PullDown  = 2,
	    PullDefault = PullNone
	};

	/** Create a DigitalIn connected to the specified pin
	 *
	 *  @param pin DigitalIn pin to connect to
	 *  @param mode the initial mode of the pin
	 */
	DigitalIn(PinName pin, PullMode pmode = PullDefault, InterruptMode imode = InterruptDefault);

	/** Destroy a DigitalIn object
	 *
	 */
	virtual ~DigitalIn();

	/** Read the input, represented as 0 or 1 (int)
	 *
	 *  @returns
	 *    An integer representing the state of the input pin,
	 *    0 for logical 0, 1 for logical 1
	 */
	int read();

	/** An operator shorthand for read()
	 */
	operator int()
	{
		return read();
	}

	/** Attach a member function to call when a rising or falling edge occurs on the input
	 *
	 *  @param tptr pointer to the object to call the member function on
	 *  @param mptr pointer to the member function to be called
	 */
	template<typename T>
	void callback(T* tptr, void (T::*mptr)(void))
	{
		if (tptr && mptr)
			callback_.attach(tptr, mptr);
	}

	void callback(void (*fptr)(void))
	{
		callback_.attach(fptr);
	}

	bool poll_edge(InterruptMode edge);

	static void vector_handler(uint8_t line);

protected:
	void clock_enable(unsigned int port);

protected:
	uint8_t pin_;
	GPIO_TypeDef *GPIOx_;
	int state_;
	FunctionPointer callback_;
};


#endif /* DIGITALIN_H_ */
