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

AltitudeControl::AltitudeControl() : state_(STATE_LANDED)
{
}

Throttle AltitudeControl::get_throttle()
{
	return throttle_;
}

void AltitudeControl::update_state(DroneState& drone_state)
{
	if (drone_state.flight_mode_ < FLIGHT_MODE_ALTITUDE_HOLD) {
		throttle_ = drone_state.base_throttle_;
		return;
	}

	pid_vert_speed_.set_kp_ki_kd(drone_state.altitude_kp_, drone_state.altitude_ki_, drone_state.altitude_kd_);

	dt_ = last_update_time_.elapsed();
	drone_state.target_altitude_ = drone_state.altitude_;

	switch (state_) {
	case STATE_LANDED:
		on_state_landed(drone_state);
		break;
	case STATE_ALTITUDE_HOLD:
		on_state_altitude_hold(drone_state);
		break;
	case STATE_ASCEND:
		on_state_ascend(drone_state);
		break;
	case STATE_DESCEND:
		on_state_descend(drone_state);
		break;
	default: assert(false);
	}

	if (last_base_throttle_.get() != drone_state.base_throttle_) {
		last_base_throttle_ = Throttle(drone_state.base_throttle_);
		throttle_hold_.time_stamp();
	}

	last_update_time_.time_stamp();

	drone_state.flight_posture_ = state_to_string(state_);
	drone_state.target_vertical_speed_ = get_target_vertical_speed(drone_state);
}


void AltitudeControl::on_state_landed(DroneState& drone_state)
{
	if (is_ascend_throttle(drone_state.base_throttle_)) {
		state_ = STATE_ASCEND;
	} else if (drone_state.base_throttle_ > landing_throttle) {
		state_ = STATE_DESCEND;
	}
	throttle_ = drone_state.base_throttle_;
	takeoff_altitude_ = drone_state.altitude_;
}

void AltitudeControl::on_state_ascend(DroneState& drone_state)
{
	if (is_altitude_hold_throttle(drone_state.base_throttle_)) {
		go_to_state_altitude_hold(drone_state);
	} else if (is_descend_throttle(drone_state.base_throttle_)) {
		state_ = STATE_DESCEND;
	}
	set_throttle_from_ascend_descend(drone_state);
}

void AltitudeControl::on_state_altitude_hold(DroneState& drone_state)
{
	if (is_ascend_throttle(drone_state.base_throttle_)) {
		state_ = STATE_ASCEND;
	} else if (is_descend_throttle(drone_state.base_throttle_)) {
		state_ = STATE_DESCEND;
	}
	drone_state.target_altitude_ = target_altitude_;
	set_throttle_from_altitude_hold(drone_state);
}

void AltitudeControl::on_state_descend(DroneState& drone_state)
{
	if (is_landing_throttle(drone_state.base_throttle_)) {
		state_ = STATE_LANDED;
		return;
	}

	if (is_altitude_hold_throttle(drone_state.base_throttle_)) {
		go_to_state_altitude_hold(drone_state);
	} else if (is_ascend_throttle(drone_state.base_throttle_)) {
		state_ = STATE_ASCEND;
	}
	set_throttle_from_ascend_descend(drone_state);
}

void AltitudeControl::go_to_state_altitude_hold(const DroneState& drone_state)
{
	state_ = STATE_ALTITUDE_HOLD;
	target_altitude_ = drone_state.altitude_;
}


Speed AltitudeControl::get_target_vertical_speed(const DroneState& drone_state)
{
	if (is_landing_throttle(drone_state.base_throttle_)) {
		return Speed();
	} else if (is_ascend_throttle(drone_state.base_throttle_)) {
		static const Speed factor = max_vertical_speed / (1.0f - max_throttle_altitude_hold.get());
		return factor * (drone_state.base_throttle_-max_throttle_altitude_hold);
	} else if (is_descend_throttle(drone_state.base_throttle_)) {
		static const Speed factor = min_vertical_speed / min_throttle_altitude_hold.get();
		return -factor * (drone_state.base_throttle_-min_throttle_altitude_hold);
	}
	return Speed();
}

Speed AltitudeControl::error_as_vertical_speed(const DroneState& drone_state)
{
	return get_target_vertical_speed(drone_state) - drone_state.vertical_speed_;
}

Altitude AltitudeControl::error_as_altitude(const DroneState& drone_state)
{
	return target_altitude_- drone_state.altitude_;
}

void AltitudeControl::set_throttle_from_ascend_descend(const DroneState& drone_state)
{
	Speed err = error_as_vertical_speed(drone_state);
	float err_pid = pid_vert_speed_.get_pi_dmedian(err.meters_per_second(), dt_.seconds_float(),
			drone_state.altitude_ki_leak_);
	throttle_ = Throttle(drone_state.base_throttle_ + err_pid, landing_throttle, 1.0f);
}

void AltitudeControl::set_throttle_from_altitude_hold(const DroneState& drone_state)
{
	Speed err = error_as_vertical_speed(drone_state);
	err += error_as_altitude(drone_state) / drone_state.altitude_correction_period_;
	float err_pid = pid_vert_speed_.get_pi_dmedian(err.meters_per_second(), dt_.seconds_float(),
			drone_state.altitude_ki_leak_);
	throttle_ = Throttle(drone_state.base_throttle_ + err_pid, landing_throttle, 1.0f);
}

bool AltitudeControl::is_altitude_hold_throttle(const Throttle& t)
{
	if (throttle_hold_.elapsed() < TimeSpan::from_milliseconds(200)) {
		return false;
	}
	return (t >= min_throttle_altitude_hold && t <= max_throttle_altitude_hold) ? true : false;
}

bool AltitudeControl::is_ascend_throttle(const Throttle& t)
{
	return t > max_throttle_altitude_hold;
}

bool AltitudeControl::is_descend_throttle(const Throttle& t)
{
	return t < min_throttle_altitude_hold;
}

bool AltitudeControl::is_landing_throttle(const Throttle& t)
{
	return t <= landing_throttle;
}

const char* AltitudeControl::state_to_string(AltitudeControl::State state)
{
	static const std::array<const char*, 5> as_str = {"Landed","Altitude Hold","Ascend","Descend"};
	return (state < as_str.size()) ? as_str[state] : "Unknown";
}
