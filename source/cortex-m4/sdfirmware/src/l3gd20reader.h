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

#ifndef L3GD20READER_H_
#define L3GD20READER_H_

#include "d3math.h"
#include "units.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "l3gd20.h"

class L3GD20Reader
{
public:
	L3GD20Reader(L3GD20& gyro, const Matrix3f& axes_align);
	~L3GD20Reader() = default;
	const Vector3f& calculate_static_bias(QueueHandle_t hGyroQueue, uint32_t num_samples);
	const Vector3f& bias() const;
	Vector3f read_data(uint32_t watermark);
	static bool wait_for_data(QueueHandle_t hGyroQueue, const TimeSpan time_to_wait = TimeSpan::from_milliseconds(50));
private:
	L3GD20& gyro_;
	Matrix3f axes_align_;
	Vector3f static_bias_;
};

#endif /* L3GD20READER_H_ */
