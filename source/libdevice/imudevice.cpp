#include <string>
#include <stdexcept>
#include <unistd.h>
#include "axesdata.h"
#include "imudevice.h"

imu_device::imu_device(const char *filename, unsigned int mode)
	: fd_(-1)
	, filename_(filename)
	, mode_(mode)
{

}

imu_device::~imu_device()
{
	close();
}

void imu_device::open()
{

}

ssize_t imu_device::read(double3d_t* buffer, size_t size)
{

	return 0;
}

bool imu_device::read_average(double& x, double& y, double &z)
{
	return 0;
}

void imu_device::close()
{
	if (fd_ >= 0)
		::close(fd_);
}


void imu_device::enable()
{

}

void imu_device::disable()
{

}

int imu_device::get_rate()
{
	return 0;
}

void imu_device::set_rate(unsigned int rate)
{

}

int imu_device::get_fifo_threshold()
{
	return 0;
}

void imu_device::set_fifo_threshold(unsigned int threshold)
{

}

int imu_device::get_full_scale()
{
	return 0;
}

void imu_device::set_full_scale(int scale)
{

}

void imu_device::reset()
{

}
