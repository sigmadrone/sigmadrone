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
	 * Get the current pulse from the hardware
	 */
	virtual unsigned int getpulse(unsigned int channel);

	/**
	 * Get the current pulse from the hardware
	 */
	virtual void setpulse(unsigned int channel, unsigned int pulse);

	/**
	 * Get the current pulse length from the hardware in mSeconds
	 */
	virtual double getpulse_ms(unsigned int channel);

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
	int ioctl(unsigned long int __request, unsigned long int arg);

protected:
	int fd_;
	std::string devicename_;
};

#endif
