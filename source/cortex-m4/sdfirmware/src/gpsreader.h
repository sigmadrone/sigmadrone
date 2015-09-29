/*
 * gpsreader.h
 *
 *  Created on: Sep 27, 2015
 *      Author: svassilev
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
