#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include "filesampler.h"

file_sampler::file_sampler(const std::string& filename, bool simulate_realtime) :
		simulate_realtime_(simulate_realtime)
{
	file_.exceptions( std::ifstream::failbit | std::ifstream::badbit );
	file_.open(filename.c_str());
	reset_time();
}

file_sampler::~file_sampler()
{
}

bool file_sampler::update()
{
	std::string line;
	try {
		std::getline(file_, line);
	} catch (std::exception& e) {
		std::cout << "file_sampler error: " << e.what() << std::endl;
		return false;
	}
	sscanf(line.c_str(),"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
			&(data.acc3d_.at(0,0)), &(data.acc3d_.at(1,0)), &(data.acc3d_.at(2,0)),
			&(data.gyr3d_.at(0,0)), &(data.gyr3d_.at(1,0)), &(data.gyr3d_.at(2,0)),
			&(data.mag3d_.at(0,0)), &(data.mag3d_.at(1,0)), &(data.mag3d_.at(2,0)),
			&data.bar1d_,
			&data.dtime_);
	if (simulate_realtime_) {
		timespec ts;
		ts.tv_sec = (time_t)data.dtime_;
		ts.tv_nsec = (long)((data.dtime_-(double)ts.tv_sec)*1000000000.0);
		nanosleep(&ts,0);
	}
	data.time_ += data.dtime_;
	return true;
}

void file_sampler::reset_time()
{
	timespec now;
	errno = 0;
	if (clock_gettime(CLOCK_REALTIME, &now) < 0) {
		std::stringstream code;
		code << errno;
		throw std::runtime_error(std::string("clock_gettime failed: ") + std::string(", error code: ") + code.str());
	}
	data.time_ = ((double)now.tv_sec) + ((double)now.tv_nsec)/1000000000.0;
}

