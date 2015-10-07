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
#ifndef FILESENSOR_H_
#define FILESENSOR_H_

#include <pthread.h>

class file_sensor
{
public:
	file_sensor(const std::string& filename, unsigned int rate = 50, double scale = 1.0);
	~file_sensor();
	void open();
	void close();
	bool read(double &value) const;
	std::string filename() const;

protected:
	static void *file_sensor_thread(void*);
	void update_value();
	void update_thread();

protected:
	std::string filename_;
	unsigned int rate_;
	double scale_;
	mutable unsigned int trottle_counter_;
	mutable double cachedvalue_;
	mutable bool updating_;
	bool exit_;
	pthread_t thread_;
	pthread_cond_t cond_;
	pthread_mutex_t mutex_;
};

#endif
