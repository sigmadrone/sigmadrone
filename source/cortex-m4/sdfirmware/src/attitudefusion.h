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

class attitudefusion
{
public:
	typedef Vector3f vector_type;
	typedef QuaternionF quaternion_type;

	explicit attitudefusion(double accelerometer_correction_speed = 1.5, vector_type earth_g = vector_type(0, 0, -1));
	~attitudefusion();
	void input(const std::vector<vector_type>& gyr, const std::vector<vector_type>& acc, const std::vector<vector_type>& mag, double dtime);
	vector_type get_earth_g() const;
	void set_earth_g(vector_type earth_g);
	vector_type get_filtered_earth_g() const;
	vector_type get_earth_m() const;
	void set_earth_m(vector_type earth_m);
	void accelerometer_correction_speed(double accelerometer_correction_speed);
	void gyro_drift_pid(double kp, double ki, double kd);
	void gyro_drift_leak_rate(double leak_rate);

	void set_track_gyroscope(bool value) { use_gyroscope_ = value; }
	void set_track_accelerometer(bool value) { use_accelerometer_ = value; }
	void set_track_magnetometer(bool value) { use_magnetometer_ = value; }

	vector_type get_filtered_gyr() { return gyr_lpf_.output(); }
	vector_type get_filtered_acc() { return acc_lpf_.output(); }
	vector_type get_filtered_mag() { return mag_lpf_.output(); }

	vector_type get_drift_error() const;
	/*
	 * Return our attitude in the world coordinate system
	 */
	quaternion_type get_attitude() const;

	/*
	 * Return the world attitude in our coordinate system
	 */
	quaternion_type get_world_attitude() const;
	void reset_attitude();

	/*
	 * Return the filtered calculated correction speed
	 * aligning the estimated attitude to the one
	 * detected by the accelerometer sensor
	 */
	vector_type get_alignment_speed() const;

public:
	void track_gyroscope(const vector_type& omega, double dtime);
	void track_accelerometer(const vector_type& g, double dtime);
	void track_magnetometer(const vector_type& m, double dtime);

public:
	bool use_gyroscope_;
	bool use_accelerometer_;
	bool use_magnetometer_;
	double accelerometer_correction_speed_;

	/*
	 * The earth M vector reading while in initial position.
	 */
	vector_type earth_m_;

protected:
	LowPassFilter<vector_type, float> gyr_lpf_;
	LowPassFilter<vector_type, float> acc_lpf_;
	LowPassFilter<vector_type, float> mag_lpf_;
	LowPassFilter<vector_type, float> gyr_lpf2_;


	/*
	 * The earth G vector reading while in leveled position.
	 */
	vector_type earth_g_;

	/*
	 *
	 */
	LowPassFilter<vector_type, float> filtered_earth_g_;

	/*
	 * Current attitude of the world. We assume that
	 * the world is rotating in our coordinate system.
	 */
	quaternion_type attitude_;

	quaternion_type coarse_attitude_;

	/*
	 * Filtered correction speed
	 */
	vector_type alignment_w_;

	PidController<vector_type> drift_pid_;
	vector_type drift_err_;
	double drift_leak_rate_;
};

#endif
