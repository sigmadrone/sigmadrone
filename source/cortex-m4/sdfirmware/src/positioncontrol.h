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

#ifndef POSITIONCONTROL_H_
#define POSITIONCONTROL_H_

#include "dronestate.h"
#include "pidcontroller.h"
#include "geolocation.h"
#include "lowpassfilter.h"

class PositionControl
{
public:
	PositionControl();
	void update_state(DroneState& state);
	const QuaternionF& target_swing() const;
	bool is_tracking();
private:
	bool should_start_tracking(const DroneState& state);
	bool should_stop_tracking(const DroneState& state);
	void start_tracking(const DroneState& state);
	void stop_tracking();
	void record_base_rc_values_if_landed(const DroneState& state);
	void calculate_target_swing(DroneState& state);
	bool is_landed(const DroneState& state);
	Distance get_error_as_distance(
			const Latitude& latitude,
			const Longitude& longitude);
	Vector3f get_error_vector(
			const QuaternionF& attitude,
			const Latitude& latitude,
			const Longitude& longitude);
	Angle map_distance_err_to_swing_angle(const Distance& err_dist);

	static bool has_rc_value_changed(float oldVal, float newVal);
	static Angle clip_swing_angle(const Angle&);
private:
	typedef PidController<Angle> PidControllerType;

	static const QuaternionF qt_ortho_xy_;
	static const Angle max_swing_angle_;
	static const Angle min_swing_angle_;

	QuaternionF target_swing_;
	PidControllerType pid_;
	GeoLocation target_location_;
	TimeStamp rc_record_ts_;
	LowPassFilter<Distance,float> distance_lpf_;
	LowPassFilter<Vector3f,float> axis_lpf_;
	float rc_value_pitch_;
	float rc_value_roll_;
};




#endif /* POSITIONCONTROL_H_ */
