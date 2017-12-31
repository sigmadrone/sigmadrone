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
#include "attitudefusion.h"

#include <iostream>

attitudefusion::attitudefusion(double accelerometer_correction_speed, attitudefusion::vector_type earth_g)
	: use_gyroscope_(true)
	, use_accelerometer_(true)
	, use_magnetometer_(true)
	, accelerometer_correction_speed_(accelerometer_correction_speed)
	, gyr_lpf_{0.25}
	, acc_lpf_{0.9995}
	, mag_lpf_{0.90}
	, gyr_lpf2_{0.5}
	, earth_g_(earth_g)
	, filtered_earth_g_(0.85)
	, attitude_(quaternion_type::identity)
	, coarse_attitude_(quaternion_type::identity)
	, drift_pid_(0, 0.01, 0)
    , drift_leak_rate_(0.00001)
{
	filtered_earth_g_.reset(earth_g);
}

void attitudefusion::input(const std::vector<vector_type>& gyr, const std::vector<vector_type>& acc, const std::vector<vector_type>& mag, double dtime)
{
	for (auto x : gyr) {
		gyr_lpf_.do_filter(DEG2RAD(x));
		gyr_lpf2_.do_filter(DEG2RAD(x));
	}

	for (auto x : acc) {
		acc_lpf_.do_filter(x);
	}

	for (auto x : mag) {
		mag_lpf_.do_filter(x);
	}

	quaternion_type deltaq = quaternion_type::fromAngularVelocity(gyr_lpf2_.output() - drift_err_, dtime).normalize();
	Vector3f filtered_acc = acc_lpf_.output();
	acc_lpf_.reset(deltaq.conjugate().rotate(filtered_acc));

	if (use_gyroscope_) {
		track_gyroscope(gyr_lpf_.output(), dtime);
	}
	if (use_accelerometer_) {
		track_accelerometer(acc_lpf_.output().normalize(), dtime);
	}
	if (use_magnetometer_) {
		track_magnetometer(mag_lpf_.output().normalize(), dtime);
	}
}

attitudefusion::~attitudefusion()
{
}

attitudefusion::vector_type attitudefusion::get_earth_g() const
{
	return earth_g_;
}

attitudefusion::vector_type attitudefusion::get_filtered_earth_g() const
{
	return filtered_earth_g_.output();
}

attitudefusion::vector_type attitudefusion::get_drift_error() const
{
	return drift_err_;
}

void attitudefusion::set_earth_g(attitudefusion::vector_type earth_g)
{
	earth_g_ = earth_g;
}

attitudefusion::vector_type attitudefusion::get_earth_m() const
{
	return earth_m_;
}

void attitudefusion::set_earth_m(attitudefusion::vector_type earth_m)
{
	earth_m_ = earth_m;
}

void attitudefusion::accelerometer_correction_speed(double accelerometer_correction_speed)
{
	accelerometer_correction_speed_ = accelerometer_correction_speed;
}

void attitudefusion::gyro_drift_pid(double kp, double ki, double kd)
{
	drift_pid_.set_kp_ki_kd(kp, ki, kd);
}

void attitudefusion::gyro_drift_leak_rate(double leak_rate)
{
	drift_leak_rate_ = leak_rate;
}

void attitudefusion::track_gyroscope(const attitudefusion::vector_type& omega, double dtime)
{
	quaternion_type deltaq = quaternion_type::fromAngularVelocity(omega - drift_err_, dtime).normalize();
	attitude_ = (attitude_ * deltaq).normalize();
}

void attitudefusion::track_accelerometer(const attitudefusion::vector_type& g, double dtime)
{
	/*
	 * Estimate after rotating the initial earth acceleration
	 * vector with the world attitude quaternion.
	 */
	attitudefusion::vector_type g_estimated = get_world_attitude().rotate(earth_g_);

	/*
	 * Calculate the rotation between the estimated earth acceleration
	 * vector and the one detected by the accelerometer sensor.
	 */
	quaternion_type q = quaternion_type::fromVectors(g_estimated, g);

	/*
	 * Generate angular velocity to adjust our attitude in the
	 * direction of the sensor reading. We assume that our
	 * attitude quaternion has accumulated error, so we will
	 * try to correct it by calculating a delta quaternion that
	 * when adjust our attitude with it will better align
	 * the estimated earth acceleration with one detected by
	 * the sensor.
	 */
//	alignment_w_ = quaternion_type::angularVelocity(quaternion_type::identity, q, q.angle() / DEG2RAD(accelerometer_correction_speed_));
	alignment_w_ = quaternion_type::angularVelocity(quaternion_type::identity, q, accelerometer_correction_speed_);
	drift_err_ = drift_pid_.get_pid(alignment_w_, dtime, drift_leak_rate_);
	quaternion_type deltaq = quaternion_type::fromAngularVelocity(-alignment_w_, dtime);
	attitude_ = (attitude_ * deltaq).normalize();

	filtered_earth_g_.do_filter(attitude_.rotate(g));
}

void attitudefusion::track_magnetometer(const attitudefusion::vector_type& m, double dtime)
{
	if (fabs(attitude_.x) > 0.04 || fabs(attitude_.y) > 0.04) {
		/*
		 * Ignore magnetometer reading if the system is sufficiently tilted
		 * from the XY plane
		 */
		return;
	}

	/*
	 * Ignore the Z component from the reading
	 */
	attitudefusion::vector_type mag_xy = attitudefusion::vector_type(m.at(0), m.at(1), 0.0).normalize();

	/*
	 * Estimate the mag_xy using the earth quaternion. Note: here we assume that
	 * when the aircraft is pointed to the magnetic north, the reading normalized
	 * in the XY plane will be [1,0,0].
	 */
	attitudefusion::vector_type mag_xy_estimated = get_world_attitude().rotate(attitudefusion::vector_type(1,0,0));

	/*
	 * Nullify the Z component and normalize
	 */
	mag_xy_estimated.at(2) = 0.0;
	mag_xy_estimated = mag_xy_estimated.normalize();

	/*
	 * Calculate the rotation between the estimated vector
	 * and the one received by the sensor.
	 */
	quaternion_type q = quaternion_type::fromVectors(mag_xy_estimated, mag_xy);

	/*
	 * Generate angular velocity to adjust our attitude in the
	 * direction of the sensor reading.
	 */
	attitudefusion::vector_type w = quaternion_type::angularVelocity(quaternion_type::identity, q, 10);
	if (w.length() != 0.0) {
		quaternion_type deltaq = quaternion_type::fromAngularVelocity(-w, dtime);
		attitude_ = (attitude_ * deltaq).normalize();
	}
}

attitudefusion::quaternion_type attitudefusion::get_attitude() const
{
	return attitude_;
}

attitudefusion::quaternion_type attitudefusion::get_world_attitude() const
{
	return attitude_.conjugate();
}


void attitudefusion::reset_attitude()
{
	attitude_ = quaternion_type::identity;
}

attitudefusion::vector_type attitudefusion::get_alignment_speed() const
{
	return alignment_w_;
}
