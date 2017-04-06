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
#include <stdexcept>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bmp280.h"


#define BMP280_I2C_ADDR					(0xEE)

#define le16_to_cpu(x) ((u16)(x))
#define le32_to_cpu(x) ((u32)(x))
#define be16_to_cpu(x) ((u16)((((u16)(x) & (u16)0x00ffU) << 8) |(((u16)(x) & (u16)0xff00U) >> 8)))
#define be32_to_cpu(x) ((u32)( \
	(((u32)(x) & (u32)0x000000ffUL) << 24) | \
	(((u32)(x) & (u32)0x0000ff00UL) <<  8) | \
	(((u32)(x) & (u32)0x00ff0000UL) >>  8) | \
	(((u32)(x) & (u32)0xff000000UL) >> 24)))

static void delay_msec(unsigned int msec)
{
	vTaskDelay(msec / portTICK_RATE_MS);
}

BMP280::BMP280(SPIMaster& spi, uint8_t cs)
	: spi_(spi)
	, cs_(cs)
    , chip_id_(0)
	, oversamp_temperature_(0)
	, oversamp_pressure_(0)
{
	memset(&calib_, 0, sizeof(calib_));
	init();
	set_oversamp_temperature(BMP280_OVERSAMP_2X);
	set_oversamp_pressure(BMP280_OVERSAMP_16X);
	set_work_mode(BMP280_STANDARD_RESOLUTION_MODE);
	set_filter(BMP280_FILTER_COEFF_4);
	set_power_mode(BMP280_NORMAL_MODE);
}

BMP280::~BMP280() {
}

#define SPI_BUFFER_LEN 5
#define BUFFER_LENGTH	0xFF
#define	SPI_READ	0x80
#define SPI_WRITE	0x7F
#define BMP280_DATA_INDEX	1
#define BMP280_ADDRESS_INDEX	2

void BMP280::read_reg(uint8_t reg_addr, uint8_t* data, uint16_t nbytes)
{
	/* Reading is done by lowering CSB and first sending one control byte. The control bytes consist
	 * of the SPI register address (= full register address without bit 7) and the read command (bit 7 =
	 * RW = ‘1’). After writing the control byte, data is sent out of the SDO pin (SDI in 3-wire mode);
	 * the register address is automatically incremented
	 */
	spi_.read_reg(cs_, reg_addr | SPI_READ, data, nbytes);
}

void BMP280::write_reg(uint8_t reg_addr, uint8_t* data, uint16_t nbytes)
{
	/* Writing is done by lowering CSB and sending pairs control bytes and register data. The control
	 * bytes consist of the SPI register address (= full register address without bit 7) and the write
	 * command (bit7 = RW = ‘0’). Several pairs can be written without raising CSB. The transaction is
	 * ended by a raising CSB
	 */
	u8 array[SPI_BUFFER_LEN * BMP280_ADDRESS_INDEX];

	for (u8 stringpos = BMP280_INIT_VALUE; stringpos < nbytes; stringpos++) {
		/* the operation of (reg_addr++)&0x7F done as per the
		SPI communication protocol specified in the data sheet*/
		u8 index = stringpos * BMP280_ADDRESS_INDEX;
		array[index] = (reg_addr++) & SPI_WRITE;
		array[index + BMP280_DATA_INDEX] = *(data + stringpos);
	}
	spi_.write(cs_, array, nbytes * 2);
}


u32 BMP280::read_chip_id()
{
	u8 chip_id;
	read_reg(BMP280_CHIP_ID_REG, &chip_id, 1);
	return chip_id;
}

/*!
 *  @brief Reads actual pressure from uncompensated pressure
 *  and returns pressure in Pa as double.
 *  @note Output value of "96386.2"
 *  equals 96386.2 Pa = 963.862 hPa.
 *
 */
