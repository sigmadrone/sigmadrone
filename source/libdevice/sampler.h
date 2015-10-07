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
#ifndef SAMPLER_H_
#define SAMPLER_H_

#include <string>
#include "biasimusensor.h"
#include "imusensor.h"
#include "filesensor.h"
#include "matrix.h"

class sampler
{
public:
	biasimu_sensor gyr_;
	imu_sensor acc_;
	imu_sensor mag_;
	file_sensor bar_;

	struct data {
		Vector3d gyr3d_;
		Vector3d acc3d_;
		Vector3d mag3d_;
		double bar1d_;
		double dtime_gyr_;
		double time_gyr_;
		double dtime_acc_;
		double time_acc_;
		double dtime_mag_;
		double time_mag_;
		double dtime_bar_;
		double time_bar_;
		bool gyr3d_upd_;
		bool acc3d_upd_;
		bool mag3d_upd_;
		bool bar1d_upd_;
		data()
		: bar1d_(0)
		, dtime_gyr_(0)
		, time_gyr_(0)
		, dtime_acc_(0)
		, time_acc_(0)
		, dtime_mag_(0)
		, time_mag_(0)
		, dtime_bar_(0)
		, time_bar_(0)
		, gyr3d_upd_(false)
		, acc3d_upd_(false)
		, mag3d_upd_(false)
		, bar1d_upd_(false) {}
	} data;

public:
	sampler(const std::string& gyrdev = "", const std::string& accdev = "", const std::string& magdev = "", const std::string& bardev = "");
	~sampler();
	void init();
	void update();

protected:
	void update_sensors();
	void update_time_gyr();
	void update_time_acc();
	void update_time_bar();
	void update_time_mag();
	double get_clock_time();
};

#endif
