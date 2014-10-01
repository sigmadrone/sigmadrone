#include <stdexcept>
#include "pca9685controller.h"


pca9685controller::pca9685controller(unsigned int channels, const std::string& devicename)
	: servocontroller(channels)
{

}

pca9685controller::~pca9685controller()
{
}

void pca9685controller::setrate(unsigned int)
{
}

int pca9685controller::getrate()
{
	return rate_;
}

void pca9685controller::update()
{

}

void pca9685controller::enable()
{

}

void pca9685controller::disable()
{

}