double BMP280::compensate_pressure_double(s32 v_uncomp_pressure_s32)
{
	double v_x1_u32r = BMP280_INIT_VALUE;
	double v_x2_u32r = BMP280_INIT_VALUE;
	double pressure = BMP280_INIT_VALUE;

	v_x1_u32r = ((double)calib_.t_fine/2.0) - 64000.0;
	v_x2_u32r = v_x1_u32r * v_x1_u32r *
	((double)calib_.dig_P6) / 32768.0;
	v_x2_u32r = v_x2_u32r + v_x1_u32r *
	((double)calib_.dig_P5) * 2.0;
	v_x2_u32r = (v_x2_u32r / 4.0) +
	(((double)calib_.dig_P4) * 65536.0);
	v_x1_u32r = (((double)calib_.dig_P3) *
		v_x1_u32r * v_x1_u32r / 524288.0 +
		((double)calib_.dig_P2) * v_x1_u32r) / 524288.0;
	v_x1_u32r = (1.0 + v_x1_u32r / 32768.0) *
	((double)calib_.dig_P1);
	pressure = 1048576.0 - (double)v_uncomp_pressure_s32;
	/* Avoid exception caused by division by zero */
	if ((v_x1_u32r > 0) || (v_x1_u32r < 0))
		pressure = (pressure - (v_x2_u32r / 4096.0)) *
					6250.0 / v_x1_u32r;
	else
	return BMP280_INVALID_DATA;
	v_x1_u32r = ((double)calib_.dig_P9) *
	pressure * pressure / 2147483648.0;
	v_x2_u32r = pressure * ((double)calib_.dig_P8) / 32768.0;
	pressure = pressure + (v_x1_u32r + v_x2_u32r +
			((double)calib_.dig_P7)) / 16.0;

	return pressure;
}

/*!
 *	@brief Reads actual pressure from uncompensated pressure
 *	and returns the value in Pascal(Pa)
 *	@note Output value of "96386" equals 96386 Pa =
 *	963.86 hPa = 963.86 millibar
 *
 *
 *
 *
 *  @param  v_uncomp_pressure_s32: value of uncompensated pressure
 *
 */
u32 BMP280::compensate_pressure_int32(s32 v_uncomp_pressure_s32)
{
	s32 v_x1_u32r = BMP280_INIT_VALUE;
	s32 v_x2_u32r = BMP280_INIT_VALUE;
	u32 v_pressure_u32 = BMP280_INIT_VALUE;
	/* calculate x1*/
	v_x1_u32r = (((s32)calib_.t_fine)
			>> BMP280_SHIFT_BIT_POSITION_BY_01_BIT) - (s32)64000;
	/* calculate x2*/
	v_x2_u32r = (((v_x1_u32r >> BMP280_SHIFT_BIT_POSITION_BY_02_BITS)
			* (v_x1_u32r >> BMP280_SHIFT_BIT_POSITION_BY_02_BITS))
			>> BMP280_SHIFT_BIT_POSITION_BY_11_BITS)
			* ((s32)calib_.dig_P6);
	v_x2_u32r = v_x2_u32r + ((v_x1_u32r *
			((s32)calib_.dig_P5))
			<< BMP280_SHIFT_BIT_POSITION_BY_01_BIT);
	v_x2_u32r = (v_x2_u32r >> BMP280_SHIFT_BIT_POSITION_BY_02_BITS)
			+ (((s32)calib_.dig_P4)
			<< BMP280_SHIFT_BIT_POSITION_BY_16_BITS);
	/* calculate x1*/
	v_x1_u32r = (((calib_.dig_P3
			* (((v_x1_u32r
			>> BMP280_SHIFT_BIT_POSITION_BY_02_BITS) * (v_x1_u32r
			>> BMP280_SHIFT_BIT_POSITION_BY_02_BITS))
			>> BMP280_SHIFT_BIT_POSITION_BY_13_BITS))
			>> BMP280_SHIFT_BIT_POSITION_BY_03_BITS)
			+ ((((s32)calib_.dig_P2)
			* v_x1_u32r)
			>> BMP280_SHIFT_BIT_POSITION_BY_01_BIT))
			>> BMP280_SHIFT_BIT_POSITION_BY_18_BITS;
	v_x1_u32r = ((((32768 + v_x1_u32r))
			* ((s32)calib_.dig_P1))
			>> BMP280_SHIFT_BIT_POSITION_BY_15_BITS);
	/* calculate pressure*/
	v_pressure_u32 = (((u32)(((s32)1048576) - v_uncomp_pressure_s32)
			- (v_x2_u32r >> BMP280_SHIFT_BIT_POSITION_BY_12_BITS)))
			* 3125;
	/* check overflow*/
	if (v_pressure_u32 < 0x80000000)
		/* Avoid exception caused by division by zero */
		if (v_x1_u32r != BMP280_INIT_VALUE)
			v_pressure_u32 = (v_pressure_u32
					<< BMP280_SHIFT_BIT_POSITION_BY_01_BIT)
					/ ((u32)v_x1_u32r);
		else
			return BMP280_INVALID_DATA;
	else
	/* Avoid exception caused by division by zero */
	if (v_x1_u32r != BMP280_INIT_VALUE)
		v_pressure_u32 = (v_pressure_u32 / (u32)v_x1_u32r) * 2;
	else
		return BMP280_INVALID_DATA;
	/* calculate x1*/
	v_x1_u32r = (((s32)calib_.dig_P9) * ((s32)(
			((v_pressure_u32
			>> BMP280_SHIFT_BIT_POSITION_BY_03_BITS)
			* (v_pressure_u32
			>> BMP280_SHIFT_BIT_POSITION_BY_03_BITS))
			>> BMP280_SHIFT_BIT_POSITION_BY_13_BITS)))
			>> BMP280_SHIFT_BIT_POSITION_BY_12_BITS;
	/* calculate x2*/
	v_x2_u32r = (((s32)(v_pressure_u32 >>
			BMP280_SHIFT_BIT_POSITION_BY_02_BITS))
			* ((s32)calib_.dig_P8))
			>> BMP280_SHIFT_BIT_POSITION_BY_13_BITS;
	/* calculate true pressure*/
	v_pressure_u32 = (u32)((s32)v_pressure_u32 + ((v_x1_u32r + v_x2_u32r
			+ calib_.dig_P7)
			>> BMP280_SHIFT_BIT_POSITION_BY_04_BITS));

	return v_pressure_u32;
}

