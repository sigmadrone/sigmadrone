#include <stdexcept>
#include "servocontroller.h"


servocontroller::servocontroller(unsigned int channels)
	: motor_(new servomotor[channels])
	, channels_(channels)
	, rate_(0)
{

}

servocontroller::~servocontroller()
{
	delete [] motor_;
}

servomotor& servocontroller::operator[](unsigned int channel)
{
	if (channel >= channels_)
		throw std::range_error("servocontroller::operator[], channel is outside of the available channels range");
	return motor_[channel];
}

void servocontroller::setrate(unsigned int)
{
}

int servocontroller::getrate()
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
