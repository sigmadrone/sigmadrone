/*
 * bmp180-i2c.cpp
 *
 *  Created on: Jul 28, 2015
 *      Author: Martin Stoilov
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bmp180.h"


#define BMP085_CHIP_ID					0x55
#define BMP085_CALIBRATION_DATA_START	0xAA
#define BMP085_CALIBRATION_DATA_LENGTH	11	/* 16 bit values */
#define BMP085_CHIP_ID_REG				0xD0
#define BMP085_CTRL_REG					0xF4
#define BMP085_TEMP_MEASUREMENT			0x2E
#define BMP085_PRESSURE_MEASUREMENT		0x34
#define BMP085_CONVERSION_REGISTER_MSB	0xF6
#define BMP085_CONVERSION_REGISTER_LSB	0xF7
#define BMP085_CONVERSION_REGISTER_XLSB	0xF8
#define BMP085_TEMP_CONVERSION_TIME		5
#define BMP180_I2C_ADDR					(0xEE)


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

BMP180::BMP180(I2CMaster& i2c)
	: i2c_(i2c)
{
	memset((struct bmp085_data*)this, 0, sizeof(struct bmp085_data));
	read_calibration_data();
}

BMP180::~BMP180() {
	// TODO Auto-generated destructor stub
}
void BMP180::read_calibration_data()
{
	u16 tmp[BMP085_CALIBRATION_DATA_LENGTH];
	struct bmp085_data *data = (struct bmp085_data *)this;
	struct bmp085_calibration_data *cali = &(data->calibration);
	i2c_.read(BMP180_I2C_ADDR, BMP085_CALIBRATION_DATA_START, (u8 *)tmp, (BMP085_CALIBRATION_DATA_LENGTH << 1));

	cali->AC1 =  be16_to_cpu(tmp[0]);
	cali->AC2 =  be16_to_cpu(tmp[1]);
	cali->AC3 =  be16_to_cpu(tmp[2]);
	cali->AC4 =  be16_to_cpu(tmp[3]);
	cali->AC5 =  be16_to_cpu(tmp[4]);
	cali->AC6 = be16_to_cpu(tmp[5]);
	cali->B1 = be16_to_cpu(tmp[6]);
	cali->B2 = be16_to_cpu(tmp[7]);
	cali->MB = be16_to_cpu(tmp[8]);
	cali->MC = be16_to_cpu(tmp[9]);
	cali->MD = be16_to_cpu(tmp[10]);
}

void  BMP180::update_raw_pressure()
{
	u32 tmp = 0;
	struct bmp085_data *data = (struct bmp085_data *)this;

	i2c_.write8(BMP180_I2C_ADDR, BMP085_CTRL_REG, BMP085_PRESSURE_MEASUREMENT + (data->oversampling_setting << 6));

	/* wait for the end of conversion */
	delay_msec(1 + (2 + (3 << data->oversampling_setting)));
	/* copy data into a u32 (4 bytes), but skip the first byte. */
	i2c_.read(BMP180_I2C_ADDR, BMP085_CONVERSION_REGISTER_MSB, ((u8 *)&tmp)+1, 3);
	data->raw_pressure = be32_to_cpu((tmp));
	data->raw_pressure >>= (8-data->oversampling_setting);
}

void BMP180::update_raw_temperature()
{
	u16 tmp;
	struct bmp085_data *data = (struct bmp085_data *)this;

	i2c_.write8(BMP180_I2C_ADDR, BMP085_CTRL_REG, BMP085_TEMP_MEASUREMENT);
	delay_msec(1 + (BMP085_TEMP_CONVERSION_TIME));

	i2c_.read(BMP180_I2C_ADDR, BMP085_CONVERSION_REGISTER_MSB, (u8*)&tmp, sizeof(tmp));
	data->raw_temperature = be16_to_cpu(tmp);
	data->last_temp_measurement = HAL_GetTick();
}

/*
 * This function starts the temperature measurement and returns the value
 * in tenth of a degree celsius.
 */
void BMP180::update_temperature()
{
	struct bmp085_data *data = (struct bmp085_data *)this;
	struct bmp085_calibration_data *cali = &data->calibration;
	long x1, x2;

	update_raw_temperature();
	x1 = ((data->raw_temperature - cali->AC6) * cali->AC5) >> 15;
	x2 = (cali->MC << 11) / (x1 + cali->MD);
	data->b6 = x1 + x2 - 4000;
	/* if NULL just update b6. Used for pressure only measurements */
	data->temperature = ((x1+x2+8) >> 4) / 10.0;
}

/*
 * This function starts the pressure measurement and returns the value
 * in millibar. Since the pressure depends on the ambient temperature,
 * a temperature measurement is executed according to the given temperature
 * measurement period (default is 1 sec boundary). This period could vary
 * and needs to be adjusted according to the sensor environment, i.e. if big
 * temperature variations then the temperature needs to be read out often.
 */
void BMP180::update_pressure()
{
	struct bmp085_data *data = (struct bmp085_data *)this;
	struct bmp085_calibration_data *cali = &data->calibration;
	s32 x1, x2, x3, b3;
	u32 b4, b7;
	s32 p;

	/* alt least every second force an update of the ambient temperature */
	if ((data->last_temp_measurement == 0) ||
			data->last_temp_measurement > HAL_GetTick() ||
			data->last_temp_measurement + 1000 < HAL_GetTick()) {
		update_temperature();
	}

	update_raw_pressure();
	x1 = (data->b6 * data->b6) >> 12;
	x1 *= cali->B2;
	x1 >>= 11;

	x2 = cali->AC2 * data->b6;
	x2 >>= 11;

	x3 = x1 + x2;

	b3 = (((((s32)cali->AC1) * 4 + x3) << data->oversampling_setting) + 2);
	b3 >>= 2;

	x1 = (cali->AC3 * data->b6) >> 13;
	x2 = (cali->B1 * ((data->b6 * data->b6) >> 12)) >> 16;
	x3 = (x1 + x2 + 2) >> 2;
	b4 = (cali->AC4 * (u32)(x3 + 32768)) >> 15;

	b7 = ((u32)data->raw_pressure - b3) * (50000 >> data->oversampling_setting);
	p = ((b7 < 0x80000000) ? ((b7 << 1) / b4) : ((b7 / b4) * 2));
	x1 = p >> 8;
	x1 *= x1;
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	p += (x1 + x2 + 3791) >> 4;

	data->pressure = p;
}

float BMP180::get_pressure()
{
	return pressure;
}

float BMP180::get_temperature()
{
	return temperature;
}

/*
 * This function sets the chip-internal oversampling. Valid values are 0..3.
 * The chip will use 2^oversampling samples for internal averaging.
 * This influences the measurement time and the accuracy; larger values
 * increase both. The datasheet gives an overview on how measurement time,
 * accuracy and noise correlate.
 */
void BMP180::set_oversampling(unsigned char oversampling)
{
	if (oversampling > 3)
		oversampling = 3;
	oversampling_setting = oversampling;
}

/*
 * Returns the currently selected oversampling. Range: 0..3
 */
unsigned char BMP180::get_oversampling()
{
	return oversampling_setting;
}
