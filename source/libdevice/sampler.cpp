#include <time.h>
#include <errno.h>
#include <sstream>
#include <stdexcept>
#include "sampler.h"


sampler::sampler(const std::string& gyrdev, const std::string& accdev, const std::string& magdev, const std::string& bardev)
	: gyr_(gyrdev)
	, acc_(accdev)
	, mag_(magdev, 10)
	, bar_(bardev, 50, 0.01)
{
	if (!gyr_.filename().empty())
		gyr_.open();
	if (!acc_.filename().empty()) {
		acc_.open(gyr_.filename().empty() ? imu_sensor::o_default : imu_sensor::o_nonblock);
	}
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
	if (!bar_.filename().empty()) {
		data.bar1d_upd_ = bar_.read(data.bar1d_);
		if (data.bar1d_upd_)
			update_time_bar();
	}
	if (!acc_.filename().empty()) {
		data.acc3d_upd_ = acc_.read_scaled_average(data.acc3d_.at(0, 0), data.acc3d_.at(1, 0), data.acc3d_.at(2, 0));
		if (data.acc3d_upd_)
			update_time_acc();
	}
	if (!mag_.filename().empty()) {
		data.mag3d_upd_ = mag_.read_scaled_average(data.mag3d_.at(0, 0), data.mag3d_.at(1, 0), data.mag3d_.at(2, 0));
		if (data.mag3d_upd_)
			update_time_mag();
	}
	if (!gyr_.filename().empty()) {
		data.gyr3d_upd_ = gyr_.read_scaled_average(data.gyr3d_.at(0, 0), data.gyr3d_.at(1, 0), data.gyr3d_.at(2, 0));
		if (data.gyr3d_upd_)
			update_time_gyr();
	}
}

void sampler::update()
{
	update_sensors();
}

void sampler::init()
{
	data.dtime_acc_ = data.dtime_gyr_ = data.dtime_mag_ = get_clock_time();
	for (int i = 0; i < 32; i++)
		update();
}

void sampler::update_time_gyr()
{
	double now = get_clock_time();
	data.dtime_gyr_ = now - data.time_gyr_;
	data.time_gyr_ = now;
}

void sampler::update_time_acc()
{
	double now = get_clock_time();
	data.dtime_acc_ = now - data.time_acc_;
	data.time_acc_ = now;
}

void sampler::update_time_bar()
{
	double now = get_clock_time();
	data.dtime_bar_ = now - data.time_bar_;
	data.time_bar_ = now;
}

void sampler::update_time_mag()
{
	double now = get_clock_time();
	data.dtime_mag_ = now - data.time_mag_;
	data.time_mag_ = now;
}

double sampler::get_clock_time()
{
	timespec now;

	errno = 0;
	if (clock_gettime(CLOCK_REALTIME, &now) < 0) {
		std::stringstream code;
		code << errno;
		throw std::runtime_error(std::string("clock_gettime failed: ") + std::string(", error code: ") + code.str());
	}
	return ((double)now.tv_sec) + ((double)now.tv_nsec)/1000000000.0;
}