/*!
 *	@brief Reads actual pressure from uncompensated pressure
 *	and returns the value in Pascal(Pa)
 *	@note Output value of "96386" equals 96386 Pa =
 *	963.86 hPa = 963.86 millibar
 *
 *
 *
 *
 *  @param  v_uncomp_pressure_s32: value of uncompensated pressure
 */
s32 BMP280::compensate_temperature_int32(s32 v_uncomp_temperature_s32)
{
	s32 v_x1_u32r = 0;
	s32 v_x2_u32r = 0;
	s32 temperature = 0;
	/* calculate true temperature*/
	/*calculate x1*/
	v_x1_u32r = ((((v_uncomp_temperature_s32
			>> BMP280_SHIFT_BIT_POSITION_BY_03_BITS)
			- ((s32)calib_.dig_T1
			<< BMP280_SHIFT_BIT_POSITION_BY_01_BIT)))
			* ((s32)calib_.dig_T2))
			>> BMP280_SHIFT_BIT_POSITION_BY_11_BITS;
	/*calculate x2*/
	v_x2_u32r = (((((v_uncomp_temperature_s32
			>> BMP280_SHIFT_BIT_POSITION_BY_04_BITS)
			- ((s32)calib_.dig_T1))
			* ((v_uncomp_temperature_s32
			>> BMP280_SHIFT_BIT_POSITION_BY_04_BITS)
			- ((s32)calib_.dig_T1)))
			>> BMP280_SHIFT_BIT_POSITION_BY_12_BITS)
			* ((s32)calib_.dig_T3))
			>> BMP280_SHIFT_BIT_POSITION_BY_14_BITS;
	/*calculate t_fine*/
	calib_.t_fine = v_x1_u32r + v_x2_u32r;
	/*calculate temperature*/
	temperature = (calib_.t_fine * 5 + 128)
			>> BMP280_SHIFT_BIT_POSITION_BY_08_BITS;

	return temperature;
}

/*!
 *	@brief Reads actual pressure from uncompensated pressure
 *	and returns pressure in Pa as double.
 *	@note Output value of "96386.2"
 *	equals 96386.2 Pa = 963.862 hPa.
 *
 *
 *
 *  @param v_uncomp_pressure_s32 : value of uncompensated pressure
 *
 */
double BMP280::compensate_temperature_double(s32 v_uncomp_temperature_s32)
{
	double v_x1_u32r = 0;
	double v_x2_u32r = 0;
	double temperature = 0;

	/* calculate x1*/
	v_x1_u32r = (((double)v_uncomp_temperature_s32) / 16384.0 -
			((double)calib_.dig_T1) / 1024.0) *
	((double)calib_.dig_T2);
	/* calculate x2*/
	v_x2_u32r = ((((double)v_uncomp_temperature_s32) / 131072.0 -
			((double)calib_.dig_T1) / 8192.0) *
			(((double)v_uncomp_temperature_s32) / 131072.0 -
			((double)calib_.dig_T1) / 8192.0)) *
	((double)calib_.dig_T3);
	/* calculate t_fine*/
	calib_.t_fine = (s32)(v_x1_u32r + v_x2_u32r);
	/* calculate true pressure*/
	temperature = (v_x1_u32r + v_x2_u32r) / 5120.0;

	return temperature;
}

/*!
 * @brief Computing waiting time for sensor data read
 *
 *
 */
u8 BMP280::compute_wait_time()
{
	u8 v_delaytime_u8r;

	v_delaytime_u8r = (T_INIT_MAX + T_MEASURE_PER_OSRS_MAX * (((1
			<< oversamp_temperature_)
			>> BMP280_SHIFT_BIT_POSITION_BY_01_BIT)
			+ ((1 << oversamp_pressure_)
			>> BMP280_SHIFT_BIT_POSITION_BY_01_BIT))
			+ ((oversamp_pressure_ > 0) ?
			T_SETUP_PRESSURE_MAX : 0) + 15) / 16;
	return v_delaytime_u8r;
}

