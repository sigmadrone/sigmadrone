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


#ifndef MAGCALIBRATOR_H_
#define MAGCALIBRATOR_H_

#include "d3math.h"

class MagCalibrator
{
public:
	MagCalibrator();
	void start_stop_calibration(bool start);
	void add_reading(const Vector3f& mag_values);
	const Vector3f& bias() const;
	const Vector3f& scale_factor() const;
	bool is_calibrating() const;
	Vector3f calibrate_reading(const Vector3f& mag_values);
	void set_bias(const Vector3f&);
	void set_scale_factor(const Vector3f&);

private:
	Vector3f mag_bias_;
	Vector3f scale_factor_;
	Vector3f min_values_;
	Vector3f max_values_;
	bool calibration_in_progress_;
};




#endif /* MAGCALIBRATOR_H_ */
