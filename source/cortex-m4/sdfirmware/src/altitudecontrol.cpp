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

//static const Throttle throttle_alt_hold_range = max_throttle_altitude_hold - min_throttle_altitude_hold;

AltitudeControl::AltitudeControl() :
	pid_altitude_(0.08,0.02,0),
	pid_speed_(0.3,0.02,0),
	state_(STATE_LANDED),
	current_vertical_speed_(0)
{
}

Throttle AltitudeControl::get_throttle()
{
	return throttle_;
}

void AltitudeControl::update_state(DroneState& drone_state)
{
	if (drone_state.flight_mode_ != FLIGHT_MODE_ALTITUDE_HOLD) {
		throttle_ = drone_state.base_throttle_;
		return;
	}

	if (last_update_time_.elapsed() < TimeSpan::from_milliseconds(100)) {
		return;
	}

	current_vertical_speed_ = Speed(drone_state.altitude_ - last_altitude_,
			last_update_time_.elapsed());

	dt_ = last_update_time_.elapsed();
	last_update_time_.time_stamp();

	switch (state_) {
	case STATE_LANDED:
		on_state_landed(drone_state);
		break;
	case STATE_TAKE_OFF:
		on_state_takeoff(drone_state);
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

	last_altitude_ = drone_state.altitude_;
	last_base_throttle_ = drone_state.base_throttle_;

	drone_state.flight_posture_ = state_to_string(state_);

	if (drone_state.base_throttle_ < landing_throttle) {
		throttle_ = 0.0;
	}
}


void AltitudeControl::on_state_landed(DroneState& drone_state)
{
	if (is_landing_throttle(drone_state.base_throttle_)) {
		throttle_ = Throttle(0.0f);
		return;
	}
	state_ = STATE_TAKE_OFF;
	throttle_ = drone_state.base_throttle_;
	takeoff_altitude_ = drone_state.altitude_;
}

void AltitudeControl::on_state_takeoff(DroneState& drone_state)
{
	if (is_ascend_throttle(drone_state.base_throttle_)) {
		state_ = STATE_ASCEND;
		pid_speed_.get_d_median(error_as_vertical_speed(drone_state).meters_per_second(),
				dt_.seconds_float());
		pid_speed_.set_integral_error(0.0f);
	}
	throttle_ = drone_state.base_throttle_;
}

void AltitudeControl::on_state_ascend(DroneState& drone_state)
{
	if (is_altitude_hold_throttle(drone_state.base_throttle_)) {
		go_to_state_altitude_hold(drone_state);
	} else {
		if (is_descend_throttle(drone_state.base_throttle_)) {
			state_ = STATE_DESCEND;
		}
		set_throttle_from_speed_pid(drone_state);
	}
}

void AltitudeControl::on_state_altitude_hold(DroneState& drone_state)
{
	float alt_err_pid = pid_altitude_.get_pi_dmedian(error_as_altitude(drone_state).meters(),
			dt_.seconds_float(), 0.1);
	float base_throttle = hovering_throttle(drone_state);
	throttle_ = Throttle(base_throttle - alt_err_pid, landing_throttle, 1.0f);
	if (is_altitude_hold_throttle(drone_state.base_throttle_)) {
		return;
	}
	state_ = is_ascend_throttle(drone_state.base_throttle_) ? STATE_ASCEND : STATE_DESCEND;
	pid_speed_.set_integral_error(0.0f);
}

void AltitudeControl::on_state_descend(DroneState& drone_state)
{
	if (is_altitude_hold_throttle(drone_state.base_throttle_)) {
		go_to_state_altitude_hold(drone_state);
		return;
	}

	if (is_landing_altitude(drone_state) && is_landing_throttle(drone_state.base_throttle_)) {
		state_ = STATE_LANDED;
		throttle_ = 0;
		return;
	}

	if (is_ascend_throttle(drone_state.base_throttle_)) {
		state_ = STATE_ASCEND;
	}
	set_throttle_from_speed_pid(drone_state);
}

void AltitudeControl::go_to_state_altitude_hold(const DroneState& drone_state)
{
	if (drone_state.base_throttle_ == last_base_throttle_) {
		state_ = STATE_ALTITUDE_HOLD;
		pid_altitude_.set_integral_error(0.0f);
		pid_altitude_.get_d_median(error_as_altitude(drone_state).meters(), dt_.seconds_float());
		desired_altitude_ = drone_state.altitude_;
	}
}


Speed AltitudeControl::get_desired_vertical_speed(const DroneState& drone_state)
{
	if (STATE_ASCEND == state_) {
		static const Speed factor = max_vertical_speed / (1.0f - max_throttle_altitude_hold);
		return factor * (drone_state.base_throttle_-max_throttle_altitude_hold);
	} else if (STATE_DESCEND == state_) {
		static const Speed factor = min_vertical_speed / (min_throttle_altitude_hold.get() - 0.0f);
		return -factor * (drone_state.base_throttle_-min_throttle_altitude_hold);
	}
	return 0;
}

Speed AltitudeControl::current_vertical_speed()
{
	return current_vertical_speed_;
}

Speed AltitudeControl::error_as_vertical_speed(const DroneState& drone_state)
{
	return current_vertical_speed_ - get_desired_vertical_speed(drone_state);
}

Altitude AltitudeControl::error_as_altitude(const DroneState& drone_state)
{
	return drone_state.altitude_ - desired_altitude_;
}

void AltitudeControl::set_throttle_from_speed_pid(const DroneState& drone_state)
{
#if 0
	Speed desired_speed = get_desired_vertical_speed(drone_state);
	Speed speed_err = error_as_vertical_speed(drone_state);
	float speed_err_pid = pid_speed_.get_pi_dmedian(
			speed_err.meters_per_second(), dt_.seconds_float(), 0.1);
	speed_err_pid = speed_err_pid / max_vertical_speed.meters_per_second();
	throttle_ =  Throttle(hovering_throttle(drone_state).get()-speed_err_pid, landing_throttle, 1.0f);
#endif
	throttle_ = drone_state.base_throttle_;
}

bool AltitudeControl::is_landing_altitude(const DroneState& drone_state)
{
	return drone_state.altitude_ < takeoff_altitude_ + Altitude::from_meters(0.3);
}

bool AltitudeControl::is_altitude_hold_throttle(const Throttle& t)
{
	return t >= min_throttle_altitude_hold && t <= max_throttle_altitude_hold;
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

Throttle AltitudeControl::hovering_throttle(const DroneState& drone_state)
{
	if (drone_state.battery_percentage_ >= 40) {
		return 0.55f; // TODO: this will have to be measured
	}
	return 0.55f; // TODO: this will have to be measured
}

const char* AltitudeControl::state_to_string(AltitudeControl::State state)
{
	static const std::array<const char*, 5> as_str = {"LANDED","TAKE_OFF","ALTITUDE_HOLD","ASCEND","DESCEND"};
	return (state < as_str.size()) ? as_str[state] : "UNKNOWN";
}
