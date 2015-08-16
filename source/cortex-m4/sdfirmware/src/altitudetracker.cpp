/*
 * altitudetracker.cpp
 *
 *  Created on: Aug 15, 2015
 *      Author: svassilev
 */

#include "altitudetracker.h"

AltitudeTracker::AltitudeTracker(const Altitude& ceiling, float safe_threshold) :
	flight_ceiling_(ceiling), starting_altitude_(INVALID_ALTITUDE), alarm_count_(0),
	safe_threshold_(safe_threshold), flight_ceiling_hit_(false) {
	assert(safe_threshold >= 0.0f && safe_threshold_ <= 1.0f);
}

AltitudeTracker::~AltitudeTracker() {
}

Altitude AltitudeTracker::flight_ceiling() const {
	return flight_ceiling_;
}

void AltitudeTracker::update_state(DroneState& drone_state) {
	if (!drone_state.altitude_.is_valid()) {
		return;
	}

	if (!starting_altitude_.is_valid()) {
		highest_altitude_ = starting_altitude_ = drone_state.altitude_;
	}

	current_altitude_ = drone_state.altitude_;
	if (drone_state.altitude_ > highest_altitude_) {
		highest_altitude_ = drone_state.altitude_;
	}

	if (!is_flight_ceiling_hit()) {
		if (current_altitude_ > starting_altitude_ + flight_ceiling_) {
			if (++alarm_count_ > ALARM_COUNT_THRESHOLD) {
				flight_ceiling_hit_ = true;
			}
		} else {
			alarm_count_ = 0;
		}
	} else {
		if (current_altitude_ < (starting_altitude_ + flight_ceiling_ * safe_threshold_)) {
			clear_alarm();
		}
	}
}

Altitude AltitudeTracker::highest_recorded_altitude() const {
	return highest_altitude_;
}

void AltitudeTracker::flight_ceiling(const Altitude& new_ceiling) {
	flight_ceiling_ = new_ceiling;
	clear_alarm();
}

void AltitudeTracker::clear_alarm() {
	flight_ceiling_hit_ = false;
	alarm_count_ = 0;
}

bool AltitudeTracker::is_flight_ceiling_hit() const {
	return flight_ceiling_hit_;
}
