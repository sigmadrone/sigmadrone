#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
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
	if (::ioctl(fd_, PCA9685_IOC_SETRATE, rate) < 0)
		throw std::runtime_error("ioctl failed for device: " + devicename_);
}

unsigned int pca9685controller::getrate()
{
	int ret = ::ioctl(fd_, PCA9685_IOC_GETRATE, 0);
	if (ret < 0)
		throw std::runtime_error("ioctl failed for device: " + devicename_);
	return ret;
}

void pca9685controller::update()
{
	unsigned int rate = getrate();
	for (size_t i = 0; i < channels_; i++) {
		if (motors_[i].valid()) {
			if (::ioctl(fd_, (PCA9685_IOC_SETLED0 + i), PCA9685_PULSE(0, PCA9685_SIZE * motors_[i].pulse() * rate / 1000.0)) < 0)
				throw std::runtime_error("ioctl failed for device: " + devicename_);
		}
	}
}

void pca9685controller::enable()
{
	if (::ioctl(fd_, PCA9685_IOC_SETPWMOE, 1) < 0 || ::ioctl(fd_, PCA9685_IOC_SETMOTOROE, 1) < 0)
		throw std::runtime_error("ioctl failed for device: " + devicename_);

}

void pca9685controller::disable()
{
	if (::ioctl(fd_, PCA9685_IOC_SETMOTOROE, 0) < 0 || ::ioctl(fd_, PCA9685_IOC_SETPWMOE, 0) < 0)
		throw std::runtime_error("ioctl failed for device: " + devicename_);
}

void pca9685controller::reset()
{
	for (size_t i = 0; i < channels_; i++)
		motors_[i].reset();
}
