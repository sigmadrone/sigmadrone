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

#ifndef GPSREADER_H_
#define GPSREADER_H_

#include "timestamp.h"
#include "uart.h"
#include "digitalout.h"
#include "ringbuffer.h"

class TinyGPS;

class GPSReader {
public:
	GPSReader();
	~GPSReader();
	void start();
	bool update_state();
	float longitude();
	float lattitude();
	Altitude altitude();
	float /*km/h*/ speed(); // TODO: define speed unit
	// course in last full GPRMC sentence in 100th of a degree
	float course();
	uint32_t satellite_count();

private:
	bool process_read_data();

	static const uint32_t buffer_size_ = 256;

	UART uart_;
	DigitalOut gps_power_;
	TinyGPS* gps_parser_;
	RingBuffer buffer_;
};

#endif /* gpsreader_H_ */
