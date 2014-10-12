/*
 * attcontroller.cpp
 *
 *  Created on: Oct 7, 2014
 *      Author: mstoilov
 */

#include <stdexcept>
#include "attcontroller.h"
#include "serverapp.h"

attcontroller::attcontroller(server_app& app)
	: app_(app)
	, exit_(false)
	, attQ_(QuaternionD::identity)
	, targetQ_(QuaternionD::identity)
	, thrust_(0.0)
{
	Vector3d TrustDir(0, 0, 1);
	M0_ = Vector3d::cross(Vector3d( 1.0, -1.0, 0.0), TrustDir).normalize();
	M1_ = Vector3d::cross(Vector3d( 1.0,  1.0, 0.0), TrustDir).normalize();
	M2_ = Vector3d::cross(Vector3d(-1.0,  1.0, 0.0), TrustDir).normalize();
	M3_ = Vector3d::cross(Vector3d(-1.0, -1.0, 0.0), TrustDir).normalize();
	pid_.reset(1.9, 0.0, 0.35);
}

attcontroller::~attcontroller()
{
	stop();
}

void attcontroller::start()
{
	if (!thread_) {
		exit_ = false;
		thread_.reset(new boost::thread(boost::bind(&attcontroller::worker, this)));
	}
}

void attcontroller::stop()
{
	if (thread_) {
		exit_ = true;
		thread_->join();
		thread_.reset();
	}
}

void attcontroller::worker()
{
	app_.ssampler_->init();
	while (!exit_) {
		try {
			boost::this_thread::sleep(boost::posix_time::milliseconds(2));
			app_.ssampler_->update();
			if (app_.ssampler_->data.gyr3d_upd_)
				app_.attitude_tracker_->track_gyroscope(DEG2RAD(app_.ssampler_->data.gyr3d_), app_.ssampler_->data.dtime_);
			if (app_.ssampler_->data.acc3d_upd_)
				app_.attitude_tracker_->track_accelerometer(app_.ssampler_->data.acc3d_);
			if (app_.ssampler_->data.mag3d_upd_)
				app_.attitude_tracker_->track_magnetometer(app_.ssampler_->data.mag3d_);
			attQ_ = app_.attitude_tracker_->get_attitude();

			pid_.set_target(targetQ_);
			Vector3d correction = pid_.get_torque(attQ_, DEG2RAD(app_.ssampler_->data.gyr3d_), app_.ssampler_->data.dtime_);

			//  From the motor trust measurement:
			//  0.6 --> 450g * 22.5cm
			//  0.6 --> (450/1000) * (22.5/100)
			//  0.6 --> 0.10125 kg.m
			Vector3d torqueRPM = correction * 0.6 / (101.25/1000.0) / 2.0;

			if (thrust_ > 0.0) {
				app_.servoctrl_->motor(0).offset(thrust_ + Vector3d::dot(torqueRPM, M0_));
				app_.servoctrl_->motor(1).offset(thrust_ + Vector3d::dot(torqueRPM, M1_));
				app_.servoctrl_->motor(2).offset(thrust_ + Vector3d::dot(torqueRPM, M2_));
				app_.servoctrl_->motor(3).offset(thrust_ + Vector3d::dot(torqueRPM, M3_));
			} else {
				app_.servoctrl_->motor(0).reset();
				app_.servoctrl_->motor(1).reset();
				app_.servoctrl_->motor(2).reset();
				app_.servoctrl_->motor(3).reset();
			}
			app_.servoctrl_->update();
		} catch (std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}
	}
	exit_ = false;
}

QuaternionD attcontroller::get_attitude() const
{
	return attQ_;
}
