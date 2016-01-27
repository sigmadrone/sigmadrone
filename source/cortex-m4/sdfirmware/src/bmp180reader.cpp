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

#include "bmp180reader.h"
#include "FreeRTOS.h"
#include "task.h"

Bmp180Reader::Bmp180Reader(BMP180& bmp)
	: bmp_(bmp)
{
}

Bmp180Reader::~Bmp180Reader()
{
}

Distance Bmp180Reader::altitude_meters(bool do_read_sensor)
{
	float hpa = pressure_hpa(do_read_sensor);
	return convert_hpa_to_altitude(hpa);
}

float Bmp180Reader::pressure_hpa(bool do_read_sensor)
{
	if (do_read_sensor) {
		read_pressure();
	}
	return pressure_filter_.output();
}

float Bmp180Reader::temperature_celsius(bool do_read_sensor)
{
	if (do_read_sensor) {
		read_temperature();
	}
	return temperature_filter_.output();
}

Distance Bmp180Reader::convert_hpa_to_altitude(float hpa)
{
	return Distance::from_feet((1.0f - powf(hpa/1013.25f,0.19284f)) * 145366.45f);
}

void Bmp180Reader::read_pressure()
{
	bmp_.update_pressure();
	float pressure = (float)bmp_.get_pressure() / 100.0f;
	pressure_filter_.do_filter(pressure);
}

void Bmp180Reader::read_temperature()
{
	bmp_.update_temperature();
	float temperature = (float)bmp_.get_temperature();
	temperature_filter_.do_filter(temperature);
}

void Bmp180Reader::calibrate()
{
	for (size_t i = 0; i < PRESSURE_FILTER_ORDER; ++i) {
		read_pressure();
		read_temperature();
		vTaskDelay(50 / portTICK_RATE_MS);
	}
}
