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

#include "altitudetracker.h"

AltitudeTracker::AltitudeTracker(const Altitude& ceiling, float safe_threshold) :
	flight_ceiling_(ceiling), starting_altitude_(INVALID_ALTITUDE), estimated_altitude_(INVALID_ALTITUDE),
	alarm_count_(0), safe_threshold_(safe_threshold), flight_ceiling_hit_(false), last_baro_reading_(0)
{
	assert(safe_threshold >= 0.0f && safe_threshold_ <= 1.0f);
}

AltitudeTracker::~AltitudeTracker()
{
}

void AltitudeTracker::reset()
{
	starting_altitude_ = INVALID_ALTITUDE;
	alarm_count_ = 0;
	flight_ceiling_hit_ = false;
}

Altitude AltitudeTracker::flight_ceiling() const
{
	return flight_ceiling_;
}

Altitude AltitudeTracker::flight_ceiling_absolute() const
{
	return starting_altitude_ != INVALID_ALTITUDE ? (flight_ceiling_ + starting_altitude_) :
			Altitude::from_meters(0);
}

float AltitudeTracker::calc_vertical_accel(const DroneState& state)
{
	Vector3f worldAccel = state.attitude_.rotate(state.accel_raw_+state.accelerometer_adjustment_);
	return worldAccel[2] + 1.0f;
}

inline float apply_deadband(float val, float deadband)
{
	if (fabs(val) < deadband) { return 0; }
	return (val > 0) ? val - deadband : val+deadband;
}

void AltitudeTracker::estimate_altitude(DroneState& drone_state)
{
	static float ACCEL_KP = 1.3;
	static float KI_LEAK = 0.05;

	float vert_accel = apply_deadband(calc_vertical_accel(drone_state), 0.01);

	pid_.set_kp_ki_kd(drone_state.altitude_tracker_kp_,
			drone_state.altitude_tracker_ki_, drone_state.altitude_tracker_kd_);

	// update velocity estimate from accelerometer measurement
	float alt_inc = -vert_accel * 9.8605 * drone_state.dt_.seconds_float() * ACCEL_KP;
	estimated_velocity_ += Speed::from_meters_per_second(alt_inc);

	// update altitude with the velocity estimate
	estimated_altitude_ += estimated_velocity_ * drone_state.dt_;
	if (drone_state.pressure_hpa_ != last_baro_reading_) {
		// Correct the altitude and velocity estimate from the baro measurement/observation
		TimeSpan dt = estimate_ts_.elapsed();
		Altitude alt_err = drone_state.altitude_ - estimated_altitude_;

		estimated_velocity_ += pid_.get_pid(alt_err, dt.seconds_float(), KI_LEAK) / ONE_SECOND;
		estimated_altitude_ += alt_err * drone_state.altitude_tracker_kp2_;

		last_baro_reading_ = drone_state.pressure_hpa_;
		estimate_ts_.time_stamp();
	}

	drone_state.altitude_from_acc_ = drone_state.altitude_;
	drone_state.altitude_ = estimated_altitude_;
	drone_state.vertical_speed_ = estimated_velocity_.meters_per_second();
}

void AltitudeTracker::update_state(DroneState& drone_state)
{
	if (!drone_state.altitude_.is_valid()) {
		return;
	}

	if (!starting_altitude_.is_valid()) {
		highest_altitude_ = starting_altitude_ = drone_state.altitude_;
	}

	if (!estimated_altitude_.is_valid()) {
		estimated_altitude_ = drone_state.altitude_;
	}

	estimate_altitude(drone_state);

	if (flight_ceiling_ != drone_state.flight_ceiling_) {
		clear_alarm(); // flight ceiling changed
	}

	flight_ceiling_ = drone_state.flight_ceiling_;
	drone_state.take_off_altitude_ = starting_altitude_;

	if (drone_state.altitude_ > highest_altitude_) {
		highest_altitude_ = drone_state.altitude_;
	}

	if (drone_state.enforce_flight_ceiling_ && !is_flight_ceiling_hit()) {
		if (estimated_altitude_ > starting_altitude_ + flight_ceiling_) {
			if (++alarm_count_ > ALARM_COUNT_THRESHOLD) {
				flight_ceiling_hit_ = true;
			}
		} else {
			alarm_count_ = 0;
		}
	} else {
		if (estimated_altitude_ < (starting_altitude_ + flight_ceiling_ * safe_threshold_)) {
			clear_alarm();
		}
	}
}

Altitude AltitudeTracker::highest_recorded_altitude() const
{
	return highest_altitude_;
}

void AltitudeTracker::clear_alarm()
{
	flight_ceiling_hit_ = false;
	alarm_count_ = 0;
}

bool AltitudeTracker::is_flight_ceiling_hit() const
{
	return flight_ceiling_hit_;
}
