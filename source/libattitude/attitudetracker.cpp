#include "attitudetracker.h"

attitudetracker::attitudetracker(double blend_g, Vector3d earth_g)
	: blend_g_(blend_g)
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

double attitudetracker::get_nlerp_blend() const
{
	return blend_g_;
}

void attitudetracker::set_nlerp_blend(double blend_g)
{
	blend_g_ = blend_g;
}

void attitudetracker::track_gyroscope(const Vector3d& omega, double dtime)
{
	QuaternionD deltaq = QuaternionD::fromAngularVelocity(omega, dtime);
	attitude_ = attitude_ * deltaq; 			// attitude_ * deltaq * ~attitude_ * attitude_;
	attitude_ = attitude_.normalize();
}

void attitudetracker::track_accelerometer(const Vector3d& g)
{
	Vector3d g_estimated = attitude_.conjugated().rotate(earth_g_.normalize());
	QuaternionD q = QuaternionD::fromVectors(g_estimated, g.normalize());
	QuaternionD deltaq = QuaternionD::nlerp(QuaternionD::identity, q, blend_g_);
	attitude_ = attitude_ * deltaq.conjugated(); // attitude_ * deltaq * ~attitude_ * attitude_;
	attitude_ = attitude_.normalize();
}

void attitudetracker::track_magnetometer(const Vector3d& m)
{
	/*
	 * TBD
	 */
}

QuaternionD attitudetracker::get_attitude() const
{
	return attitude_;
}

void attitudetracker::set_attitude(const QuaternionD& attitude)
{
	attitude_ = attitude;
}

void attitudetracker::reset_attitude()
{
	set_attitude(QuaternionD::identity);
}
