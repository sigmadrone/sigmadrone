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

#ifndef BMP280READER_H_
#define BMP280READER_H_

#include "units.h"
#include "bmp280.h"
#include "sensorsprefilters.h"

struct Bmp280Reader {
public:
	Bmp280Reader(BMP280& bmp);
	~Bmp280Reader();

	Distance get_altitude(bool read_sensor=false);
	Pressure get_pressure(bool read_sensor=false);
	Temperature get_temperature(bool read_sensor=false);
	void read_pressure();
	void read_temperature();
	void calibrate();

	PressurePreFilter pressure_filter_;
	TemperaturePreFilter temperature_filter_;
private:
	BMP280& bmp_;
	Pressure base_pressure_;
};

#endif /* BMP280READER_H_ */
