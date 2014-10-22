#include "attitudetracker.h"

#define USEEARTHATTITUDE 1

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
#ifndef USEEARTHATTITUDE
	QuaternionD deltaq = QuaternionD::fromAngularVelocity(omega, dtime);
	attitude_ = attitude_ * deltaq; 			// attitude_ * deltaq * ~attitude_ * attitude_;
	attitude_ = attitude_.normalize();

#else
	QuaternionD deltaq = QuaternionD::fromAngularVelocity(-omega, dtime);
	attitude_ = (deltaq * attitude_).normalize();
#endif
}

void attitudetracker::track_accelerometer(const Vector3d& g)
{
#ifndef USEEARTHATTITUDE
	Vector3d g_estimated = attitude_.conjugated().rotate(earth_g_.normalize());
	QuaternionD q = QuaternionD::fromVectors(g_estimated, g.normalize());
	QuaternionD deltaq = QuaternionD::nlerp(QuaternionD::identity, q, blend_g_);
	attitude_ = attitude_ * deltaq.conjugated(); // attitude_ * deltaq * ~attitude_ * attitude_;
	attitude_ = attitude_.normalize();
#else
	Vector3d g_estimated = attitude_.rotate(earth_g_.normalize());
	QuaternionD q = QuaternionD::fromVectors(g_estimated, g.normalize());
	QuaternionD deltaq = QuaternionD::nlerp(QuaternionD::identity, q, blend_g_);
	attitude_ = (deltaq * attitude_).normalize();
#endif
}

void attitudetracker::track_magnetometer(const Vector3d& m)
{
	g2m_q_ = QuaternionD::fromVectors(attitude_.rotate(earth_g_.normalize()), m.normalize());

	Vector3d m_estimated = attitude_.rotate(earth_m_.normalize());
	QuaternionD q = QuaternionD::fromVectors(m_estimated, m.normalize());
	QuaternionD deltaq = QuaternionD::nlerp(QuaternionD::identity, q, blend_g_/2.0);

#if 1
	QuaternionD g_estimated(attitude_.rotate(earth_g_.normalize()), 0.0);
	QuaternionD qin(deltaq.axis(), 0.0);
	QuaternionD qper = (qin - g_estimated * qin * g_estimated) * 1.0 / 2.0;
	deltaq = QuaternionD::fromAxisRot(qper.axis(), deltaq.angle());
#endif

	attitude_ = (deltaq.normalize() * attitude_).normalize();


#if 0
	/*
	 * Earth plane represented by its normal vector
	 */
	QuaternionD earth_plane_q(attitude_.rotate(earth_g_.normalize()));

	/*
	 * Magnetic field as Quaternion
	 */
	QuaternionD magnetic_field_q(magnetic_field.normalize());
	QuaternionD magnetic_field_estimated_q = QuaternionD(attitude_.rotate(Vector3d(0.06, -0.194, -0.978).normalize()));
	QuaternionD magnetic_field_proj_q = (magnetic_field_q + earth_plane_q * magnetic_field_q * earth_plane_q) * 1.0 / 2.0;
	QuaternionD magnetic_field_estimated_proj_q = (magnetic_field_estimated_q + earth_plane_q * magnetic_field_estimated_q * earth_plane_q) * 1.0 / 2.0;
	Vector3d m_estimated(magnetic_field_estimated_proj_q.x, magnetic_field_estimated_proj_q.y, magnetic_field_estimated_proj_q.z);
	Vector3d m(magnetic_field_proj_q.x, magnetic_field_proj_q.y, magnetic_field_proj_q.z);
#endif
}

QuaternionD attitudetracker::get_attitude() const
{
#ifndef USEEARTHATTITUDE
	return attitude_;
#else
	return attitude_.conjugated();
#endif
}

void attitudetracker::set_attitude(const QuaternionD& attitude)
{
#ifndef USEEARTHATTITUDE
	attitude_ = attitude;
#else
	attitude_ = attitude.conjugated();
#endif
}

void attitudetracker::reset_attitude()
{
	set_attitude(QuaternionD::identity);
}