/*!
 *	@brief This API is used to
 *	calibration parameters used for calculation in the registers
 *
 *  parameter | Register address |   bit
 *------------|------------------|----------------
 *	dig_T1    |  0x88 and 0x89   | from 0 : 7 to 8: 15
 *	dig_T2    |  0x8A and 0x8B   | from 0 : 7 to 8: 15
 *	dig_T3    |  0x8C and 0x8D   | from 0 : 7 to 8: 15
 *	dig_P1    |  0x8E and 0x8F   | from 0 : 7 to 8: 15
 *	dig_P2    |  0x90 and 0x91   | from 0 : 7 to 8: 15
 *	dig_P3    |  0x92 and 0x93   | from 0 : 7 to 8: 15
 *	dig_P4    |  0x94 and 0x95   | from 0 : 7 to 8: 15
 *	dig_P5    |  0x96 and 0x97   | from 0 : 7 to 8: 15
 *	dig_P6    |  0x98 and 0x99   | from 0 : 7 to 8: 15
 *	dig_P7    |  0x9A and 0x9B   | from 0 : 7 to 8: 15
 *	dig_P8    |  0x9C and 0x9D   | from 0 : 7 to 8: 15
 *	dig_P9    |  0x9E and 0x9F   | from 0 : 7 to 8: 15
 *
 *
 */
void BMP280::get_calib_param(void)
{
	/* variable used to return communication result*/
	u8 a_data_u8[BMP280_CALIB_DATA_SIZE] = {BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE, BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE, BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE, BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE, BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE, BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE, BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE, BMP280_INIT_VALUE,
			BMP280_INIT_VALUE, BMP280_INIT_VALUE};
	/* check the p_bmp280 structure pointer as NULL*/
	read_reg(BMP280_TEMPERATURE_CALIB_DIG_T1_LSB_REG,
			a_data_u8,
			BMP280_PRESSURE_TEMPERATURE_CALIB_DATA_LENGTH);
	/* read calibration values*/
	calib_.dig_T1 = (u16)((((u16)((u8)a_data_u8[
				BMP280_TEMPERATURE_CALIB_DIG_T1_MSB]))
				<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
				| a_data_u8[
				BMP280_TEMPERATURE_CALIB_DIG_T1_LSB]);
	calib_.dig_T2 = (s16)((((s16)((s8)a_data_u8[
				BMP280_TEMPERATURE_CALIB_DIG_T2_MSB]))
				<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
				| a_data_u8[
				BMP280_TEMPERATURE_CALIB_DIG_T2_LSB]);
	calib_.dig_T3 = (s16)((((s16)((s8)a_data_u8[
				BMP280_TEMPERATURE_CALIB_DIG_T3_MSB]))
				<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
				| a_data_u8[
				BMP280_TEMPERATURE_CALIB_DIG_T3_LSB]);
	calib_.dig_P1 = (u16)((((u16)((u8)a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P1_MSB]))
				<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
				| a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P1_LSB]);
	calib_.dig_P2 = (s16)((((s16)((s8)a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P2_MSB]))
				<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
				| a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P2_LSB]);
	calib_.dig_P3 = (s16)((((s16)((s8)a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P3_MSB]))
				<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
				| a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P3_LSB]);
	calib_.dig_P4 = (s16)((((s16)((s8)a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P4_MSB]))
				<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
				| a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P4_LSB]);
	calib_.dig_P5 = (s16)((((s16)((s8)a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P5_MSB]))
				<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
				| a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P5_LSB]);
	calib_.dig_P6 = (s16)((((s16)((s8)a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P6_MSB]))
				<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
				| a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P6_LSB]);
	calib_.dig_P7 = (s16)((((s16)((s8)a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P7_MSB]))
				<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
				| a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P7_LSB]);
	calib_.dig_P8 = (s16)((((s16)((s8)a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P8_MSB]))
				<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
				| a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P8_LSB]);
	calib_.dig_P9 = (s16)((((s16)((s8)a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P9_MSB]))
				<< BMP280_SHIFT_BIT_POSITION_BY_08_BITS)
				| a_data_u8[
				BMP280_PRESSURE_CALIB_DIG_P9_LSB]);
}

