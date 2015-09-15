/*
 * battery.h
 *
 *  Created on: Sep 13, 2015
 *      Author: svassilev
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
