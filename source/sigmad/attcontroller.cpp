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
