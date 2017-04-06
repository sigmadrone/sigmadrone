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

#include "bmp280reader.h"
#include "FreeRTOS.h"
#include "task.h"

Bmp280Reader::Bmp280Reader(BMP280& bmp)
	: bmp_(bmp), base_pressure_(0)
{
}

Bmp280Reader::~Bmp280Reader()
{
}

Distance Bmp280Reader::get_altitude(bool do_read_sensor)
{
	return pressure_to_altitude(get_pressure(do_read_sensor), base_pressure_, get_temperature(false));
}

Pressure Bmp280Reader::get_pressure(bool do_read_sensor)
{
	if (do_read_sensor) {
		read_pressure();
	}
	return pressure_filter_.output();
}

Temperature Bmp280Reader::get_temperature(bool do_read_sensor)
{
	if (do_read_sensor) {
		read_temperature();
	}
	return temperature_;
}

void Bmp280Reader::read_pressure()
{
	pressure_filter_.do_filter(Pressure::from_pa(bmp_.get_pressure()));
	read_temperature();
}

void Bmp280Reader::read_temperature()
{
	temperature_ = Temperature::from_celsius(bmp_.get_temperature());
}

void Bmp280Reader::calibrate()
{
	const size_t iterations = 100;
	float filter_alpha = pressure_filter_.alpha();

	pressure_filter_.set_alpha(0.0f);

	for (size_t i = 0; i < 300; ++i) {
		get_pressure(true);
		get_temperature(true);
	}

	base_pressure_ = {0};
	for (size_t i = 0; i < iterations; ++i) {
		base_pressure_ += get_pressure(true);
	}
	base_pressure_ /= iterations;
	pressure_filter_.do_filter(base_pressure_);
	pressure_filter_.set_alpha(filter_alpha);
}
