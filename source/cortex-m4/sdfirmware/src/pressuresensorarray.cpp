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

#include "pressuresensorarray.h"
#include "FreeRTOS.h"
#include "task.h"

PressureSensorArray::PressureSensorArray(const std::vector<IPressureSensor*>& sensors)
{
	for (auto ps: sensors) {
		sensors_.push_back(PressureSensorInstance(ps));
	}
}

PressureSensorArray::~PressureSensorArray()
{
}

Distance PressureSensorArray::get_altitude(bool do_read_sensor)
{
	return pressure_to_altitude(get_pressure(do_read_sensor), base_pressure_, get_temperature(false));
}

Pressure PressureSensorArray::get_pressure(bool do_read_sensor)
{
	if (do_read_sensor) {
		read_pressure();
	}
	return pressure_;
}

Temperature PressureSensorArray::get_temperature(bool do_read_sensor)
{
	if (do_read_sensor) {
		read_temperature();
	}
	return temperature_;
}

void PressureSensorArray::read_pressure()
{
	Pressure pressure;
	for (auto&& it: sensors_) {
		if (it.pressure_sensor_->is_fifo_supported()) {
			while (!it.pressure_sensor_->is_fifo_empty()) {
				it.pressure_filter_.do_filter(Pressure::from_hpa(it.pressure_sensor_->read_pressure_hpa()));
			}
		} else {
			it.pressure_filter_.do_filter(Pressure::from_hpa(it.pressure_sensor_->read_pressure_hpa()));
		}
		pressure +=  it.pressure_filter_.output() * it.weight_;
	}
	pressure_ = pressure;
	read_temperature();
}

void PressureSensorArray::read_temperature()
{
	Temperature t;
	for (auto it: sensors_) {
		t = t + Temperature::from_celsius(it.pressure_sensor_->read_temperature_celsius());
	}
	temperature_ = t / sensors_.size();
}

float PressureSensorArray::calc_variance(const std::vector<float>& samples)
{
	float mean = 0;
	assert(samples.size() > 1);
	for (auto it: samples) {
		mean += it;
	}
	mean /= samples.size();
	float var = 0;
	for (auto it: samples) {
		var += (it - mean) * (it - mean);
	}
	return var;
}

void PressureSensorArray::calculate_sensor_weights()
{
	float inv_variance_sum = 0.0f;
	for (size_t i = 0; i < sensors_.size(); ++i) {
		inv_variance_sum += 1.0f / sensors_[i].variance_;
	}
	for (auto&& it: sensors_) {
		it.weight_ = 1.0f / (it.variance_ * inv_variance_sum);
	}
}

void PressureSensorArray::calibrate()
{
	const size_t iterations = 100;

	std::vector<std::vector<float>> samples(sensors_.size());

	for (auto&& it: sensors_) {
		it.pressure_filter_.reset();
	}

	for (size_t i = 0; i < iterations; ++i) {
		for (size_t j = 0; j < sensors_.size(); ++j) {
			if (sensors_[j].pressure_sensor_->is_fifo_supported()) {
				while (sensors_[j].pressure_sensor_->is_fifo_empty()) {
					vTaskDelay(5);
				}
			} else {
				if (i > 0) {
					while (samples[j][i-1] == sensors_[j].pressure_sensor_->read_pressure_hpa()) {
						vTaskDelay(5);
					}
				}
			}
			float hpa = sensors_[j].pressure_sensor_->read_pressure_hpa();
			samples[j].push_back(hpa);
		}
	}

	for (size_t i = 0; i < sensors_.size(); ++i) {
		sensors_[i].variance_ = calc_variance(samples[i]);
		sensors_[i].std_deviation_ = std::sqrt(sensors_[i].variance_/(iterations-1));
	}

	calculate_sensor_weights();

	for (size_t i = 0; i < iterations; ++i) {
		for (auto&& it: sensors_) {
			while (it.pressure_sensor_->is_fifo_supported() && it.pressure_sensor_->is_fifo_empty()) {
				vTaskDelay(1);
			}
		}
		get_pressure(true);
	}
	base_pressure_ = get_pressure(false);
}

void PressureSensorArray::set_lpf_coefficient(float alpha)
{
	for (auto&& it: sensors_) {
		it.pressure_filter_.set_alpha(alpha);
	}
}
