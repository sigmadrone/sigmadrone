/*
 * battery.h
 *
 *  Created on: Sep 13, 2015
 *      Author: svassilev
 */

#ifndef BATTERY_H_
#define BATTERY_H_

#include "adc.h"

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
	void update(ADCHandle& adc);
	void set_type(Battery::Type type);
	float voltage() const;
	float max_voltage() const;
	float min_voltage() const;
	float nominal_voltage() const;
	float charge_percentage() const;
	Battery::Type type() const;

private:
	void auto_detect_type();

	static constexpr float r1_div_ = 100.0f;
	static constexpr float r2_div_ = 21.5f;
	static constexpr float lipo_1cell_max_v_ = 4.2f;
	static constexpr float lipo_1cell_nominal_v_ = 3.7f;
	static constexpr float lipo_1cell_min_v_ = 3.2f;

	float voltage_;
	Type type_;
};

#endif /* BATTERY_H_ */
