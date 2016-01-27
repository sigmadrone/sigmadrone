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

#ifndef SENSORS_PREFILTERS_H_
#define SENSORS_PREFILTERS_H_

#include "firfilt.h"
#include "iirfilt.h"

class MagLowPassPreFilter3d : public FirFilter<Vector3f, float, 18> {
	using Base = FirFilter<Vector3f, float, 18>;
public:
	MagLowPassPreFilter3d() : Base(
			{0.0086, 0.0120, 0.0216, 0.0362, 0.0537, 0.0719, 0.0883, 0.1006, 0.1072, 0.1072,
		0.1006, 0.0883, 0.0719, 0.0537, 0.0362, 0.0216, 0.0120, 0.0086}) {}
};

typedef IirLowPassFilter5Hz<Vector3d, double> AccelLowPassPreFilter3d;

static const size_t PRESSURE_FILTER_ORDER = 25;
typedef MovingAverageFilter<float, float, PRESSURE_FILTER_ORDER> PressurePreFilter;
typedef MovingAverageFilter<float, float, PRESSURE_FILTER_ORDER> TemperaturePreFilter;

#endif /* SENSORS_PREFILTERS_H_ */
