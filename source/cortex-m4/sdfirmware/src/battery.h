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

#ifndef BATTERY_H_
#define BATTERY_H_

#include <string>
#include "units.h"

class Battery {
public:

	typedef enum {
		TYPE_UNKNOWN = 0,
		TYPE_AUTO_DETECT = TYPE_UNKNOWN,
		TYPE_LIPO_1S = 1,
		TYPE_LIPO_2S = 2,
		TYPE_LIPO_3S = 3,
		TYPE_LIPO_4S = 4,
		TYPE_LIPO_5S = 5,
		TYPE_LIPO_6S = 6,
		TYPE_MAX
	} Type;

	Battery(Battery::Type type=TYPE_AUTO_DETECT);
	~Battery();
	void update(Voltage new_measurement);
	void set_type(Battery::Type type);
	Voltage voltage() const;
	Voltage max_voltage() const;
	Voltage min_voltage() const;
	Voltage nominal_voltage() const;
	float charge_percentage() const;
	Battery::Type type() const;
	std::string type_as_string() const;

private:
	void auto_detect_type();

	static const Voltage lipo_1cell_max_v_;
	static const Voltage lipo_1cell_nominal_v_;
	static const Voltage lipo_1cell_min_v_;

	Voltage voltage_;
	Type type_;
};

#endif /* BATTERY_H_ */
