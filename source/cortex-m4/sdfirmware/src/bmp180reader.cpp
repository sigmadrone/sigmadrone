/*
 * bmp180reader.cpp
 *
 *  Created on: Jul 27, 2015
 *      Author: svassilev
 */

#include "bmp180reader.h"
#include "bmp180.h"
#include "FreeRTOS.h"
#include "task.h"

Bmp180Reader* Bmp180Reader::instance_ = 0;


Bmp180Reader::Bmp180Reader() {
	assert(instance_ == 0);

	static float filt_coeff[fir_filter_order];
	static const float single_coeff = 1.0f/(float)fir_filter_order;

	for (size_t i = 0; i < fir_filter_order; ++i) {
		filt_coeff[i] = single_coeff;
	}

	pressure_filter_.init_coeff(filt_coeff);
	temperature_filter_.init_coeff(filt_coeff);

	bmp180_init();

	instance_ = this;
}

Bmp180Reader::~Bmp180Reader() {
	assert(instance_ == this);
	instance_ = 0;
}

float Bmp180Reader::altitude_meters(bool do_read_sensor) {
	float hpa = pressure_hpa(do_read_sensor);
	return convert_hpa_to_altitude(hpa);
}

float Bmp180Reader::pressure_hpa(bool do_read_sensor) {
	if (do_read_sensor) {
		read_pressure();
	}
	return *instance()->pressure_filter_.get_output();
}

float Bmp180Reader::temperature_celsius(bool do_read_sensor) {
	if (do_read_sensor) {
		read_temperature();
	}
	return *instance()->temperature_filter_.get_output();
}

float Bmp180Reader::convert_hpa_to_altitude(float hpa) {
	return (1.0f - powf(hpa/1013.25f,0.19284f)) * 145366.45f;
}

void Bmp180Reader::read_pressure() {
	instance();
	float pressure = (float)bmp180_get_pressure(bmp180_get_uncomp_pressure()) / 100.0f;
	instance()->pressure_filter_.do_filter(&pressure);
}

void Bmp180Reader::read_temperature() {
	instance();
	float temperature = (float)bmp180_get_temperature(bmp180_get_uncomp_temperature()) * 0.1f;
	instance()->temperature_filter_.do_filter(&temperature);
}

void Bmp180Reader::calibrate() {
	for (size_t i = 0; i < fir_filter_order; ++i) {
		read_pressure();
		read_temperature();
		vTaskDelay(50 / portTICK_RATE_MS);
	}
}

void Bmp180Reader::cleanup() {
	if (Bmp180Reader::instance_) {
		delete Bmp180Reader::instance_;
		Bmp180Reader::instance_ = 0;
	}
}

Bmp180Reader* Bmp180Reader::instance() {
	if (instance_ == 0) {
		instance_ = new Bmp180Reader();
	}
	return instance_;
}