/*!
 *	@brief This API is used to reads filter setting
 *	in the register 0xF5 bit 3 and 4
 *
 *
 *
 *	@param v_value_u8 : The value of filter coefficient
 *	value	    |	Filter coefficient
 * -------------|-------------------------
 *	0x00        | BMP280_FILTER_COEFF_OFF
 *	0x01        | BMP280_FILTER_COEFF_2
 *	0x02        | BMP280_FILTER_COEFF_4
 *	0x03        | BMP280_FILTER_COEFF_8
 *	0x04        | BMP280_FILTER_COEFF_16
 */
u8 BMP280::get_filter()
{
	u8 v_value_u8;
	/* variable used to return communication result*/
	u8 v_data_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
	read_reg(BMP280_CONFIG_REG_FILTER__REG,
			&v_data_u8,
			BMP280_GEN_READ_WRITE_DATA_LENGTH);
	v_value_u8 = BMP280_GET_BITSLICE(v_data_u8,
			BMP280_CONFIG_REG_FILTER);
	return v_value_u8;
}



/*!
 *	@brief This API is used to get
 *	the pressure oversampling setting in the register 0xF4
 *	bits from 2 to 4
 *
 *        value             | Pressure oversampling
 *  ------------------------|------------------------------
 *       0x00               |  BMP280_OVERSAMP_SKIPPED
 *       0x01               |  BMP280_OVERSAMP_1X
 *       0x02               |  BMP280_OVERSAMP_2X
 *       0x03               |  BMP280_OVERSAMP_4X
 *       0x04               |  BMP280_OVERSAMP_8X
 *       0x05,0x06 and 0x07 |  BMP280_OVERSAMP_16X
 *
 *
 *
 */
u8 BMP280::get_oversamp_pressure()
{
	u8 v_value_u8;
	u8 v_data_u8 = BMP280_INIT_VALUE;

	/* read pressure over sampling */
	read_reg(BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__REG,
			&v_data_u8, BMP280_GEN_READ_WRITE_DATA_LENGTH);
	v_value_u8 = BMP280_GET_BITSLICE(v_data_u8,	BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE);
	oversamp_pressure_ = v_value_u8;
	return v_value_u8;
}


/*!
 *	@brief This API is used to set
 *	the pressure oversampling setting in the register 0xF4
 *	bits from 2 to 4
 *
 *        value             | Pressure oversampling
 *  ------------------------|------------------------------
 *       0x00               |  BMP280_OVERSAMP_SKIPPED
 *       0x01               |  BMP280_OVERSAMP_1X
 *       0x02               |  BMP280_OVERSAMP_2X
 *       0x03               |  BMP280_OVERSAMP_4X
 *       0x04               |  BMP280_OVERSAMP_8X
 *       0x05,0x06 and 0x07 |  BMP280_OVERSAMP_16X
 *
 */
void BMP280::set_oversamp_pressure(u8 v_value_u8)
{
	u8 v_data_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/

	read_reg(BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__REG,
			&v_data_u8, BMP280_GEN_READ_WRITE_DATA_LENGTH);

	v_data_u8 = BMP280_SET_BITSLICE(v_data_u8,
				BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE,
				v_value_u8);
	write_reg(
		BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__REG,
		&v_data_u8,
		BMP280_GEN_READ_WRITE_DATA_LENGTH);
	oversamp_pressure_ = v_value_u8;
}

/*!
 *	@brief This API is used to set
 *	the temperature oversampling setting in the register 0xF4
 *	bits from 5 to 7
 *
 *        value             | Temperature oversampling
 *  ------------------------|------------------------------
 *       0x00               |  BMP280_OVERSAMP_SKIPPED
 *       0x01               |  BMP280_OVERSAMP_1X
 *       0x02               |  BMP280_OVERSAMP_2X
 *       0x03               |  BMP280_OVERSAMP_4X
 *       0x04               |  BMP280_OVERSAMP_8X
 *       0x05,0x06 and 0x07 |  BMP280_OVERSAMP_16X
 *
 *
 *  @param v_value_u8 :The value of temperature over sampling
 *
 *
 */
void BMP280::set_oversamp_temperature(u8 v_value_u8)
{
	u8 v_data_u8 = BMP280_INIT_VALUE;
	read_reg(BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__REG,
			&v_data_u8, BMP280_GEN_READ_WRITE_DATA_LENGTH);
	v_data_u8 = BMP280_SET_BITSLICE(v_data_u8, BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE, v_value_u8);
	write_reg(
		BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__REG,
		&v_data_u8,
		BMP280_GEN_READ_WRITE_DATA_LENGTH);
	oversamp_temperature_ = v_value_u8;
}

