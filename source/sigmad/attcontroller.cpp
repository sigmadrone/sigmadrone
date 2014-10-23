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
	P0_ = Vector3d( 1.0, -1.0, 0.0);
	P1_ = Vector3d( 1.0,  1.0, 0.0);
	P2_ = Vector3d(-1.0,  1.0, 0.0);
	P3_ = Vector3d(-1.0, -1.0, 0.0);
	set_thrust_dir(Vector3d(0, 0, 1));
	pid_.reset(1.2, 0.0, 0.40);
}

attcontroller::~attcontroller()
{
	stop();
}

void attcontroller::set_thrust_dir(const Vector3d& thrustdir)
{
	thrustdir_ = thrustdir.normalize();
	M0_ = Vector3d::cross(P0_, thrustdir_).normalize();
	M1_ = Vector3d::cross(P1_, thrustdir_).normalize();
	M2_ = Vector3d::cross(P2_, thrustdir_).normalize();
	M3_ = Vector3d::cross(P3_, thrustdir_).normalize();
}

void attcontroller::set_correction_thrust(const Vector4d& ct)
{
	ct_ = ct;
}

Vector4d attcontroller::get_correction_thrust()
{
	return ct_;
}

void attcontroller::start()
{
	if (!thread_) {
		thrust_ = 0.0;
		app_.servoctrl_->armmotors();
		app_.servoctrl_->enable();
		exit_ = false;
		app_.attitude_tracker_->reset_attitude();
		thread_.reset(new boost::thread(boost::bind(&attcontroller::worker, this)));
	}
}

void attcontroller::stop()
{
	if (thread_) {
		app_.servoctrl_->reset();
		app_.servoctrl_->disable();
		exit_ = true;
		thread_->join();
		thread_.reset();
	}
}

bool attcontroller::is_running()
{
	return (thread_) ? true : false;
}

void attcontroller::worker()
{
	app_.ssampler_->init();
	/*
	 * Initialize the initial Magnetometer position.
	 */
	for (int i = 0; i < 20; i++) {
		app_.ssampler_->update();
		if (app_.ssampler_->data.mag3d_upd_) {
			app_.attitude_tracker_->set_earth_m(app_.ssampler_->data.mag3d_);
		}
	}
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
				app_.servoctrl_->motor(0).offset_clamp(thrust_ + ct_.at(0) + Vector3d::dot(torqueRPM, M0_), 0.05, 1.0);
				app_.servoctrl_->motor(1).offset_clamp(thrust_ + ct_.at(1) + Vector3d::dot(torqueRPM, M1_), 0.05, 1.0);
				app_.servoctrl_->motor(2).offset_clamp(thrust_ + ct_.at(2) + Vector3d::dot(torqueRPM, M2_), 0.05, 1.0);
				app_.servoctrl_->motor(3).offset_clamp(thrust_ + ct_.at(3) + Vector3d::dot(torqueRPM, M3_), 0.05, 1.0);
			} else {
				app_.servoctrl_->motor(0).offset(0.0);
				app_.servoctrl_->motor(1).offset(0.0);
				app_.servoctrl_->motor(2).offset(0.0);
				app_.servoctrl_->motor(3).offset(0.0);
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
