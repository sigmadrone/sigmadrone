#include <time.h>
#include <errno.h>
#include <sstream>
#include <stdexcept>
#include "sampler.h"


sampler::sampler(const std::string& gyrdev, const std::string& accdev, const std::string& magdev, const std::string& bardev)
	: gyr_(gyrdev)
	, acc_(accdev)
	, mag_(magdev, 10)
	, bar_(bardev, 10, 0.01)
{
	if (!gyr_.filename().empty())
		gyr_.open();
	if (!acc_.filename().empty())
		acc_.open(imu_sensor::o_nonblock);
	if (!mag_.filename().empty())
		mag_.open(imu_sensor::o_nonblock);
	if (!bar_.filename().empty())
		bar_.open();
}

sampler::~sampler()
{
}

void sampler::update_sensors()
{
	if (!bar_.filename().empty())
		data.bar1d_upd_ = bar_.read(data.bar1d_);
	if (!acc_.filename().empty())
		data.acc3d_upd_ = acc_.read_scaled_average(data.acc3d_.at(0, 0), data.acc3d_.at(1, 0), data.acc3d_.at(2, 0));
	if (!mag_.filename().empty())
		data.mag3d_upd_ = mag_.read_scaled_average(data.mag3d_.at(0, 0), data.mag3d_.at(1, 0), data.mag3d_.at(2, 0));
	if (!gyr_.filename().empty())
		data.gyr3d_upd_ = gyr_.read_scaled_average(data.gyr3d_.at(0, 0), data.gyr3d_.at(1, 0), data.gyr3d_.at(2, 0));
}

void sampler::update()
{
	update_sensors();
	update_time();
}

void sampler::init()
{
	update_time();
}

void sampler::update_time()
{
	timespec now;

	errno = 0;
	if (clock_gettime(CLOCK_REALTIME, &now) < 0) {
		std::stringstream code;
		code << errno;
		throw std::runtime_error(std::string("clock_gettime failed: ") + std::string(", error code: ") + code.str());
	}
	data.dtime_ = ((double)now.tv_sec) + ((double)now.tv_nsec)/1000000000.0 - data.time_;
	data.time_ = ((double)now.tv_sec) + ((double)now.tv_nsec)/1000000000.0;
}
