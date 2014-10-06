#include <string>
#include <sstream>
#include <stdexcept>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include "axesdata.h"
#include "biasimusensor.h"

biasimu_sensor::biasimu_sensor(const std::string& filename, unsigned int trottle, double scale)
	: imu_sensor(filename, trottle, scale)
	, bias_()
{
}

biasimu_sensor::~biasimu_sensor()
{
}

size_t biasimu_sensor::read(double3d_t* buffer, size_t size) const
{
	size_t ret = imu_sensor::read(buffer, size);
	for (size_t i = 0; i < ret; i++) {
		buffer[i].x -= bias_.x;
		buffer[i].y -= bias_.y;
		buffer[i].z -= bias_.z;
	}
	return ret;
}

void biasimu_sensor::bias_update(size_t samples)
{
	double3d_t tmp;
	if (!isopen())
		return;
	bias_ = double3d_t();
	for (size_t i = 0; i < 32; i++) {
		while (!imu_sensor::read(&tmp, 1))
			;
	}
	for (size_t i = 0; i < samples; i++) {
		while (!imu_sensor::read(&tmp, 1))
			;
		bias_.x += tmp.x;
		bias_.y += tmp.y;
		bias_.z += tmp.z;
	}
	bias_.x /= samples;
	bias_.y /= samples;
	bias_.z /= samples;
}
