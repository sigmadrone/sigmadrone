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
#include "positioncontrol.h"
#include "libgps/TinyGPS.h"

const QuaternionF PositionControl::qt_ortho_xy_ = QuaternionF::fromAxisRot({0,0,1}, M_PI/2);
const Angle PositionControl::max_swing_angle_ = Angle::from_radians(Angle::PI/4);
const Angle PositionControl::min_swing_angle_ = Angle::from_radians(-Angle::PI/4);

PositionControl::PositionControl() : rc_value_pitch_(0.0f), rc_value_roll_(0.0f)
{
}

void PositionControl::update_state(DroneState& state)
{
	if (!state.motors_armed_ || state.flight_mode_ != FLIGHT_MODE_ALTITUDE_HOLD) {
		return;
	}

	if (is_tracking()) {
		if (should_stop_tracking(state)) {
			stop_tracking();
		} else {
			calculate_target_swing(state);
		}
	} else {
		if (should_start_tracking(state)) {
			start_tracking(state);
		} else {
			record_base_rc_values_if_landed(state);
		}
	}
}

bool PositionControl::is_tracking()
{
	return target_location_.is_valid();
}

bool PositionControl::should_stop_tracking(const DroneState& state)
{
	return has_rc_value_changed(state.pitch_, rc_value_pitch_) ||
			has_rc_value_changed(state.roll_, rc_value_roll_);
}

bool PositionControl::should_start_tracking(const DroneState& state)
{
	if (has_rc_value_changed(rc_value_pitch_, state.pitch_) || has_rc_value_changed(rc_value_roll_, state.roll_)) {
		return false;
	}
	if (rc_record_ts_.elapsed() < TimeSpan::from_milliseconds(500)) {
		return false;
	}
	return state.latitude_.is_valid() && state.longitude_.is_valid();
}

void PositionControl::stop_tracking()
{
	target_location_ = GeoLocation();
}

void PositionControl::start_tracking(const DroneState& state)
{
	if (should_start_tracking(state)) {
		target_location_ = GeoLocation(state.latitude_, state.longitude_, state.altitude_);
		pid_ = PidControllerType(state.position_kp_, state.position_ki_, state.position_kd_);
		dt_.time_stamp();
	}
}

const QuaternionF& PositionControl::target_swing() const
{
	return target_swing_;
}

void PositionControl::record_base_rc_values_if_landed(const DroneState& state)
{
	if (is_landed(state))
	{
		rc_value_pitch_ = state.pitch_;
		rc_value_roll_ = state.roll_;
	}
	rc_record_ts_.time_stamp();
}

bool PositionControl::is_landed(const DroneState& state)
{
	return (state.throttle_ <= 0.2f); // start out simple
}

bool PositionControl::has_rc_value_changed(float oldVal, float newVal)
{
	return fabs(oldVal - newVal) > 0.02;
}

void PositionControl::calculate_target_swing(const DroneState& state)
{
	Distance errDist = get_error_as_distance(state);
	Vector3f errVec = get_error_vector(state);
	Vector3f swingAxis = qt_ortho_xy_.rotate(errVec);
	Angle errorAngle = map_distance_err_to_swing_angle(errDist);
	Angle swingAngle = pid_.get_pi_dmedian(errorAngle, dt_.elapsed().seconds(), 0.01);
	swingAngle = clip_swing_angle(swingAngle);
	target_swing_ = QuaternionF::fromAxisRot(swingAxis, -swingAngle.radians());
	dt_.time_stamp();
}

Distance PositionControl::get_error_as_distance(const DroneState& state)
{
	if (!state.longitude_.is_valid() || !state.latitude_.is_valid()) {
		stop_tracking();
		return Distance::from_meters(0);
	}
	float meters = TinyGPS::distance_between(target_location_.latitude().degrees(),
			target_location_.longitude().degrees(),
			state.latitude_.degrees(),
			state.longitude_.degrees());
	return Distance::from_meters(meters);
}

Vector3f PositionControl::get_error_vector(const DroneState& state)
{
	float lon_err = target_location_.longitude().degrees() - state.longitude_.degrees();
	float lat_err = target_location_.latitude().degrees() - state.latitude_.degrees();
	Vector3f err(lon_err, lat_err, 0);
	return err.normalize();
}

Angle PositionControl::clip_swing_angle(const Angle& angle)
{
	return unit_min(unit_max(angle, min_swing_angle_), max_swing_angle_);
}

Angle PositionControl::map_distance_err_to_swing_angle(const Distance& err_dist)
{
	static const Distance bubble_dist = Distance::from_meters(5);
	static const Distance max_err_dist = Distance::from_meters(25);
	static const float bubble_dist_factor = 15;
	static const float max_dist_factor = 30;
	Angle swing_angle;
	if (err_dist <= bubble_dist)
	{
		swing_angle = Angle::from_degrees(err_dist / bubble_dist * bubble_dist_factor);
	}
	else
	{
		swing_angle = Angle::from_degrees((err_dist-bubble_dist) / max_err_dist * max_dist_factor + bubble_dist_factor);
	}
	return clip_swing_angle(swing_angle);
}

