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

#include "magcalibrator.h"
#include <stdint.h>

static const float min_float = std::numeric_limits<float>::min();
static const float max_float = std::numeric_limits<float>::max();

MagCalibrator::MagCalibrator() :
	mag_bias_(0,0,0),
	scale_factor_(1.0f,1.0f,1.0f),
	min_values_(max_float, max_float, max_float),
	max_values_(min_float, min_float, min_float),
	calibration_in_progress_(false)
{
}

void MagCalibrator::start_stop_calibration(bool start)
{
	if (start) {
		max_values_ = Vector3f(min_float, min_float, min_float);
		min_values_ = Vector3f(max_float, max_float, max_float);
	} else 	if (is_calibrating()) {
		Vector3f range = (max_values_ - min_values_) / 2.0f;
		float avg_range = range.sum() / 3.0f;
		scale_factor_ = Vector3f(avg_range / range[0], avg_range / range[1], avg_range / range[2] );
		mag_bias_ = (max_values_ + min_values_) / 2;
	}
	calibration_in_progress_ = start;
}

void MagCalibrator::add_reading(const Vector3f& mag_values)
{
	for (size_t i = 0; i < 3; ++i) {
		max_values_[i] = std::max(max_values_[i],mag_values[i]);
		min_values_[i] = std::min(min_values_[i], mag_values[i]);
	}
}

const Vector3f& MagCalibrator::bias() const
{
	return mag_bias_;
}

const Vector3f& MagCalibrator::scale_factor() const
{
	return scale_factor_;
}

bool MagCalibrator::is_calibrating() const
{
	return calibration_in_progress_;
}

Vector3f MagCalibrator::calibrate_reading(const Vector3f& mag_values)
{
	Vector3f calibrated;
	for (size_t i = 0; i < 3; ++i) {
		calibrated[i] = (mag_values[i] - mag_bias_[i]) * scale_factor_[i];
	}
	return calibrated;
}

void MagCalibrator::set_bias(const Vector3f& bias)
{
	mag_bias_ = bias;
}

void MagCalibrator::set_scale_factor(const Vector3f& scale_factor)
{
	scale_factor_ = scale_factor;
}


