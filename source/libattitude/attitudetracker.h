#ifndef ATTITUDETRACKER_H_
#define ATTITUDETRACKER_H_

#include "matrix.h"

class attitudetracker
{
public:
	attitudetracker(double blend_g = 0.00175, Vector3d earth_g = Vector3d(0, 0, 1));
	~attitudetracker();
	Vector3d get_earth_g() const;
	void set_earth_g(Vector3d earth_g);
	Vector3d get_earth_m() const;
	void set_earth_m(Vector3d earth_m);
	double get_nlerp_blend() const;
	void set_nlerp_blend(double blend_g);
	void track_gyroscope(const Vector3d& omega, double dtime);
	void track_accelerometer(const Vector3d& g);
	void track_magnetometer(const Vector3d& m);
	QuaternionD get_attitude() const;
	void set_attitude(const QuaternionD& attitude);
	void reset_attitude();

public:
	/*
	 * The earth M vector reading while in initial position.
	 */
	Vector3d earth_m_;
	QuaternionD g2m_q_;

protected:
	/*
	 * This is the blend parameter for the Quaternion::nlerp
	 * used to approximate between the estimated quaternion
	 * and the one measured from the accelerometer.
	 */
	double blend_g_;

	/*
	 * The earth G vector reading while in leveled position.
	 */
	Vector3d earth_g_;

	/*
	 * Current attitude.
	 */
	QuaternionD attitude_;
};

#endif
