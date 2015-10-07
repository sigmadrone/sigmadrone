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
#include <stdexcept>
#include "servocontroller.h"


servocontroller::servocontroller(unsigned int channels)
	: motors_(new servomotor[channels])
	, channels_(channels)
	, rate_(0)
{

}

servocontroller::~servocontroller()
{
	delete [] motors_;
}

servomotor& servocontroller::motor(unsigned int channel)
{
	if (channel >= channels_)
		throw std::range_error("servocontroller::operator[], channel is outside of the available channels range");
	return motors_[channel];
}

size_t servocontroller::channelcount() const
{
	return channels_;
}

void servocontroller::armmotors()
{
	for (size_t i = 0; i < channelcount(); i++) {
		motor(i).reset();
	}
	update();
}

void servocontroller::disarmmotors() {
	for (size_t i = 0; i < channelcount(); i++) {
		setpulse(i,0);
	}
	update();
}

void servocontroller::reset() {
	for (size_t i = 0; i < channelcount(); i++) {
		if (motor(i).valid())
			motor(i).reset();
	}
	update();
}

void servocontroller::setrate(unsigned int)
{
}

unsigned int servocontroller::getrate()
{
	return rate_;
}

unsigned int servocontroller::getpulse(unsigned int )
{
	return 0;
}

void servocontroller::setpulse(unsigned int , unsigned int )
{

}

double servocontroller::getpulse_ms(unsigned int )
{
	return 0.0;
}

void servocontroller::update()
{

}

void servocontroller::enable()
{

}

void servocontroller::disable()
{

}
