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

#include "battery.h"
#include "math.h"

const Voltage Battery::lipo_1cell_max_v_ = Voltage::from_volts(4.2f);
const Voltage Battery::lipo_1cell_nominal_v_ = Voltage::from_volts(3.7f);
const Voltage Battery::lipo_1cell_min_v_ = Voltage::from_volts(3.2f);


Battery::Battery(Battery::Type type) :
	voltage_(),
	type_(type)
{
}

Battery::~Battery() {}

void Battery::update(Voltage new_measurement)
{
	if ((new_measurement > voltage_ + lipo_1cell_min_v_) ||
		(new_measurement < voltage_ - lipo_1cell_min_v_)) {
		// newly measured voltage is too big, have to try to re-detect the battery
		// type
		type_ = TYPE_AUTO_DETECT;
	}
	voltage_ = new_measurement;
	auto_detect_type();
}

Voltage Battery::voltage() const
{
	return voltage_;
}

Voltage Battery::max_voltage() const
{
	return  lipo_1cell_max_v_ * type();
}

Voltage Battery::min_voltage() const
{
	return lipo_1cell_min_v_ * type();
}

Voltage Battery::nominal_voltage() const
{
	return lipo_1cell_nominal_v_ * type();
}

float Battery::charge_percentage() const
{
	if (type() == TYPE_UNKNOWN) {
		return 0.0;
	}
	return fmax((voltage() - min_voltage()) / (max_voltage() - min_voltage()) * 100.0, 0.0);
}

Battery::Type Battery::type() const
{
	return type_;
}

std::string Battery::type_as_string() const
{
	switch (type()) {
	case TYPE_LIPO_1S: return "LiPo_1S";
	case TYPE_LIPO_2S: return "LiPo_2S";
	case TYPE_LIPO_3S: return "LiPo_3S";
	case TYPE_LIPO_4S: return "LiPo_4S";
	case TYPE_LIPO_5S: return "LiPo_5S";
	case TYPE_LIPO_6S: return "LiPo_6S";
	default: return "Unknown";
	}
}

void Battery::auto_detect_type()
{
	if (type() == TYPE_AUTO_DETECT && !voltage().is_null()) {
		for (uint32_t i = TYPE_LIPO_1S; i < TYPE_MAX; ++i) {
			if (voltage() <= lipo_1cell_max_v_ * i) {
				type_ = static_cast<Type>(i);
				break;
			}
		}
	}
}

