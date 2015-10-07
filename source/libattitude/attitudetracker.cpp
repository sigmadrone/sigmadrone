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

attitudetracker::attitudetracker(double accelerometer_correction_period, Vector3d earth_g)
	: accelerometer_correction_period_(accelerometer_correction_period)
	, earth_g_(earth_g)
	, attitude_(QuaternionD::identity)
{

}

attitudetracker::~attitudetracker()
{
}

Vector3d attitudetracker::get_earth_g() const
{
	return earth_g_;
}

void attitudetracker::set_earth_g(Vector3d earth_g)
{
	earth_g_ = earth_g;
}

Vector3d attitudetracker::get_earth_m() const
{
	return earth_m_;
}

void attitudetracker::set_earth_m(Vector3d earth_m)
{
	earth_m_ = earth_m;
}

void attitudetracker::accelerometer_correction_period(double accelerometer_correction_period)
{
	accelerometer_correction_period_ = accelerometer_correction_period;
}

void attitudetracker::track_gyroscope(const Vector3d& omega, double dtime)
{
	QuaternionD deltaq = QuaternionD::fromAngularVelocity(omega, dtime);
	attitude_ = (attitude_ * deltaq).normalize();
}

void attitudetracker::track_accelerometer(const Vector3d& g, double dtime)
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
	Vector3d g_estimated = get_world_attitude().rotate(earth_g_);

	/*
	 * Calculate the rotation between the estimated vector
	 * ant the one received by the sensor.
	 */
	QuaternionD q = QuaternionD::fromVectors(g_estimated, g);

	/*
	 * Generate angular velocity to adjust our attitude in the
	 * direction of the sensor reading.
	 */
	Vector3d w = QuaternionD::angularVelocity(QuaternionD::identity, q, accelerometer_correction_period_);
	if (w.length() == 0.0)
		return;
	QuaternionD deltaq = QuaternionD::fromAngularVelocity(-w, dtime);
	attitude_ = (attitude_ * deltaq).normalize();
}

void attitudetracker::track_magnetometer(const Vector3d& m, double dtime)
{

}

QuaternionD attitudetracker::get_attitude() const
{
	return attitude_;
}

QuaternionD attitudetracker::get_world_attitude() const
{
	return attitude_.conjugate();
}


void attitudetracker::reset_attitude()
{
	attitude_ = QuaternionD::identity;
}
