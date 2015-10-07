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
