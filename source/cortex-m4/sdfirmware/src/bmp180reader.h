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

struct Bmp180Reader {
public:
	static float altitude_meters(bool read_sensor=false);
	static float pressure_hpa(bool read_sensor=false);
	static float temperature_celsius(bool read_sensor=false);
	static void read_pressure();
	static void read_temperature();
	static void calibrate();
	static void cleanup();

	static float convert_hpa_to_altitude(float hpa);

private:
	Bmp180Reader();
	~Bmp180Reader();
	static Bmp180Reader* instance();

	static Bmp180Reader* instance_;

	static const size_t fir_filter_order = 25;
	typedef FirFilter<float, fir_filter_order, 1> LpfFilter;

	LpfFilter pressure_filter_;
	LpfFilter temperature_filter_;
};

#endif /* BMP180READER_H_ */
