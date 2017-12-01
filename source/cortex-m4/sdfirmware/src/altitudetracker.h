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

#ifndef ALTITUDETRACKER_H_
#define ALTITUDETRACKER_H_

#include <stdint.h>
#include "dronestate.h"
#include "derivativefilter.h"
#include "pidcontroller.h"
#include "sensorsprefilters.h"

class AltitudeTracker
{
public:
	AltitudeTracker(
			const Altitude& flight_ceiling = DEFAULT_FLIGHT_CEILING,
			float safe_threshold = 0.8f);
	~AltitudeTracker();
	void reset();
	Altitude flight_ceiling() const;
	Altitude flight_ceiling_absolute() const;
	Altitude highest_recorded_altitude() const;
	void update_state(DroneState& state);
	bool is_flight_ceiling_hit() const;

	static float calc_vertical_accel(const DroneState& state);

private:

	void clear_alarm();
	void estimate_altitude(DroneState& state);
	bool calc_vert_accel_bias(const DroneState& state);

	static const uint32_t ALARM_COUNT_THRESHOLD = 16;

	Altitude flight_ceiling_;
	Altitude starting_altitude_;
	Altitude highest_altitude_;
	Altitude estimated_altitude_;
	Altitude last_altitude_reading_;
	PidController<Speed> pid_;
	Speed estimated_velocity_;
	TimeStamp estimate_ts_;
	TimeStamp accel_dt_;
	DerivativeFilter<float, 7> altitude_deriv_;
	LowPassFilter<Speed, float> velocity_lpf_;
	uint32_t alarm_count_;
	float safe_threshold_;
	bool flight_ceiling_hit_;
	float last_baro_reading_;
	uint32_t vertical_accel_bias_samples_;
	float vertical_accel_bias_;
	float accel_kp_;
};


#endif /* ALTITUDETRACKER_H_ */
