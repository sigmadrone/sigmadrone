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
{

}

attcontroller::~attcontroller()
{
}

void attcontroller::start()
{
	stop();
	exit_ = false;
	thread_.reset(new boost::thread(boost::bind(&attcontroller::worker, this)));
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
	while (!exit_) {
		try {
			app_.ssampler_->update();
			if (app_.ssampler_->data.gyr3d_upd_)
				app_.attitude_->track_gyroscope(app_.ssampler_->data.gyr3d_, app_.ssampler_->data.dtime_);
			if (app_.ssampler_->data.acc3d_upd_)
				app_.attitude_->track_accelerometer(app_.ssampler_->data.acc3d_);
			if (app_.ssampler_->data.mag3d_upd_)
				app_.attitude_->track_magnetometer(app_.ssampler_->data.mag3d_);
			attQ_ = app_.attitude_->get_attitude();
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
