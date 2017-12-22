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

PositionControl::PositionControl() : distance_lpf_(0.99), axis_lpf_(0.9), rc_value_pitch_(0.0f), rc_value_roll_(0.0f)
{
}

void PositionControl::update_state(DroneState& state)
{
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
		state.position_error_ = Distance::from_meters(0);
	}
}

bool PositionControl::is_tracking()
{
	return target_location_.is_valid();
}

bool PositionControl::should_stop_tracking(const DroneState& state)
{
	return has_rc_value_changed(state.pitch_, rc_value_pitch_) ||
			has_rc_value_changed(state.roll_, rc_value_roll_) ||
			(state.flight_mode_ != FLIGHT_MODE_POSITION_HOLD && state.flight_mode_ != FLIGHT_MODE_MISSION);
}

bool PositionControl::should_start_tracking(const DroneState& state)
{
	if (!state.motors_armed_) {
		return false;
	}
	if (state.flight_mode_ != FLIGHT_MODE_POSITION_HOLD && state.flight_mode_ != FLIGHT_MODE_MISSION) {
		return false;
	}

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
	target_swing_ = QuaternionF();
}

void PositionControl::start_tracking(const DroneState& state)
{
	if (should_start_tracking(state)) {
		target_location_ = GeoLocation(state.latitude_, state.longitude_, state.altitude_);
		pid_ = PidControllerType(state.position_kp_, state.position_ki_, state.position_kd_);
		distance_lpf_.reset();
		axis_lpf_.reset(state.target_swing_.axis());
	}
}

const QuaternionF& PositionControl::target_swing() const
{
	return target_swing_;
}

void PositionControl::record_base_rc_values_if_landed(const DroneState& state)
{
	if (is_landed(state) && rc_record_ts_.elapsed() > TimeSpan::from_milliseconds(600)) {
		rc_value_pitch_ = state.pitch_;
		rc_value_roll_ = state.roll_;
		rc_record_ts_.time_stamp();
	}
}

bool PositionControl::is_landed(const DroneState& state)
{
	return (state.base_throttle_ <= 0.2f); // start out simple
}

bool PositionControl::has_rc_value_changed(float oldVal, float newVal)
{
	return fabs(oldVal - newVal) > 0.02;
}

void PositionControl::calculate_target_swing(DroneState& state)
{
	Longitude longitude = state.longitude_;
	Latitude latitude = state.latitude_;
	if (!longitude.is_valid() || !latitude.is_valid()) {
		stop_tracking();
		return;
	}

	pid_.set_kp_ki_kd(state.position_kp_, state.position_ki_, state.position_kd_);

	Distance err_dist = get_error_as_distance(latitude, longitude);
	Angle error_angle = map_distance_err_to_swing_angle(distance_lpf_.do_filter(err_dist));
	Angle swing_angle = pid_.get_pi_dmedian(error_angle, state.dt_.seconds_float(), 0.01);
	swing_angle = clip_swing_angle(swing_angle);

	Vector3f err_vec = get_error_vector(state.attitude_, latitude, longitude);
	Vector3f swing_axis = axis_lpf_.do_filter(qt_ortho_xy_.rotate(err_vec));

	state.target_swing_ = target_swing_ = QuaternionF::fromAxisRot(swing_axis, swing_angle.radians());

	state.position_error_ = err_dist;
}

Distance PositionControl::get_error_as_distance(const Latitude& latitude, const Longitude& longitude)
{
	float meters = TinyGPS::distance_between(target_location_.latitude().degrees_d(),
			target_location_.longitude().degrees_d(),
			latitude.degrees_d(),
			longitude.degrees_d());
	return Distance::from_meters(meters);
}

Vector3f PositionControl::get_error_vector(
		const QuaternionF& attitude,
		const Latitude& latitude,
		const Longitude& longitude)
{
	int32_t long_err = (longitude.get() - target_location_.longitude().get()).millionth_degrees();
	int32_t lat_err = (latitude.get() - target_location_.latitude().get()).millionth_degrees();
	Vector3f err_d = Vector3f(long_err, lat_err, 0).normalize();

	Vector3f err((float)err_d[1], (float)err_d[0], 0);
	// Rotate the error vector with drone's current twist, so we can calculate the proper
	// swing later
	QuaternionF twist, swing;
	QuaternionF::decomposeTwistSwing(~attitude, Vector3f(0,0,1), swing, twist);
	err = twist.rotate(err);
	return err.normalize();
}

Angle PositionControl::clip_swing_angle(const Angle& angle)
{
	return std::min(std::max(angle, min_swing_angle_), max_swing_angle_);
}

Angle PositionControl::map_distance_err_to_swing_angle(const Distance& err_dist)
{
	static const Distance gps_err = Distance::from_meters(0.5); //1
	static const Distance bubble_dist = Distance::from_meters(10);
	static const Distance max_err_dist = Distance::from_meters(50);
	static const float bubble_dist_factor = 30;
	static const float max_dist_factor = 45;

	if (err_dist < gps_err) {
		return Angle::from_degrees(0);
	}
	Distance adj_err_dist = err_dist - gps_err;
	Angle swing_angle = Angle::from_degrees(0);
	if (adj_err_dist <= bubble_dist)
	{
		swing_angle = Angle::from_degrees(adj_err_dist / bubble_dist * bubble_dist_factor);
	}
	else
	{
		swing_angle = Angle::from_degrees((adj_err_dist-bubble_dist) / max_err_dist * max_dist_factor + bubble_dist_factor);
	}
	return swing_angle;
}

