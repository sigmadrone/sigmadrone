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
#include "attitudetracker.h"

#include <iostream>

attitudetracker::attitudetracker(float accelerometer_correction_speed, Vector3f earth_g, float alpha)
	: accelerometer_correction_speed_(accelerometer_correction_speed)
	, alpha_(alpha)
	, earth_g_(earth_g)
	, attitude_(QuaternionF::identity)
	, drift_pid_(0, 0.01, 0)
    , drift_leak_rate_(0.00001)
{
}

attitudetracker::~attitudetracker()
{
}

Vector3f attitudetracker::get_earth_g() const
{
	return earth_g_;
}

Vector3f attitudetracker::get_drift_error() const
{
	return drift_err_;
}

void attitudetracker::set_earth_g(Vector3f earth_g)
{
	earth_g_ = earth_g;
}

Vector3f attitudetracker::get_earth_m() const
{
	return earth_m_;
}

void attitudetracker::set_earth_m(Vector3f earth_m)
{
	earth_m_ = earth_m;
}

void attitudetracker::accelerometer_correction_speed(float accelerometer_correction_speed)
{
	accelerometer_correction_speed_ = accelerometer_correction_speed;
}

void attitudetracker::gyro_drift_pid(float kp, float ki, float kd)
{
	drift_pid_.set_kp_ki_kd(kp, ki, kd);
}

void attitudetracker::gyro_drift_leak_rate(float leak_rate)
{
	drift_leak_rate_ = leak_rate;
}

void attitudetracker::track_gyroscope(const Vector3f& omega, float dtime)
{
	QuaternionF deltaq = QuaternionF::fromAngularVelocity(omega - drift_err_, dtime);
	attitude_ = (attitude_ * deltaq).normalize();
}

void attitudetracker::track_accelerometer(const Vector3f& g, float dtime)
{
	/*
	 * Estimate after rotating the initial earth acceleration
	 * vector with the world attitude quaternion.
	 */
	Vector3f g_estimated = get_world_attitude().rotate(earth_g_);

	/*
	 * Calculate the rotation between the estimated earth acceleration
	 * vector and the one detected by the accelerometer sensor.
	 */
	QuaternionF q = QuaternionF::fromVectors(g_estimated, g);

	/*
	 * Generate angular velocity to adjust our attitude in the
	 * direction of the sensor reading. We assume that our
	 * attitude quaternion has accumulated error, so we will
	 * try to correct it by calculating a delta quaternion that
	 * when adjust our attitude with it will better align
	 * the estimated earth acceleration with one detected by
	 * the sensor.
	 */
	Vector3f w = QuaternionF::angularVelocity(QuaternionF::identity, q, q.angle() / DEG2RAD(accelerometer_correction_speed_));
	filtered_w_ = filtered_w_ * alpha_ + w * (1.0f - alpha_);
	w = filtered_w_;
	drift_err_ = drift_pid_.get_pid(w, dtime, drift_leak_rate_);
	QuaternionF deltaq = QuaternionF::fromAngularVelocity(-w, dtime);
	attitude_ = (attitude_ * deltaq).normalize();
}

void attitudetracker::track_magnetometer(const Vector3f& m, float dtime)
{
	if (fabs(attitude_.x) > 0.04 || fabs(attitude_.y) > 0.04) {
		/*
		 * Ignore magnetometer reading if the system is sufficiently tilted
		 * from the XY plane
		 */
		return;
	}

	/*
	 * Ignore the Z component from the reading
	 */
	Vector3f mag_xy = Vector3f(m.at(0), m.at(1), 0.0).normalize();

	/*
	 * Estimate the mag_xy using the earth quaternion. Note: here we assume that
	 * when the aircraft is pointed to the magnetic north, the reading normalized
	 * in the XY plane will be [1,0,0].
	 */
	Vector3f mag_xy_estimated = get_world_attitude().rotate(Vector3f(1,0,0));

	/*
	 * Nullify the Z component and normalize
	 */
	mag_xy_estimated.at(2) = 0.0;
	mag_xy_estimated = mag_xy_estimated.normalize();

	/*
	 * Calculate the rotation between the estimated vector
	 * and the one received by the sensor.
	 */
	QuaternionF q = QuaternionF::fromVectors(mag_xy_estimated, mag_xy);

	/*
	 * Generate angular velocity to adjust our attitude in the
	 * direction of the sensor reading.
	 */
	Vector3f w = QuaternionF::angularVelocity(QuaternionF::identity, q, 10);
	if (w.length() != 0.0) {
		QuaternionF deltaq = QuaternionF::fromAngularVelocity(-w, dtime);
		attitude_ = (attitude_ * deltaq).normalize();
	}
}

QuaternionF attitudetracker::get_attitude() const
{
	return attitude_;
}

QuaternionF attitudetracker::get_world_attitude() const
{
	return attitude_.conjugate();
}


void attitudetracker::reset_attitude()
{
	attitude_ = QuaternionF::identity;
}

Vector3d attitudetracker::get_alignment_speed() const
{
	return filtered_w_;
}
