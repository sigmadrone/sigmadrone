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

#include <stdint.h>
#include "spimaster.h"


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
	BMP280(SPIMaster& spi, uint8_t cs);
	~BMP280();

public:
	void read_reg(uint8_t reg, uint8_t* data, uint16_t nbytes);
	void write_reg(uint8_t reg, uint8_t* data, uint16_t nbytes);

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

	void set_filter(u8 v_value_u8);
	void set_oversamp_pressure(u8 v_value_u8);
	void set_oversamp_temperature(u8 v_value_u8);
	void set_power_mode(u8 v_power_mode_u8);
	void set_standby_durn(u8 v_standby_durn_u8);
	void set_work_mode(u8 v_work_mode_u8);
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
	SPIMaster& spi_;
	u8 cs_;				        /**< chip select*/
	u8 chip_id_;
	u8 oversamp_temperature_;	/**< temperature over sampling*/
	u8 oversamp_pressure_;		/**< pressure over sampling*/
	struct calib_param_t calib_;
};

#endif /* BMP280_H_ */
