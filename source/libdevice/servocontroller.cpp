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

servomotor& servocontroller::operator[](unsigned int channel)
{
	if (channel >= channels_)
		throw std::range_error("servocontroller::operator[], channel is outside of the available channels range");
	return motors_[channel];
}

void servocontroller::setrate(unsigned int)
{
}

unsigned int servocontroller::getrate()
{
	return rate_;
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
