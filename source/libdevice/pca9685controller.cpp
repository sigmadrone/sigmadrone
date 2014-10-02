#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <sstream>
#include "pca9685.h"
#include "pca9685controller.h"


pca9685controller::pca9685controller(unsigned int channels, const std::string& devicename)
	: servocontroller(channels)
	, devicename_(devicename)
{
	fd_ = ::open(devicename.c_str(), O_RDONLY);
	if (fd_)
		throw std::runtime_error("pca9685controller failed to open device: " + devicename);
	reset();
	update();
}

pca9685controller::~pca9685controller()
{
	if (fd_ >= 0) {
		reset();
		update();
		disable();
		::close(fd_);
	}
}

void pca9685controller::setrate(unsigned int rate)
{
	ioctl(PCA9685_IOC_SETRATE, rate);
}

unsigned int pca9685controller::getrate()
{
	return ioctl(PCA9685_IOC_GETRATE, 0);
}

void pca9685controller::update()
{
	unsigned int rate = getrate();
	for (size_t i = 0; i < channels_; i++) {
		if (motors_[i].valid())
			setpulse(i, PCA9685_SIZE * motors_[i].pulse() * rate / 1000);
	}
}

void pca9685controller::enable()
{
	ioctl(PCA9685_IOC_SETPWMOE, 1);
	ioctl(PCA9685_IOC_SETMOTOROE, 1);
}

void pca9685controller::disable()
{
	ioctl(PCA9685_IOC_SETMOTOROE, 0);
	ioctl(PCA9685_IOC_SETPWMOE, 0);
}

void pca9685controller::reset()
{
	for (size_t i = 0; i < channels_; i++)
		motors_[i].reset();
}

unsigned int pca9685controller::getpulse(unsigned int channel)
{
	return ioctl(PCA9685_IOC_GETLED0 + channel, 0);
}

void pca9685controller::setpulse(unsigned int channel, unsigned int pulse)
{
	ioctl(PCA9685_IOC_SETLED0 + channel, PCA9685_PULSE(0, pulse));
}

double pca9685controller::getpulse_ms(unsigned int channel)
{
	unsigned int rate = getrate();
	if (!rate)
		return 0.0;
	return ((double)1000.0) * getpulse(channel) / (rate * PCA9685_SIZE);
}

int pca9685controller::ioctl(unsigned long int cmd, unsigned long int arg)
{
	int ret;
	ret = ::ioctl(fd_, cmd, arg);
	if (ret < 0) {
		std::stringstream strcmd, strarg;
		strcmd << cmd;
		strarg << arg;
		throw std::runtime_error("ioctl failed for device: " + devicename_ + ", cmd: " + strcmd.str() + ", arg: " + strarg.str());
	}
	return ret;
}
