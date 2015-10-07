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
#ifndef FILE_SAMPLER_H_
#define FILE_SAMPLER_H_

#include <string>
#include <fstream>
#include "matrix.h"

class file_sampler
{
public:
	struct _data {
		Vector3d gyr3d_;
		Vector3d acc3d_;
		Vector3d mag3d_;
		double bar1d_;
		double dtime_;
		double time_;
		_data() : bar1d_(0), dtime_(0), time_(0) {}
	} data;

public:
	file_sampler(const std::string& file_name, bool simulate_dt=false);
	~file_sampler();
	bool update();
private:
	void reset_time();

private:
	std::ifstream file_;
	bool simulate_realtime_;
};

#endif
