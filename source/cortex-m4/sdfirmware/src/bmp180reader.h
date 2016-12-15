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

#ifndef BMP180READER_H_
#define BMP180READER_H_

#include "units.h"
#include "bmp280.h"
#include "sensorsprefilters.h"

struct Bmp180Reader {
public:
	Bmp180Reader(BMP280& bmp);
	~Bmp180Reader();

	Distance altitude_meters(bool read_sensor=false);
	float pressure_hpa(bool read_sensor=false);
	float temperature_celsius(bool read_sensor=false);
	void read_pressure();
	void read_temperature();
	void calibrate();
	static Distance convert_hpa_to_altitude(float hpa, float base_pressure, float temp);

	PressurePreFilter pressure_filter_;
	TemperaturePreFilter temperature_filter_;
private:
	BMP280& bmp_;
	float base_pressure_;
};

#endif /* BMP180READER_H_ */
