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
	gyr_.open();
	acc_.open(imu_sensor::o_nonblock);
	mag_.open(imu_sensor::o_nonblock);
}

sampler::~sampler()
{
}

void sampler::update()
{
	bar_.read(data.bar1d_);
	acc_.read_scaled_average(data.acc3d_.at(0, 0), data.acc3d_.at(1, 0), data.acc3d_.at(2, 0));
	mag_.read_scaled_average(data.mag3d_.at(0, 0), data.mag3d_.at(1, 0), data.mag3d_.at(2, 0));
	gyr_.read_scaled_average(data.gyr3d_.at(0, 0), data.gyr3d_.at(1, 0), data.gyr3d_.at(2, 0));
	update_time();
}

void sampler::init()
{
	update();
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

