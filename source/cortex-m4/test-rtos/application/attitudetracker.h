#ifndef ATTITUDETRACKER_H_
#define ATTITUDETRACKER_H_

#include "d3math.h"

class attitudetracker
{
public:
	explicit attitudetracker(double accelerometer_correction_period = 1.5, Vector3f earth_g = Vector3f(0, 0, 1));
	~attitudetracker();
	Vector3f get_earth_g() const;
	void set_earth_g(Vector3f earth_g);
	Vector3f get_earth_m() const;
	void set_earth_m(Vector3f earth_m);
	void track_gyroscope(const Vector3f& omega, double dtime);
	void track_accelerometer(const Vector3f& g, double dtime);
	void track_magnetometer(const Vector3f& m, double dtime);
	/*
	 * Return our attitude in the world coordinate system
	 */
	QuaternionF get_attitude() const;

	/*
	 * Return the world attitude in our coordinate system
	 */
	QuaternionF get_world_attitude() const;
	void reset_attitude();

public:
	/*
	 * The earth M vector reading while in initial position.
	 */
	Vector3f earth_m_;
	double accelerometer_correction_period_;

protected:
	/*
	 * The earth G vector reading while in leveled position.
	 */
	Vector3f earth_g_;

	/*
	 * Current attitude of the world. We assume that
	 * the world is rotating in our coordinate system.
	 */
	QuaternionF attitude_;

	/*
	 * Angular velocity integral
	 */
	Vector3f integral_w_;

};

#endif
