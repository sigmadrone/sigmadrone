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
#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include <unistd.h>
#include "filesensor.h"

file_sensor::file_sensor(const std::string& filename, unsigned int rate, double scale)
	: filename_(filename)
	, rate_(rate)
	, scale_(scale)
	, trottle_counter_(0)
	, cachedvalue_(0)
	, updating_(false)
	, exit_(false)
{
	if (pthread_cond_init(&cond_, NULL) != 0)
		throw std::runtime_error("pthread_cond_init failed.");
	if (pthread_mutex_init(&mutex_, NULL) != 0)
		throw std::runtime_error("pthread_mutex_init failed.");
}

file_sensor::~file_sensor()
{
	close();
	pthread_cond_destroy(&cond_);
	pthread_mutex_destroy(&mutex_);
}

void file_sensor::close()
{
//	return;
	pthread_mutex_lock(&mutex_);
	if (updating_) {
		exit_ = true;
		pthread_cond_wait(&cond_, &mutex_);
		pthread_mutex_unlock(&mutex_);
		pthread_join(thread_, NULL);
		return;
	}
	pthread_mutex_unlock(&mutex_);
}

void file_sensor::open()
{
//	return;
	if (updating_)
		return;
	std::ifstream file;
	file.open(filename_.c_str());
	if (!file)
		throw std::runtime_error(std::string("file_sensor::open Failed to open device file: ") + filename_);
	file.close();
    if (pthread_create(&thread_, NULL, file_sensor_thread, (void*)this) != 0) {
            updating_ = false;
            throw std::runtime_error("pthread_create failed.");
    }
	updating_ = true;
}

bool file_sensor::read(double &value) const
{
//	value = 0.0;
//	return true;
	if (!updating_)
		throw std::runtime_error("file_sensor::read error: device is not open: " + filename_);
	value = cachedvalue_;
	return true;
}

std::string file_sensor::filename() const
{
	return filename_;
}

void file_sensor::update_value()
{
	std::string strval;
	std::ifstream file;
	file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
	file.open(filename_.c_str());
	file >> strval;
	file.close();
	cachedvalue_ = strtod(strval.c_str(), NULL) * scale_;
}

void file_sensor::update_thread()
{
	while (!exit_) {
		try {
			if (rate_)
				usleep(1000000/rate_);
			update_value();
		} catch (std::exception& e) {

		}
	}
	pthread_mutex_lock(&mutex_);
	updating_ = false;
	pthread_cond_signal(&cond_);
	pthread_mutex_unlock(&mutex_);
}

void* file_sensor::file_sensor_thread(void *pthis)
{
	static_cast<file_sensor*>(pthis)->update_thread();
	return NULL;
}
