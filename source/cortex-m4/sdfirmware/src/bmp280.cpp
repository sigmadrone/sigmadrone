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


/*!
* @brief defines the calling parameter types of the BMP280_DELAY_FUNCTION
*/
#define BMP280_DELAY_PARAM_TYPES u16
/***************************************************************/
/**\name	GET AND SET BITSLICE FUNCTIONS       */
/***************************************************************/
#define BMP280_GET_BITSLICE(regvar, bitname)\
	((regvar & bitname##__MSK) >> bitname##__POS)

#define BMP280_SET_BITSLICE(regvar, bitname, val)\
	((regvar & ~bitname##__MSK) | ((val<<bitname##__POS)&bitname##__MSK))

/***************************************************************/
/**\name	COMMON USED CONSTANTS       */
/***************************************************************/
/* Constants */
#define BMP280_NULL                          (0)
#define BMP280_RETURN_FUNCTION_TYPE          s8
/* right shift definitions*/
#define BMP280_SHIFT_BIT_POSITION_BY_01_BIT				 (1)
#define BMP280_SHIFT_BIT_POSITION_BY_02_BITS			(2)
#define BMP280_SHIFT_BIT_POSITION_BY_03_BITS			(3)
#define BMP280_SHIFT_BIT_POSITION_BY_04_BITS			(4)
#define BMP280_SHIFT_BIT_POSITION_BY_05_BITS			(5)
#define BMP280_SHIFT_BIT_POSITION_BY_08_BITS			(8)
#define BMP280_SHIFT_BIT_POSITION_BY_11_BITS			(11)
#define BMP280_SHIFT_BIT_POSITION_BY_12_BITS			(12)
#define BMP280_SHIFT_BIT_POSITION_BY_13_BITS			(13)
#define BMP280_SHIFT_BIT_POSITION_BY_14_BITS			(14)
#define BMP280_SHIFT_BIT_POSITION_BY_15_BITS			(15)
#define BMP280_SHIFT_BIT_POSITION_BY_16_BITS			(16)
#define BMP280_SHIFT_BIT_POSITION_BY_17_BITS			(17)
#define BMP280_SHIFT_BIT_POSITION_BY_18_BITS			(18)
#define BMP280_SHIFT_BIT_POSITION_BY_19_BITS			(19)
#define BMP280_SHIFT_BIT_POSITION_BY_25_BITS			(25)
#define BMP280_SHIFT_BIT_POSITION_BY_31_BITS			(31)
#define BMP280_SHIFT_BIT_POSITION_BY_33_BITS			(33)
#define BMP280_SHIFT_BIT_POSITION_BY_35_BITS			(35)
#define BMP280_SHIFT_BIT_POSITION_BY_47_BITS			(47)

/* numeric definitions */
#define	BMP280_PRESSURE_TEMPERATURE_CALIB_DATA_LENGTH	(24)
#define	BMP280_GEN_READ_WRITE_DATA_LENGTH				(1)
#define BMP280_REGISTER_READ_DELAY						(1)
#define	BMP280_TEMPERATURE_DATA_LENGTH					(3)
#define	BMP280_PRESSURE_DATA_LENGTH						(3)
#define	BMP280_ALL_DATA_FRAME_LENGTH					(6)
#define	BMP280_INIT_VALUE								(0)
#define	BMP280_CHIP_ID_READ_COUNT						(5)
#define	BMP280_CHIP_ID_READ_SUCCESS						(0)
#define	BMP280_CHIP_ID_READ_FAIL						((s8)-1)
#define	BMP280_INVALID_DATA								(0)

/************************************************/
/**\name	ERROR CODES      */
/************************************************/
#define	SUCCESS			((u8)0)
#define E_BMP280_NULL_PTR         ((s8)-127)
#define E_BMP280_COMM_RES         ((s8)-1)
#define E_BMP280_OUT_OF_RANGE     ((s8)-2)
#define ERROR                     ((s8)-1)
/************************************************/
/**\name	CHIP ID DEFINITION       */
/***********************************************/
#define BMP280_CHIP_ID1		(0x56)
#define BMP280_CHIP_ID2		(0x57)
#define BMP280_CHIP_ID3		(0x58)
/************************************************/
/**\name	I2C ADDRESS DEFINITION       */
/***********************************************/
#define BMP280_I2C_ADDRESS1                  (0x76)
#define BMP280_I2C_ADDRESS2                  (0x77)
/************************************************/
/**\name	POWER MODE DEFINITION       */
/***********************************************/
/* Sensor Specific constants */
#define BMP280_SLEEP_MODE                    (0x00)
#define BMP280_FORCED_MODE                   (0x01)
#define BMP280_NORMAL_MODE                   (0x03)
#define BMP280_SOFT_RESET_CODE               (0xB6)
/************************************************/
/**\name	STANDBY TIME DEFINITION       */
/***********************************************/
#define BMP280_STANDBY_TIME_1_MS              (0x00)
#define BMP280_STANDBY_TIME_63_MS             (0x01)
#define BMP280_STANDBY_TIME_125_MS            (0x02)
#define BMP280_STANDBY_TIME_250_MS            (0x03)
#define BMP280_STANDBY_TIME_500_MS            (0x04)
#define BMP280_STANDBY_TIME_1000_MS           (0x05)
#define BMP280_STANDBY_TIME_2000_MS           (0x06)
#define BMP280_STANDBY_TIME_4000_MS           (0x07)
/************************************************/
/**\name	OVERSAMPLING DEFINITION       */
/***********************************************/
#define BMP280_OVERSAMP_SKIPPED               (0x00)
#define BMP280_OVERSAMP_1X                    (0x01)
#define BMP280_OVERSAMP_2X                    (0x02)
#define BMP280_OVERSAMP_4X                    (0x03)
#define BMP280_OVERSAMP_8X                    (0x04)
#define BMP280_OVERSAMP_16X                   (0x05)
/************************************************/
/**\name	WORKING MODE DEFINITION       */
/***********************************************/
#define BMP280_ULTRA_LOW_POWER_MODE          (0x00)
#define BMP280_LOW_POWER_MODE	             (0x01)
#define BMP280_STANDARD_RESOLUTION_MODE      (0x02)
#define BMP280_HIGH_RESOLUTION_MODE          (0x03)
#define BMP280_ULTRA_HIGH_RESOLUTION_MODE    (0x04)

#define BMP280_ULTRALOWPOWER_OVERSAMP_PRESSURE             BMP280_OVERSAMP_1X
#define BMP280_ULTRALOWPOWER_OVERSAMP_TEMPERATURE          BMP280_OVERSAMP_1X

#define BMP280_LOWPOWER_OVERSAMP_PRESSURE	               BMP280_OVERSAMP_2X
#define BMP280_LOWPOWER_OVERSAMP_TEMPERATURE	           BMP280_OVERSAMP_1X

#define BMP280_STANDARDRESOLUTION_OVERSAMP_PRESSURE        BMP280_OVERSAMP_4X
#define BMP280_STANDARDRESOLUTION_OVERSAMP_TEMPERATURE     BMP280_OVERSAMP_1X

#define BMP280_HIGHRESOLUTION_OVERSAMP_PRESSURE            BMP280_OVERSAMP_8X
#define BMP280_HIGHRESOLUTION_OVERSAMP_TEMPERATURE         BMP280_OVERSAMP_1X

#define BMP280_ULTRAHIGHRESOLUTION_OVERSAMP_PRESSURE       BMP280_OVERSAMP_16X
#define BMP280_ULTRAHIGHRESOLUTION_OVERSAMP_TEMPERATURE    BMP280_OVERSAMP_2X
/************************************************/
/**\name	FILTER DEFINITION       */
/***********************************************/
#define BMP280_FILTER_COEFF_OFF               (0x00)
#define BMP280_FILTER_COEFF_2                 (0x01)
#define BMP280_FILTER_COEFF_4                 (0x02)
#define BMP280_FILTER_COEFF_8                 (0x03)
#define BMP280_FILTER_COEFF_16                (0x04)
/************************************************/
/**\name	DELAY TIME DEFINITION       */
/***********************************************/
#define T_INIT_MAX					(20)
/* 20/16 = 1.25 ms */
#define T_MEASURE_PER_OSRS_MAX				(37)
/* 37/16 = 2.3125 ms*/
#define T_SETUP_PRESSURE_MAX				(10)
/* 10/16 = 0.625 ms */
/************************************************/
/**\name	CALIBRATION PARAMETERS DEFINITION       */
/***********************************************/
/*calibration parameters */
#define BMP280_TEMPERATURE_CALIB_DIG_T1_LSB_REG             (0x88)
#define BMP280_TEMPERATURE_CALIB_DIG_T1_MSB_REG             (0x89)
#define BMP280_TEMPERATURE_CALIB_DIG_T2_LSB_REG             (0x8A)
#define BMP280_TEMPERATURE_CALIB_DIG_T2_MSB_REG             (0x8B)
#define BMP280_TEMPERATURE_CALIB_DIG_T3_LSB_REG             (0x8C)
#define BMP280_TEMPERATURE_CALIB_DIG_T3_MSB_REG             (0x8D)
#define BMP280_PRESSURE_CALIB_DIG_P1_LSB_REG                (0x8E)
#define BMP280_PRESSURE_CALIB_DIG_P1_MSB_REG                (0x8F)
#define BMP280_PRESSURE_CALIB_DIG_P2_LSB_REG                (0x90)
#define BMP280_PRESSURE_CALIB_DIG_P2_MSB_REG                (0x91)
#define BMP280_PRESSURE_CALIB_DIG_P3_LSB_REG                (0x92)
#define BMP280_PRESSURE_CALIB_DIG_P3_MSB_REG                (0x93)
#define BMP280_PRESSURE_CALIB_DIG_P4_LSB_REG                (0x94)
#define BMP280_PRESSURE_CALIB_DIG_P4_MSB_REG                (0x95)
#define BMP280_PRESSURE_CALIB_DIG_P5_LSB_REG                (0x96)
#define BMP280_PRESSURE_CALIB_DIG_P5_MSB_REG                (0x97)
#define BMP280_PRESSURE_CALIB_DIG_P6_LSB_REG                (0x98)
#define BMP280_PRESSURE_CALIB_DIG_P6_MSB_REG                (0x99)
#define BMP280_PRESSURE_CALIB_DIG_P7_LSB_REG                (0x9A)
#define BMP280_PRESSURE_CALIB_DIG_P7_MSB_REG                (0x9B)
#define BMP280_PRESSURE_CALIB_DIG_P8_LSB_REG                (0x9C)
#define BMP280_PRESSURE_CALIB_DIG_P8_MSB_REG                (0x9D)
#define BMP280_PRESSURE_CALIB_DIG_P9_LSB_REG                (0x9E)
#define BMP280_PRESSURE_CALIB_DIG_P9_MSB_REG                (0x9F)
/************************************************/
/**\name	REGISTER ADDRESS DEFINITION       */
/***********************************************/
#define BMP280_CHIP_ID_REG                   (0xD0)  /*Chip ID Register */
#define BMP280_RST_REG                       (0xE0) /*Softreset Register */
#define BMP280_STAT_REG                      (0xF3)  /*Status Register */
#define BMP280_CTRL_MEAS_REG                 (0xF4)  /*Ctrl Measure Register */
#define BMP280_CONFIG_REG                    (0xF5)  /*Configuration Register */
#define BMP280_PRESSURE_MSB_REG              (0xF7)  /*Pressure MSB Register */
#define BMP280_PRESSURE_LSB_REG              (0xF8)  /*Pressure LSB Register */
#define BMP280_PRESSURE_XLSB_REG             (0xF9)  /*Pressure XLSB Register */
#define BMP280_TEMPERATURE_MSB_REG           (0xFA)  /*Temperature MSB Reg */
#define BMP280_TEMPERATURE_LSB_REG           (0xFB)  /*Temperature LSB Reg */
#define BMP280_TEMPERATURE_XLSB_REG          (0xFC)  /*Temperature XLSB Reg */
/************************************************/
/**\name	BIT LENGTH,POSITION AND MASK DEFINITION      */
/***********************************************/
/* Status Register */
#define BMP280_STATUS_REG_MEASURING__POS           (3)
#define BMP280_STATUS_REG_MEASURING__MSK           (0x08)
#define BMP280_STATUS_REG_MEASURING__LEN           (1)
#define BMP280_STATUS_REG_MEASURING__REG           (BMP280_STAT_REG)

#define BMP280_STATUS_REG_IM_UPDATE__POS            (0)
#define BMP280_STATUS_REG_IM_UPDATE__MSK            (0x01)
#define BMP280_STATUS_REG_IM_UPDATE__LEN            (1)
#define BMP280_STATUS_REG_IM_UPDATE__REG           (BMP280_STAT_REG)
/************************************************/
/**\name	BIT LENGTH,POSITION AND MASK DEFINITION
FOR TEMPERATURE OVERSAMPLING */
/***********************************************/
/* Control Measurement Register */
#define BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__POS             (5)
#define BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__MSK             (0xE0)
#define BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__LEN             (3)
#define BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__REG             \
(BMP280_CTRL_MEAS_REG)
/************************************************/
/**\name	BIT LENGTH,POSITION AND MASK DEFINITION
FOR PRESSURE OVERSAMPLING */
/***********************************************/
#define BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__POS             (2)
#define BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__MSK             (0x1C)
#define BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__LEN             (3)
#define BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__REG             \
(BMP280_CTRL_MEAS_REG)
/************************************************/
/**\name	BIT LENGTH,POSITION AND MASK DEFINITION
FOR POWER MODE */
/***********************************************/
#define BMP280_CTRL_MEAS_REG_POWER_MODE__POS              (0)
#define BMP280_CTRL_MEAS_REG_POWER_MODE__MSK              (0x03)
#define BMP280_CTRL_MEAS_REG_POWER_MODE__LEN              (2)
#define BMP280_CTRL_MEAS_REG_POWER_MODE__REG             (BMP280_CTRL_MEAS_REG)
/************************************************/
/**\name	BIT LENGTH,POSITION AND MASK DEFINITION
FOR STANDBY DURATION */
/***********************************************/
/* Configuration Register */
#define BMP280_CONFIG_REG_STANDBY_DURN__POS                 (5)
#define BMP280_CONFIG_REG_STANDBY_DURN__MSK                 (0xE0)
#define BMP280_CONFIG_REG_STANDBY_DURN__LEN                 (3)
#define BMP280_CONFIG_REG_STANDBY_DURN__REG                 (BMP280_CONFIG_REG)
/************************************************/
/**\name	BIT LENGTH,POSITION AND MASK DEFINITION
FOR IIR FILTER */
/***********************************************/
#define BMP280_CONFIG_REG_FILTER__POS              (2)
#define BMP280_CONFIG_REG_FILTER__MSK              (0x1C)
#define BMP280_CONFIG_REG_FILTER__LEN              (3)
#define BMP280_CONFIG_REG_FILTER__REG              (BMP280_CONFIG_REG)
/************************************************/
/**\name	BIT LENGTH,POSITION AND MASK DEFINITION
FOR SPI ENABLE*/
/***********************************************/
#define BMP280_CONFIG_REG_SPI3_ENABLE__POS             (0)
#define BMP280_CONFIG_REG_SPI3_ENABLE__MSK             (0x01)
#define BMP280_CONFIG_REG_SPI3_ENABLE__LEN             (1)
#define BMP280_CONFIG_REG_SPI3_ENABLE__REG             (BMP280_CONFIG_REG)
/************************************************/
/**\name	BIT LENGTH,POSITION AND MASK DEFINITION
FOR PRESSURE AND TEMPERATURE DATA REGISTERS */
/***********************************************/
/* Data Register */
#define BMP280_PRESSURE_XLSB_REG_DATA__POS         (4)
#define BMP280_PRESSURE_XLSB_REG_DATA__MSK         (0xF0)
#define BMP280_PRESSURE_XLSB_REG_DATA__LEN         (4)
#define BMP280_PRESSURE_XLSB_REG_DATA__REG         (BMP280_PRESSURE_XLSB_REG)

#define BMP280_TEMPERATURE_XLSB_REG_DATA__POS      (4)
#define BMP280_TEMPERATURE_XLSB_REG_DATA__MSK      (0xF0)
#define BMP280_TEMPERATURE_XLSB_REG_DATA__LEN      (4)
#define BMP280_TEMPERATURE_XLSB_REG_DATA__REG      (BMP280_TEMPERATURE_XLSB_REG)
/************************************************/
/**\name	BUS READ AND WRITE FUNCTION POINTERS */
/***********************************************/
#define BMP280_WR_FUNC_PTR	s8 (*bus_write)(u8, u8, u8 *, u8)

#define BMP280_RD_FUNC_PTR	s8 (*bus_read)(u8, u8, u8 *, u8)

#define BMP280_MDELAY_DATA_TYPE u32
/****************************************************/
/**\name	DEFINITIONS FOR ARRAY SIZE OF DATA   */
/***************************************************/
#define	BMP280_TEMPERATURE_DATA_SIZE		(3)
#define	BMP280_PRESSURE_DATA_SIZE		(3)
#define	BMP280_DATA_FRAME_SIZE			(6)
#define	BMP280_CALIB_DATA_SIZE			(24)

#define	BMP280_TEMPERATURE_MSB_DATA		(0)
#define	BMP280_TEMPERATURE_LSB_DATA		(1)
#define	BMP280_TEMPERATURE_XLSB_DATA		(2)

#define	BMP280_PRESSURE_MSB_DATA		(0)
#define	BMP280_PRESSURE_LSB_DATA		(1)
#define	BMP280_PRESSURE_XLSB_DATA		(2)

#define	BMP280_DATA_FRAME_PRESSURE_MSB_BYTE	(0)
#define	BMP280_DATA_FRAME_PRESSURE_LSB_BYTE	(1)
#define	BMP280_DATA_FRAME_PRESSURE_XLSB_BYTE	(2)
#define	BMP280_DATA_FRAME_TEMPERATURE_MSB_BYTE	(3)
#define	BMP280_DATA_FRAME_TEMPERATURE_LSB_BYTE	(4)
#define	BMP280_DATA_FRAME_TEMPERATURE_XLSB_BYTE	(5)

/****************************************************/
/**\name	ARRAY PARAMETER FOR CALIBRATION     */
/***************************************************/
#define	BMP280_TEMPERATURE_CALIB_DIG_T1_LSB	(0)
#define	BMP280_TEMPERATURE_CALIB_DIG_T1_MSB	(1)
#define	BMP280_TEMPERATURE_CALIB_DIG_T2_LSB	(2)
#define	BMP280_TEMPERATURE_CALIB_DIG_T2_MSB	(3)
#define	BMP280_TEMPERATURE_CALIB_DIG_T3_LSB	(4)
#define	BMP280_TEMPERATURE_CALIB_DIG_T3_MSB	(5)
#define	BMP280_PRESSURE_CALIB_DIG_P1_LSB	(6)
#define	BMP280_PRESSURE_CALIB_DIG_P1_MSB	(7)
#define	BMP280_PRESSURE_CALIB_DIG_P2_LSB	(8)
#define	BMP280_PRESSURE_CALIB_DIG_P2_MSB	(9)
#define	BMP280_PRESSURE_CALIB_DIG_P3_LSB	(10)
#define	BMP280_PRESSURE_CALIB_DIG_P3_MSB	(11)
#define	BMP280_PRESSURE_CALIB_DIG_P4_LSB	(12)
#define	BMP280_PRESSURE_CALIB_DIG_P4_MSB	(13)
#define	BMP280_PRESSURE_CALIB_DIG_P5_LSB	(14)
#define	BMP280_PRESSURE_CALIB_DIG_P5_MSB	(15)
#define	BMP280_PRESSURE_CALIB_DIG_P6_LSB	(16)
#define	BMP280_PRESSURE_CALIB_DIG_P6_MSB	(17)
#define	BMP280_PRESSURE_CALIB_DIG_P7_LSB	(18)
#define	BMP280_PRESSURE_CALIB_DIG_P7_MSB	(19)
#define	BMP280_PRESSURE_CALIB_DIG_P8_LSB	(20)
#define	BMP280_PRESSURE_CALIB_DIG_P8_MSB	(21)
#define	BMP280_PRESSURE_CALIB_DIG_P9_LSB	(22)
#define	BMP280_PRESSURE_CALIB_DIG_P9_MSB	(23)

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

BMP280::BMP280(I2CMaster& i2c, u8 dev_addr)
	: i2c_(i2c)
	, dev_addr_(dev_addr)
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
	// TODO Auto-generated destructor stub
}

u32 BMP280::read_chip_id()
{
	return i2c_.read8(dev_addr_, BMP280_CHIP_ID_REG);
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
	i2c_.read(dev_addr_,
			BMP280_TEMPERATURE_CALIB_DIG_T1_LSB_REG,
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
	i2c_.read(dev_addr_,
			BMP280_CONFIG_REG_FILTER__REG, &v_data_u8,
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
	i2c_.read(dev_addr_,
				BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__REG,
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

	i2c_.read(dev_addr_,
			BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__REG,
			&v_data_u8, BMP280_GEN_READ_WRITE_DATA_LENGTH);

	v_data_u8 = BMP280_SET_BITSLICE(v_data_u8,
				BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE,
				v_value_u8);
	i2c_.write(
		dev_addr_,
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
	i2c_.read(dev_addr_,
			BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__REG,
			&v_data_u8, BMP280_GEN_READ_WRITE_DATA_LENGTH);
	v_data_u8 = BMP280_SET_BITSLICE(v_data_u8, BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE, v_value_u8);
	i2c_.write(dev_addr_,
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

	i2c_.read(dev_addr_,
				BMP280_CTRL_MEAS_REG_POWER_MODE__REG,
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
		i2c_.write(dev_addr_,
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
	i2c_.read(dev_addr_,
			BMP280_CONFIG_REG_STANDBY_DURN__REG, &v_data_u8,
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
	i2c_.read(dev_addr_,
			BMP280_CONFIG_REG_STANDBY_DURN__REG, &v_data_u8,
			BMP280_GEN_READ_WRITE_DATA_LENGTH);
	v_data_u8 = BMP280_SET_BITSLICE(v_data_u8,
				BMP280_CONFIG_REG_STANDBY_DURN,
				v_standby_durn_u8);
	i2c_.write(dev_addr_,
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

	i2c_.read(dev_addr_,
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
	i2c_.write(dev_addr_,
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
		i2c_.read(dev_addr_,
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
 * @brief
 *	This API reads the data from
 *	the given register
 *
 *
 *	@param v_addr_u8 -> Address of the register
 *	@param v_data_u8 -> The data from the register
 *	@param v_len_u8 -> no of bytes to read
 *
 *
 */
u8 BMP280::read_register(u8 v_addr_u8, u8 *v_data_u8, u8 v_len_u8)
{
	i2c_.read(dev_addr_, v_addr_u8, v_data_u8, v_len_u8);
	return v_len_u8;
}

/*!
 * @brief
 *	This API write the data to
 *	the given register
 *
 *
 *	@param v_addr_u8 -> Address of the register
 *	@param v_data_u8 -> The data for the register
 *	@param v_len_u8 -> no of bytes to write
 *
 *
 */
u8 BMP280::write_register(u8 v_addr_u8, u8 *v_data_u8, u8 v_len_u8)
{
	i2c_.write(dev_addr_, v_addr_u8, v_data_u8, v_len_u8);
	return v_len_u8;
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
	i2c_.read(dev_addr_,
				BMP280_CONFIG_REG_FILTER__REG, &v_data_u8,
				BMP280_GEN_READ_WRITE_DATA_LENGTH);
	v_data_u8 = BMP280_SET_BITSLICE(v_data_u8, BMP280_CONFIG_REG_FILTER, v_value_u8);
	i2c_.write(dev_addr_,
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
	i2c_.read(dev_addr_,
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
	i2c_.read(dev_addr_, BMP280_PRESSURE_MSB_REG, a_data_u8, BMP280_PRESSURE_DATA_LENGTH);
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

