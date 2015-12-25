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

#include "altitudecontrol.h"

AltitudeControl::AltitudeControl() :
	pid_(0.0,0.0,0.0),
	throttle_(0),
	flight_mode_(FLIGHT_MODE_AUTO_LEVEL),
	max_error_(Distance::from_meters(10.0))
{
}

AltitudeControl::~AltitudeControl()
{
}

void AltitudeControl::Update(DroneState& drone_state)
{
	if (drone_state.flight_mode_ != FLIGHT_MODE_AUTO_ALTITUDE) {
		throttle_ = drone_state.base_throttle_;
		return;
	}
}

Throttle AltitudeControl::GetThrottle()
{
	return throttle_;
}
