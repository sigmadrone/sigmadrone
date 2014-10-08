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
