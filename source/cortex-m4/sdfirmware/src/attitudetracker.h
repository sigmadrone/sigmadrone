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
#include "lowpassfilter.h"

class attitudetracker
{
public:
	explicit attitudetracker(double accelerometer_correction_speed = 1.5, Vector3d earth_g = Vector3d(0, 0, -1));
	~attitudetracker();
	Vector3d get_earth_g() const;
	void set_earth_g(Vector3d earth_g);
	Vector3d get_filtered_earth_g() const;
	Vector3d get_earth_m() const;
	void set_earth_m(Vector3d earth_m);
	void track_gyroscope(const Vector3d& omega, double dtime);
	void track_accelerometer(const Vector3d& g, double dtime);
	void track_magnetometer(const Vector3d& m, double dtime);
	void accelerometer_correction_speed(double accelerometer_correction_speed);
	void gyro_drift_pid(double kp, double ki, double kd);
	void gyro_drift_leak_rate(double leak_rate);

	Vector3d get_drift_error() const;
	/*
	 * Return our attitude in the world coordinate system
	 */
	QuaternionD get_attitude() const;

	/*
	 * Return the world attitude in our coordinate system
	 */
	QuaternionD get_world_attitude() const;
	void reset_attitude();

	/*
	 * Return the filtered calculated correction speed
	 * aligning the estimated attitude to the one
	 * detected by the accelerometer sensor
	 */
	Vector3d get_alignment_speed() const;

public:
	/*
	 * The earth M vector reading while in initial position.
	 */
	Vector3d earth_m_;
	double accelerometer_correction_speed_;

protected:
	/*
	 * The earth G vector reading while in leveled position.
	 */
	Vector3d earth_g_;

	/*
	 *
	 */
	LowPassFilter<Vector3d, double> filtered_earth_g_;

	/*
	 * Current attitude of the world. We assume that
	 * the world is rotating in our coordinate system.
	 */
	QuaternionD attitude_;

	QuaternionD coarse_attitude_;

	/*
	 * Filtered correction speed
	 */
	Vector3d alignment_w_;

	PidController<Vector3d> drift_pid_;
	Vector3d drift_err_;
	double drift_leak_rate_;
};

#endif
