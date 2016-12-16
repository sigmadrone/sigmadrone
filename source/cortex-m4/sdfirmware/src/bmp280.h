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

#ifndef BMP280_H_
#define BMP280_H_

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


class BMP280 {
public:
	BMP280(I2CMaster& i2c, u8 dev_addr);
	~BMP280();

public:
	double compensate_pressure_double(s32 v_uncomp_pressure_s32);
	u32 compensate_pressure_int32(s32 v_uncomp_pressure_s32);
	double compensate_temperature_double(s32 v_uncomp_temperature_s32);
	s32 compensate_temperature_int32(s32 adc_temp);
	u8 compute_wait_time();
	void get_calib_param(void);
	u8 get_filter();
	u8 get_oversamp_pressure();
	u8 get_power_mode();
	u8 get_standby_durn();
	void init();
	u8 read_register(u8 v_addr_u8, u8 *v_data_u8, u8 v_len_u8);

	void set_filter(u8 v_value_u8);
	void set_oversamp_pressure(u8 v_value_u8);
	void set_oversamp_temperature(u8 v_value_u8);
	void set_power_mode(u8 v_power_mode_u8);
	void set_standby_durn(u8 v_standby_durn_u8);
	void set_work_mode(u8 v_work_mode_u8);
	u8 write_register(u8 v_addr_u8, u8 *v_data_u8, u8 v_len_u8);
	s32 read_uncomp_temperature();
	s32 read_uncomp_pressure();
	u32 read_chip_id();
	double get_pressure();
	double get_temperature();


	struct calib_param_t {
		u16 dig_T1;/**<calibration T1 data*/
		s16 dig_T2;/**<calibration T2 data*/
		s16 dig_T3;/**<calibration T3 data*/
		u16 dig_P1;/**<calibration P1 data*/
		s16 dig_P2;/**<calibration P2 data*/
		s16 dig_P3;/**<calibration P3 data*/
		s16 dig_P4;/**<calibration P4 data*/
		s16 dig_P5;/**<calibration P5 data*/
		s16 dig_P6;/**<calibration P6 data*/
		s16 dig_P7;/**<calibration P7 data*/
		s16 dig_P8;/**<calibration P8 data*/
		s16 dig_P9;/**<calibration P9 data*/

		s32 t_fine;/**<calibration t_fine data*/
	};

private:
	I2CMaster& i2c_;
	u8 dev_addr_;				/**< device address of the sensor*/
	u8 chip_id_;				/**< chip id of the sensor*/
	u8 oversamp_temperature_;	/**< temperature over sampling*/
	u8 oversamp_pressure_;		/**< pressure over sampling*/
	struct calib_param_t calib_;


};

#endif /* BMP180_H_ */
