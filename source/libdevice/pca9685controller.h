#ifndef PCA9685CONTROLLER_H_
#define PCA9685CONTROLLER_H_

#include <string>
#include "servocontroller.h"

class pca9685controller : public servocontroller
{
public:
	pca9685controller(unsigned int channels, const std::string& devicename = "/dev/pwm0");
	virtual ~pca9685controller();

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
	void reset();

protected:
	int fd_;
	std::string devicename_;
};

#endif
