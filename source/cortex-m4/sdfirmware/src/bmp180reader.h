/*
 * bmp180reader.h
 *
 *  Created on: Jul 27, 2015
 *      Author: svassilev
 */

#ifndef BMP180READER_H_
#define BMP180READER_H_

#include "units.h"
#include "firfilt.h"
#include "bmp180.h"

struct Bmp180Reader {
public:
	Bmp180Reader(BMP180& bmp);
	~Bmp180Reader();

	Distance altitude_meters(bool read_sensor=false);
	float pressure_hpa(bool read_sensor=false);
	float temperature_celsius(bool read_sensor=false);
	void read_pressure();
	void read_temperature();
	void calibrate();
	static Distance convert_hpa_to_altitude(float hpa);

private:
	static const size_t fir_filter_order = 25;
	typedef FirFilter<float, fir_filter_order, 1> LpfFilter;

	LpfFilter pressure_filter_;
	LpfFilter temperature_filter_;
	BMP180& bmp_;
};

#endif /* BMP180READER_H_ */
