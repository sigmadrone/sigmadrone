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
#include "firfilt.h"
#include "derivativefilter.h"


static const Throttle min_throttle_altitude_hold = Throttle(0.49);
static const Throttle max_throttle_altitude_hold = Throttle(0.53);
static const Throttle landing_throttle = Throttle(0.2);
static const Speed max_vertical_speed = (ONE_METER * 4.0f) / ONE_SECOND;
static const Speed min_vertical_speed = -((ONE_METER * 3.0f) / ONE_SECOND);

class AltitudeControl {
public:
	AltitudeControl();
	~AltitudeControl() = default;
	void update_state(DroneState& drone_state);
	Throttle get_throttle();

private:
	void on_state_landed(DroneState& drone_state);
	void on_state_ascend(DroneState& drone_state);
	void on_state_altitude_hold(DroneState& drone_state);
	void on_state_descend(DroneState& drone_state);
	Speed get_target_vertical_speed(const DroneState& drone_state);
	Speed error_as_vertical_speed(const DroneState& drone_state);
	Altitude error_as_altitude(const DroneState& drone_state);
	void set_throttle_from_ascend_descend(const DroneState& drone_state);
	void set_throttle_from_altitude_hold(const DroneState& drone_state);
	void go_to_state_altitude_hold(const DroneState& drone_state);

	bool is_altitude_hold_throttle(const Throttle& t);
	bool is_ascend_throttle(const Throttle& t);
	bool is_descend_throttle(const Throttle& t);
	bool is_landing_throttle(const Throttle& t);

	enum State {
		STATE_LANDED,
		STATE_ALTITUDE_HOLD,
		STATE_ASCEND,
		STATE_DESCEND,
		STATE_LAST
	};
	const char* state_to_string(State);

	PidController<float> pid_vert_speed_;
	State state_;
	Throttle throttle_;
	Throttle last_base_throttle_;
	Altitude takeoff_altitude_;
	Altitude target_altitude_;
	TimeSpan dt_;
	TimeStamp last_update_time_;
	TimeStamp throttle_hold_;
};

#endif /* ALTITUDECONTROL_H_ */
