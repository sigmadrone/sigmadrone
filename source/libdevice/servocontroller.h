#ifndef SERVOCONTROLLER_H_
#define SERVOCONTROLLER_H_

#include "servomotor.h"

class servocontroller
{
public:
	servocontroller(unsigned int channels);
	virtual ~servocontroller();
	servomotor& operator[](unsigned int channel);

	/**
	 * Set the rate of the generated PWM pulses
	 */
	virtual void setrate(unsigned int rate);

	/**
	 * Get the rate of the generated PWM pulses
	 */
	virtual unsigned int getrate();

	/**
	 * Update the hardware PWM controller with the motor(s) pulse
	 */
	virtual void update();

	/**
	 * Enable hardware output
	 */
	virtual void enable();

	/**
	 * Disable hardware output
	 */
	virtual void disable();

protected:
	servomotor* motors_;
	size_t channels_;
	unsigned int rate_;
};

#endif
