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

#ifndef ALTITUDECONTROL_H_
#define ALTITUDECONTROL_H_

#include "dronestate.h"
#include "pidcontroller.h"

class AltitudeControl {
public:
	AltitudeControl();
	~AltitudeControl();
	void Update(DroneState& drone_state);
	Throttle GetThrottle();

private:
	PidController<float> pid_;
	Throttle throttle_;
	FlightMode flight_mode_;
	Distance max_error_;
};

#endif /* ALTITUDECONTROL_H_ */
