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

#include "l3gd20reader.h"

L3GD20Reader::L3GD20Reader(L3GD20& gyro, const Matrix3f& axes_align) : gyro_(gyro), axes_align_(axes_align)
{
}

const Vector3f& L3GD20Reader::calculate_static_bias(QueueHandle_t hGyroQueue, uint32_t num_samples)
{
	static_bias_.clear();
	L3GD20::AxesDPS_t gyro_axes;
	gyro_.GetFifoAngRateDPS(&gyro_axes); // Drain the fifo
	for (uint32_t i = 0; i < num_samples; i++) {
		wait_for_data(hGyroQueue);
		static_bias_ += read_data(1);
	}
	static_bias_ /= static_cast<float>(num_samples);
	return static_bias_;
}


const Vector3f& L3GD20Reader::bias() const
{
	return static_bias_;
}

Vector3f L3GD20Reader::read_data(uint32_t watermark)
{
	Vector3f gyro_data;
	uint8_t gyro_samples = gyro_.GetFifoSourceReg() & 0x1F;
	if (gyro_samples >= watermark) {
		L3GD20::AxesDPS_t axes;
		for (size_t i = 0; i < gyro_samples; ++i) {
			gyro_.GetAngRateDPS(&axes);
			gyro_data += Vector3f(axes.AXIS_X, axes.AXIS_Y, axes.AXIS_Z);
		}
		gyro_data /= gyro_samples;
	}
	return axes_align_ * gyro_data;
}

bool L3GD20Reader::wait_for_data(QueueHandle_t hGyroQueue, const TimeSpan time_to_wait)
{
	uint32_t msg;
	return xQueueReceive(hGyroQueue, &msg, ( TickType_t ) portTICK_PERIOD_MS * time_to_wait.milliseconds());
}
