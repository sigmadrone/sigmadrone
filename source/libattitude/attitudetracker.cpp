#include "attitudetracker.h"


attitudetracker::attitudetracker(double blend_g, Vector3d earth_g)
	: blend_g_(blend_g)
	, earth_g_(earth_g)
	, world_attitude_(QuaternionD::identity)
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
	QuaternionD deltaq = QuaternionD::fromAngularVelocity(-omega, dtime);
	world_attitude_ = (deltaq * world_attitude_).normalize();
}

void attitudetracker::track_accelerometer(const Vector3d& g)
{
	Vector3d g_estimated = world_attitude_.rotate(earth_g_.normalize());
	QuaternionD q = QuaternionD::fromVectors(g_estimated, g.normalize());
	QuaternionD deltaq = QuaternionD::nlerp(QuaternionD::identity, q, blend_g_);
	world_attitude_ = (deltaq * world_attitude_).normalize();
}

void attitudetracker::track_magnetometer(const Vector3d& m)
{
	g2m_q_ = QuaternionD::fromVectors(world_attitude_.rotate(earth_g_.normalize()), m.normalize());

	Vector3d m_estimated = world_attitude_.rotate(get_earth_m().normalize());
	QuaternionD q = QuaternionD::fromVectors(m_estimated, m.normalize());
	QuaternionD deltaq = QuaternionD::nlerp(QuaternionD::identity, q, blend_g_/3.0);
//	attitude_ = (deltaq.normalize() * attitude_).normalize();
}

QuaternionD attitudetracker::get_attitude() const
{
	return world_attitude_.conjugated();
}

QuaternionD attitudetracker::get_world_attitude() const
{
	return world_attitude_.conjugated();
}


void attitudetracker::reset_attitude()
{
	world_attitude_ = QuaternionD::identity;
}
