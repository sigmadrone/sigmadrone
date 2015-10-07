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
	size_t validsamples = 0;
	double3d_t tmp;
	if (!isopen())
		return;
	bias_ = double3d_t();
	for (size_t i = 0; i < 32; i++)
		imu_sensor::read(&tmp, 1);
	bias_.x = bias_.y = bias_.z = 0;
	for (size_t i = 0; i < samples; i++) {
		if (imu_sensor::read(&tmp, 1)) {
			bias_.x += tmp.x;
			bias_.y += tmp.y;
			bias_.z += tmp.z;
			validsamples++;
		}
	}
	if (validsamples) {
		bias_.x /= validsamples;
		bias_.y /= validsamples;
		bias_.z /= validsamples;
	}
}