/*!
 *	@brief This API used to get the
 *	Operational Mode from the sensor in the register 0xF4 bit 0 and 1
 *
 *
 *
 *	@param v_power_mode_u8 : The value of power mode value
 *  value            |   Power mode
 * ------------------|------------------
 *	0x00             | BMP280_SLEEP_MODE
 *	0x01 and 0x02    | BMP280_FORCED_MODE
 *	0x03             | BMP280_NORMAL_MODE
 *
 *
 */
u8 BMP280::get_power_mode()
{
	u8 v_power_mode_u8;
	u8 v_mode_u8 = BMP280_INIT_VALUE;

	read_reg(BMP280_CTRL_MEAS_REG_POWER_MODE__REG,
			&v_mode_u8, BMP280_GEN_READ_WRITE_DATA_LENGTH);
	v_power_mode_u8 = BMP280_GET_BITSLICE(v_mode_u8,
				BMP280_CTRL_MEAS_REG_POWER_MODE);
	return v_power_mode_u8;
}

/*!
 *	@brief This API used to set the
 *	Operational Mode from the sensor in the register 0xF4 bit 0 and 1
 *
 *
 *
 *	@param v_power_mode_u8 : The value of power mode value
 *  value            |   Power mode
 * ------------------|------------------
 *	0x00             | BMP280_SLEEP_MODE
 *	0x01 and 0x02    | BMP280_FORCED_MODE
 *	0x03             | BMP280_NORMAL_MODE
 *
 *
 */
void BMP280::set_power_mode(u8 v_power_mode_u8)
{
	u8 v_mode_u8 = BMP280_INIT_VALUE;
	if (v_power_mode_u8 <= BMP280_NORMAL_MODE) {
		/* write the power mode*/
		v_mode_u8 = (oversamp_temperature_
				<< BMP280_SHIFT_BIT_POSITION_BY_05_BITS)
				+ (oversamp_pressure_
				<< BMP280_SHIFT_BIT_POSITION_BY_02_BITS)
				+ v_power_mode_u8;
		write_reg(
				BMP280_CTRL_MEAS_REG_POWER_MODE__REG,
				&v_mode_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);
	} else {
		throw(std::runtime_error("BMP280::set_power_mode Out of range"));
	}
}

/*!
 *	@brief This API used to Read the
 *	standby duration time from the sensor in the register 0xF5 bit 5 to 7
 *
 *	@param v_standby_durn_u8 : The standby duration time value.
 *  value     |  standby duration
 * -----------|--------------------
 *    0x00    | BMP280_STANDBYTIME_1_MS
 *    0x01    | BMP280_STANDBYTIME_63_MS
 *    0x02    | BMP280_STANDBYTIME_125_MS
 *    0x03    | BMP280_STANDBYTIME_250_MS
 *    0x04    | BMP280_STANDBYTIME_500_MS
 *    0x05    | BMP280_STANDBYTIME_1000_MS
 *    0x06    | BMP280_STANDBYTIME_2000_MS
 *    0x07    | BMP280_STANDBYTIME_4000_MS
 *
 *
 *
 *  @return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
 */
u8 BMP280::get_standby_durn()
{
	u8 v_standby_durn_u8;
	u8 v_data_u8 = BMP280_INIT_VALUE;

	/* read the standby duration*/
	read_reg(BMP280_CONFIG_REG_STANDBY_DURN__REG, &v_data_u8,
			BMP280_GEN_READ_WRITE_DATA_LENGTH);
	v_standby_durn_u8 = BMP280_GET_BITSLICE(v_data_u8,
			BMP280_CONFIG_REG_STANDBY_DURN);
	return v_standby_durn_u8;
}

/*!
 *	@brief This API used to Read the
 *	standby duration time from the sensor
 *	in the register 0xF5 bit 5 to 7
 *	@note Normal mode comprises an
 *	automated perpetual cycling between an (active)
 *	Measurement period and an (inactive) standby period.
 *	@note The standby time is determined
 *	by the contents of the register t_sb.
 *	Standby time can be set using BMP280_STANDBYTIME_125_MS.
 *
 *	@note bmp280_set_standby_durn(BMP280_STANDBYTIME_125_MS)
 *
 *
 *
 *	@param v_standby_durn_u8 : The standby duration time value.
 *  value     |  standby duration
 * -----------|--------------------
 *    0x00    | BMP280_STANDBYTIME_1_MS
 *    0x01    | BMP280_STANDBYTIME_63_MS
 *    0x02    | BMP280_STANDBYTIME_125_MS
 *    0x03    | BMP280_STANDBYTIME_250_MS
 *    0x04    | BMP280_STANDBYTIME_500_MS
 *    0x05    | BMP280_STANDBYTIME_1000_MS
 *    0x06    | BMP280_STANDBYTIME_2000_MS
 *    0x07    | BMP280_STANDBYTIME_4000_MS
 *
 */
