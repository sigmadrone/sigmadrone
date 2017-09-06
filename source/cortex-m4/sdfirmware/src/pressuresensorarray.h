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

#pragma once

#include "units.h"
#include "bmp280.h"
#include "sensorsprefilters.h"
#include "ipressuresensor.h"
#include <vector>

struct PressureSensorArray {
public:
	PressureSensorArray(const std::vector<IPressureSensor*>& sensors);
	PressureSensorArray(BMP280& bmp280);
	~PressureSensorArray();

	Distance get_altitude(bool read_sensor=false);
	Pressure get_pressure(bool read_sensor=false);
	Temperature get_temperature(bool read_sensor=false);
	void read_pressure();
	void read_temperature();
	void calibrate();

	PressurePreFilter pressure_filter_;
private:

	static float calc_variance(const std::vector<float>&);
	void calculate_sensor_weights();

	struct PressureSensorInstance
	{
		PressureSensorInstance(IPressureSensor* sensor) : pressure_sensor_(sensor), last_measurement_(0), weight_(0), variance_(0), std_deviation_(0) {}
		IPressureSensor* pressure_sensor_;
		float last_measurement_;
		float weight_;
		float variance_;
		float std_deviation_;
	};

	std::vector<PressureSensorInstance> sensors_;
	Pressure base_pressure_;
	Temperature temperature_;
};
