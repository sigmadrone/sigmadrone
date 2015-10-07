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

#ifndef BMP180_H_
#define BMP180_H_

#include "i2cmaster.h"

/*unsigned integer types*/
typedef	uint8_t u8;/**< used for unsigned 8bit */
typedef	uint16_t u16;/**< used for unsigned 16bit */
typedef	uint32_t u32;/**< used for unsigned 32bit */
typedef	uint64_t u64;/**< used for unsigned 64bit */

/*signed integer types*/
typedef	int8_t s8;/**< used for signed 8bit */
typedef	int16_t s16;/**< used for signed 16bit */
typedef	int32_t s32;/**< used for signed 32bit */
typedef	int64_t s64;/**< used for signed 64bit */

struct bmp085_calibration_data {
	s16 AC1, AC2, AC3;
	u16 AC4, AC5, AC6;
	s16 B1, B2;
	s16 MB, MC, MD;
};

struct bmp085_data {
	struct	bmp085_calibration_data calibration;
	u8	oversampling_setting;
	u32	raw_temperature;
	u32	raw_pressure;
	float temperature;
	float pressure;
	u32	temp_measurement_period;
	unsigned long last_temp_measurement;
	u8	chip_id;
	s32	b6; /* calculated temperature correction coefficient */
};

class BMP180 : public bmp085_data {
public:
	BMP180(I2CMaster& i2c);
	~BMP180();
	void set_oversampling(unsigned char oversampling);
	unsigned char get_oversampling();
	void update_pressure();
	void update_temperature();
	float get_pressure();
	float get_temperature();

private:
	void read_calibration_data();
	void update_raw_pressure();
	void update_raw_temperature();

private:
	I2CMaster& i2c_;
};

#endif /* BMP180_H_ */
