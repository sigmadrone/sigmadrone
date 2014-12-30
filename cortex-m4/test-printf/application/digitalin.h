/*
 * digitalin.h
 *
 *  Created on: Dec 10, 2014
 *      Author: mstoilov
 */

#ifndef DIGITALIN_H_
#define DIGITALIN_H_

#include "../application/functionptr.h"
#include "../application/pinnames.h"
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

	/** Attach a member function to call when a rising edge occurs on the input
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
		if (fptr)
			callback_.attach(fptr);
	}

	static void vector_handler(uint8_t line);

protected:
	void clock_enable(unsigned int port);

protected:
	uint8_t pin_;
	GPIO_TypeDef *GPIOx_;
	FunctionPointer callback_;
};


#endif /* DIGITALIN_H_ */
