/*
 * bmp180reader.cpp
 *
 *  Created on: Jul 27, 2015
 *      Author: svassilev
 */

#include "bmp180reader.h"
#include "FreeRTOS.h"
#include "task.h"

Bmp180Reader::Bmp180Reader(BMP180& bmp)
	: bmp_(bmp)
{
	static float filt_coeff[fir_filter_order];
	static const float single_coeff = 1.0f/(float)fir_filter_order;

	for (size_t i = 0; i < fir_filter_order; ++i) {
		filt_coeff[i] = single_coeff;
	}

	pressure_filter_.init_coeff(filt_coeff);
	temperature_filter_.init_coeff(filt_coeff);

}

Bmp180Reader::~Bmp180Reader() {

}

Distance Bmp180Reader::altitude_meters(bool do_read_sensor) {
	float hpa = pressure_hpa(do_read_sensor);
	return convert_hpa_to_altitude(hpa);
}

float Bmp180Reader::pressure_hpa(bool do_read_sensor) {
	if (do_read_sensor) {
		read_pressure();
	}
	return *pressure_filter_.get_output();
}

float Bmp180Reader::temperature_celsius(bool do_read_sensor) {
	if (do_read_sensor) {
		read_temperature();
	}
	return *temperature_filter_.get_output();
}

Distance Bmp180Reader::convert_hpa_to_altitude(float hpa) {
	return Distance::from_feet((1.0f - powf(hpa/1013.25f,0.19284f)) * 145366.45f);
}

void Bmp180Reader::read_pressure() {
	try {
		bmp_.update_pressure();
		float pressure = (float)bmp_.get_pressure() / 100.0f;
		pressure_filter_.do_filter(&pressure);
	} catch (std::exception& e) {

	}
}

void Bmp180Reader::read_temperature() {
	try {
		bmp_.update_temperature();
		float temperature = (float)bmp_.get_temperature();
		temperature_filter_.do_filter(&temperature);
	} catch (std::exception& e) {

	}
}

void Bmp180Reader::calibrate() {
	for (size_t i = 0; i < fir_filter_order; ++i) {
		read_pressure();
		read_temperature();
		vTaskDelay(50 / portTICK_RATE_MS);
	}
}

void Bmp180Reader::cleanup() {

}

