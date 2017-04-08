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

#ifndef LSM303READER_H_
#define LSM303READER_H_

#include "d3math.h"
#include "units.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "lsm303d.h"
#include "digitalin.h"
#include "sensorsprefilters.h"
#include "magcalibrator.h"

class LSM303Reader
{
public:
	LSM303Reader(LSM303D& acc, PinName int2_pin, const Matrix3f& axes_align);
	~LSM303Reader() = default;
	Vector3f read_sample_acc();
	Vector3f read_sample_mag();
	size_t size();
	void init(uint8_t watermark);
	void enable_int2(bool enable);

	MagCalibrator mag_calibrator_;
private:
	void isr();

private:
	LSM303D& acc_;
	DigitalIn int2_pin_;
	Matrix3f axes_align_;
	MagPreFilter mag_filter_;
};

#endif
