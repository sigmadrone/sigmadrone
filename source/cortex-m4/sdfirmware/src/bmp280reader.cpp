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

Distance Bmp280Reader::altitude_meters(bool do_read_sensor)
{
	float hpa = pressure_hpa(do_read_sensor);
	return convert_hpa_to_altitude(hpa, base_pressure_, temperature_celsius(false));
}

float Bmp280Reader::pressure_hpa(bool do_read_sensor)
{
	if (do_read_sensor) {
		read_pressure();
	}
	return pressure_filter_.output();
}

float Bmp280Reader::temperature_celsius(bool do_read_sensor)
{
	if (do_read_sensor) {
		read_temperature();
	}
	return temperature_filter_.output();
}

Distance Bmp280Reader::convert_hpa_to_altitude(float hpa, float base_pressure, float temperature)
{
	return Distance::from_meters((powf(base_pressure/hpa,0.1902f) - 1.0f) * (temperature + 273.15f)/0.0065);
}

void Bmp280Reader::read_pressure()
{
	float pressure = (float)bmp_.get_pressure() / 100.0f;
	pressure_filter_.do_filter(pressure);
	temperature_filter_.do_filter(bmp_.get_temperature());
}

void Bmp280Reader::read_temperature()
{
	float temperature = (float)bmp_.get_temperature();
	temperature_filter_.do_filter(temperature);
}

void Bmp280Reader::calibrate()
{
	const size_t iterations = 100;
	float filter_alpha = pressure_filter_.alpha();
	pressure_filter_.set_alpha(0.0f);
	for (size_t i = 0; i < 300; ++i) {
		pressure_hpa(true);
	}

	base_pressure_ = 0.0f;
	for (size_t i = 0; i < iterations; ++i) {
		base_pressure_ += pressure_hpa(true);
	}
	base_pressure_ /= iterations;
	pressure_filter_.do_filter(base_pressure_);

	pressure_filter_.set_alpha(filter_alpha);

	filter_alpha = temperature_filter_.alpha();
	temperature_filter_.set_alpha(0);
	temperature_celsius(true);
	temperature_filter_.set_alpha(filter_alpha);
}
