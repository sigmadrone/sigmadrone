#ifndef ATTITUDETRACKER_H_
#define ATTITUDETRACKER_H_

#include "matrix.h"

class attitudetracker
{
public:
	attitudetracker(Vector3d earth_g = Vector3d(0, 0, 1), double accelerometer_correction_period = 3.5);
	~attitudetracker();
	Vector3d get_earth_g() const;
	void set_earth_g(Vector3d earth_g);
	Vector3d get_earth_m() const;
	void set_earth_m(Vector3d earth_m);
	void track_gyroscope(const Vector3d& omega, double dtime);
	void track_accelerometer(const Vector3d& g, double dtime);
	void track_magnetometer(const Vector3d& m, double dtime);
	/*
	 * Return our attitude in the world coordinate system
	 */
	QuaternionD get_attitude() const;

	/*
	 * Return the world attitude in our coordinate system
	 */
	QuaternionD get_world_attitude() const;
	void reset_attitude();

public:
	/*
	 * The earth M vector reading while in initial position.
	 */
	Vector3d earth_m_;
	double accelerometer_correction_period_;

protected:
	/*
	 * The earth G vector reading while in leveled position.
	 */
	Vector3d earth_g_;

	/*
	 * Current attitude of the world. We assume that
	 * the world is rotating in our coordinate system.
	 */
	QuaternionD attitude_;

	/*
	 * Angular velocity integral
	 */
	Vector3d integral_w_;

};

#endif
