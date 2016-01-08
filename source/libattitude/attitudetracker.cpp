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

attitudetracker::attitudetracker(double accelerometer_correction_period, Vector3f earth_g)
	: accelerometer_correction_period_(accelerometer_correction_period)
	, earth_g_(earth_g)
	, attitude_(QuaternionF::identity)
{
}

attitudetracker::~attitudetracker()
{
}

Vector3f attitudetracker::get_earth_g() const
{
	return earth_g_;
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

void attitudetracker::accelerometer_correction_period(double accelerometer_correction_period)
{
	accelerometer_correction_period_ = accelerometer_correction_period;
}

void attitudetracker::track_gyroscope(const Vector3f& omega, double dtime)
{
	QuaternionF deltaq = QuaternionF::fromAngularVelocity(omega, dtime);
	attitude_ = (attitude_ * deltaq).normalize();
}

void attitudetracker::track_accelerometer(const Vector3f& g, double dtime)
{
	/*
	 * if the length of the g is not close to 1,
	 * don't use it.
	 *
	 */
#if 1
	if (g.length() < 0.85 || g.length() > 1.15)
		return;
#endif
	/*
	 * Estimate after rotating the initial vector with the
	 * world attitude quaternion.
	 */
	Vector3f g_estimated = get_world_attitude().rotate(earth_g_);

	/*
	 * Calculate the rotation between the estimated vector
	 * and the one received by the sensor.
	 */
	QuaternionF q = QuaternionF::fromVectors(g_estimated, g);

	/*
	 * Generate angular velocity to adjust our attitude in the
	 * direction of the sensor reading.
	 */
	Vector3f w = QuaternionF::angularVelocity(QuaternionF::identity, q, accelerometer_correction_period_);
	if (w.length() == 0.0)
		return;
	QuaternionF deltaq = QuaternionF::fromAngularVelocity(-w, dtime);
	attitude_ = (attitude_ * deltaq).normalize();
}

void attitudetracker::track_magnetometer(const Vector3f& m, double dtime)
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
