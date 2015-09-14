/*
 * battery.cpp
 *
 *  Created on: Sep 13, 2015
 *      Author: svassilev
 */

#include "battery.h"

Battery::Battery(Battery::Type type) :
	voltage_(0.0),
	type_(type)
{
}

Battery::~Battery() {}

void Battery::update(ADCHandle& adc)
{
	voltage_ = adc.read_value_as_voltage() * (r1_div_+r2_div_) / r2_div_;
	auto_detect_type();
}

float Battery::voltage() const
{
	return voltage_;
}

float Battery::max_voltage() const
{
	return type() * lipo_1cell_max_v_;
}

float Battery::min_voltage() const
{
	return type() * lipo_1cell_min_v_;
}

float Battery::nominal_voltage() const
{
	return type() * lipo_1cell_nominal_v_;
}

float Battery::charge_percentage() const
{
	return (voltage() - min_voltage()) / (max_voltage() - min_voltage()) * 100.0;
}

Battery::Type Battery::type() const
{
	return type_;
}

void Battery::auto_detect_type()
{
	if (type() == TYPE_AUTO_DETECT && voltage() > 0.0) {
		for (uint32_t i = TYPE_LIPO_1S; i < TYPE_MAX; ++i)
		if (voltage() <= lipo_1cell_max_v_ * i) {
			type_ = static_cast<Type>(i);
			break;
		}
	}
}

