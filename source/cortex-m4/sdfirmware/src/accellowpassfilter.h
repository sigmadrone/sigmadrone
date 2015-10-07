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

#ifndef ACCELLOWPASSFILTER_H_
#define ACCELLOWPASSFILTER_H_

#include "firfilt.h"

struct AccelLowPassFilter {
	AccelLowPassFilter() : filter_() {
		static const float oneHzCutoff_18[] = {
				0.0086, 0.0120, 0.0216, 0.0362, 0.0537, 0.0719, 0.0883, 0.1006, 0.1072, 0.1072,
				0.1006, 0.0883, 0.0719, 0.0537, 0.0362, 0.0216, 0.0120, 0.0086
		};
		filter_.init_coeff(oneHzCutoff_18);
	}
	~AccelLowPassFilter() {}
	Vector3f do_filter(const Vector3f accel) {
		return filter_.do_filter(accel);
	}
private:
	FirFilter<float, 18, 3> filter_;
};

#endif /* ACCELLOWPASSFILTER_H_ */
