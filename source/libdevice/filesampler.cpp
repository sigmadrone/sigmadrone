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

