#include "attitudetracker.h"

#include <iostream>

attitudetracker::attitudetracker(Vector3d earth_g, double accelerometer_correction_period)
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
	if (g.length() < 0.85 || g.length() > 1.15)
		return;
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