void BMP280::set_standby_durn(u8 v_standby_durn_u8)
{
	u8 v_data_u8 = BMP280_INIT_VALUE;

	/* write the standby duration*/
	read_reg(BMP280_CONFIG_REG_STANDBY_DURN__REG, &v_data_u8,
			BMP280_GEN_READ_WRITE_DATA_LENGTH);
	v_data_u8 = BMP280_SET_BITSLICE(v_data_u8,
				BMP280_CONFIG_REG_STANDBY_DURN,
				v_standby_durn_u8);
	write_reg(
				BMP280_CONFIG_REG_STANDBY_DURN__REG,
				&v_data_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);
}

/*!
 *	@brief This API is used to write
 *	 the working mode of the sensor
 *
 *
 *  @param v_work_mode_u8 : The value of work mode
 *   value      |  mode
 * -------------|-------------
 *    0         | BMP280_ULTRA_LOW_POWER_MODE
 *    1         | BMP280_LOW_POWER_MODE
 *    2         | BMP280_STANDARD_RESOLUTION_MODE
 *    3         | BMP280_HIGH_RESOLUTION_MODE
 *    4         | BMP280_ULTRA_HIGH_RESOLUTION_MODE
 *
 */
void BMP280::set_work_mode(u8 v_work_mode_u8)
{
	u8 v_data_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
	if (v_work_mode_u8 > BMP280_ULTRA_HIGH_RESOLUTION_MODE)
		throw(std::runtime_error("Out of range"));

	read_reg(
			BMP280_CTRL_MEAS_REG, &v_data_u8,
			BMP280_GEN_READ_WRITE_DATA_LENGTH);
	switch (v_work_mode_u8) {
	/* write work mode*/
	case BMP280_ULTRA_LOW_POWER_MODE:
		oversamp_temperature_ = BMP280_ULTRALOWPOWER_OVERSAMP_TEMPERATURE;
		oversamp_pressure_ = BMP280_ULTRALOWPOWER_OVERSAMP_PRESSURE;
		break;
	case BMP280_LOW_POWER_MODE:
		oversamp_temperature_ = BMP280_LOWPOWER_OVERSAMP_TEMPERATURE;
		oversamp_pressure_ = BMP280_LOWPOWER_OVERSAMP_PRESSURE;
		break;
	case BMP280_STANDARD_RESOLUTION_MODE:
		oversamp_temperature_ =	BMP280_STANDARDRESOLUTION_OVERSAMP_TEMPERATURE;
		oversamp_pressure_ = BMP280_STANDARDRESOLUTION_OVERSAMP_PRESSURE;
		break;
	case BMP280_HIGH_RESOLUTION_MODE:
		oversamp_temperature_ = BMP280_HIGHRESOLUTION_OVERSAMP_TEMPERATURE;
		oversamp_pressure_ = BMP280_HIGHRESOLUTION_OVERSAMP_PRESSURE;
		break;
	case BMP280_ULTRA_HIGH_RESOLUTION_MODE:
		oversamp_temperature_ = BMP280_ULTRAHIGHRESOLUTION_OVERSAMP_TEMPERATURE;
		oversamp_pressure_ = BMP280_ULTRAHIGHRESOLUTION_OVERSAMP_PRESSURE;
		break;
	}
	v_data_u8 = BMP280_SET_BITSLICE(v_data_u8,
		BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE,
		oversamp_temperature_);
	v_data_u8 = BMP280_SET_BITSLICE(v_data_u8,
		BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE,
		oversamp_pressure_);
	write_reg(
			BMP280_CTRL_MEAS_REG,
			&v_data_u8,
			BMP280_GEN_READ_WRITE_DATA_LENGTH);
}

/*!
 *	@brief This function is used for initialize
 *	the bus read and bus write functions
 *  and assign the chip id and I2C address of the BMP280 sensor
 *	chip id is read in the register 0xD0 bit from 0 to 7
 *
 *
 */
void BMP280::init()
{
	u8 v_data_u8 = BMP280_INIT_VALUE;
	u8 v_chip_id_read_count = BMP280_CHIP_ID_READ_COUNT;

	while (v_chip_id_read_count > 0) {
		/* read chip id */
		read_reg(
				BMP280_CHIP_ID_REG, &v_data_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);
		/* Check for the correct chip id */
		if ((v_data_u8 == BMP280_CHIP_ID1)
			|| (v_data_u8 == BMP280_CHIP_ID2)
			|| (v_data_u8 == BMP280_CHIP_ID3))
			break;
		v_chip_id_read_count--;
		/* Delay added concerning the low speed of power up system to
		facilitate the proper reading of the chip ID */
		delay_msec(BMP280_REGISTER_READ_DELAY);
	}

	/*assign chip ID to the global structure*/
	chip_id_ = v_data_u8;
	get_calib_param();
}


