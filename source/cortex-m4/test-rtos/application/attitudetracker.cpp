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
	if (g.length() < 0.85 || g.length() > 1.15)
		return;
	/*
	 * Estimate after rotating the initial vector with the
	 * world attitude quaternion.
	 */
	Vector3f g_estimated = get_world_attitude().rotate(earth_g_);

	/*
	 * Calculate the rotation between the estimated vector
	 * ant the one received by the sensor.
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
