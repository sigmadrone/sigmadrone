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
#ifndef ATTITUDETRACKER_H_
#define ATTITUDETRACKER_H_

#include "d3math.h"
#include "pidcontroller.h"

class attitudetracker
{
public:
	explicit attitudetracker(double accelerometer_correction_period = 1.5, Vector3f earth_g = Vector3f(0, 0, -1));
	~attitudetracker();
	Vector3f get_earth_g() const;
	void set_earth_g(Vector3f earth_g);
	Vector3f get_earth_m() const;
	void set_earth_m(Vector3f earth_m);
	void track_gyroscope(const Vector3f& omega, double dtime);
	void track_accelerometer(const Vector3f& g, double dtime);
	void track_magnetometer(const Vector3f& m, double dtime);
	void accelerometer_correction_speed(double accelerometer_correction_speed);
	void gyro_drift_pid(float kp, float ki, float kd);

	Vector3f get_drift_error() const;
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
	double accelerometer_correction_speed_;

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

	PidController<Vector3f> drift_pid_;
	Vector3f drift_err_;
};

#endif