/*!
 *	@brief This API is used to write filter setting
 *	in the register 0xF5 bit 3 and 4
 *
 *
 *
 *	@param v_value_u8 : The value of filter coefficient
 *	value	    |	Filter coefficient
 * -------------|-------------------------
 *	0x00        | BMP280_FILTER_COEFF_OFF
 *	0x01        | BMP280_FILTER_COEFF_2
 *	0x02        | BMP280_FILTER_COEFF_4
 *	0x03        | BMP280_FILTER_COEFF_8
 *	0x04        | BMP280_FILTER_COEFF_16
 *
 */
void BMP280::set_filter(u8 v_value_u8)
{
	u8 v_data_u8 = BMP280_INIT_VALUE;
	/* check the p_bmp280 structure pointer as NULL*/
		/* write filter*/
	read_reg(
				BMP280_CONFIG_REG_FILTER__REG, &v_data_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);
	v_data_u8 = BMP280_SET_BITSLICE(v_data_u8, BMP280_CONFIG_REG_FILTER, v_value_u8);

	write_reg(
				BMP280_CONFIG_REG_FILTER__REG,
				&v_data_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);
}


/*!
 *	@brief This API is used to read uncompensated temperature
 *	in the registers 0xFA, 0xFB and 0xFC
 *	@note 0xFA -> MSB -> bit from 0 to 7
 *	@note 0xFB -> LSB -> bit from 0 to 7
 *	@note 0xFC -> LSB -> bit from 4 to 7
 *
 *	@param v_uncomp_temperature_s32 : The uncompensated temperature.
 *
 *
 */
s32 BMP280::read_uncomp_temperature()
{
	s32 v_uncomp_temperature_s32;
	/* Array holding the MSB and LSb value
	 a_data_u8r[0] - Temperature MSB
	 a_data_u8r[1] - Temperature LSB
	 a_data_u8r[2] - Temperature LSB
	 */
	u8 a_data_u8r[BMP280_TEMPERATURE_DATA_SIZE] =
		{	BMP280_INIT_VALUE, BMP280_INIT_VALUE, BMP280_INIT_VALUE};
	/* check the p_bmp280 structure pointer as NULL*/
		/* read temperature data */
	read_reg(
				BMP280_TEMPERATURE_MSB_REG, a_data_u8r,
				BMP280_TEMPERATURE_DATA_LENGTH);
	v_uncomp_temperature_s32 = (s32)((((u32)(
				a_data_u8r[BMP280_TEMPERATURE_MSB_DATA]))
				<< BMP280_SHIFT_BIT_POSITION_BY_12_BITS)
				| (((u32)(
				a_data_u8r[BMP280_TEMPERATURE_LSB_DATA]))
				<< BMP280_SHIFT_BIT_POSITION_BY_04_BITS)
				| ((u32)a_data_u8r[BMP280_TEMPERATURE_XLSB_DATA]
				>> BMP280_SHIFT_BIT_POSITION_BY_04_BITS));
	return v_uncomp_temperature_s32;
}

s32 BMP280::read_uncomp_pressure()
{
	s32 v_uncomp_pressure_s32;

	/* Array holding the MSB and LSb value
	 a_data_u8[0] - Pressure MSB
	 a_data_u8[1] - Pressure LSB
	 a_data_u8[2] - Pressure LSB
	 */
	u8 a_data_u8[BMP280_PRESSURE_DATA_SIZE] = {0, 0, 0};
	/* check the p_bmp280 structure pointer as NULL*/
	read_reg( BMP280_PRESSURE_MSB_REG, a_data_u8, BMP280_PRESSURE_DATA_LENGTH);
	v_uncomp_pressure_s32 = (s32)((((u32)(
				a_data_u8[BMP280_PRESSURE_MSB_DATA]))
				<< BMP280_SHIFT_BIT_POSITION_BY_12_BITS)
				| (((u32)(a_data_u8[BMP280_PRESSURE_LSB_DATA]))
				<< BMP280_SHIFT_BIT_POSITION_BY_04_BITS)
				| ((u32)a_data_u8[BMP280_PRESSURE_XLSB_DATA]
				>> BMP280_SHIFT_BIT_POSITION_BY_04_BITS));
	return v_uncomp_pressure_s32;
}

double BMP280::get_pressure()
{
	return compensate_pressure_double(read_uncomp_pressure());
}

double BMP280::get_temperature()
{
	return compensate_temperature_double(read_uncomp_temperature());
}

