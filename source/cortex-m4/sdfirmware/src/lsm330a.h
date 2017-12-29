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


#ifndef LSM330A_H_
#define LSM330A_H_

#include <stdint.h>
#include "spimaster.h"
#include "d3math.h"

class LSM330A {
protected:
	SPIMaster& spi_;
	uint16_t cs_;
	std::vector<GPIOPin> pins_;

public:
	typedef struct {
		int16_t AXIS_X;
		int16_t AXIS_Y;
		int16_t AXIS_Z;
	} AxesRaw_t;

	typedef struct {
		float AXIS_X;
		float AXIS_Y;
		float AXIS_Z;
	} AxesAcc_t;

	typedef enum {
		MEMS_SET            = 0x01,
		MEMS_RESET          = 0x00
	} Reset_t;

	typedef enum {
		MEMS_ENABLE			= 0x01,
		MEMS_DISABLE		= 0x00
	} State_t;

	typedef enum {
		ODR_POWERDOWN		= 0x00,
		ODR_3_125Hz			= 0x10,
		ODR_6_25Hz			= 0x20,
		ODR_12_5Hz			= 0x30,
		ODR_25Hz			= 0x40,
		ODR_50Hz			= 0x50,
		ODR_100Hz			= 0x60,
		ODR_400Hz			= 0x70,
		ODR_800Hz			= 0x80,
		ODR_1600Hz			= 0x90
	} ODR_t;

	typedef enum {
		FULLSCALE_2         = 0x00,
		FULLSCALE_4         = 0x08,
		FULLSCALE_6         = 0x10,
		FULLSCALE_8         = 0x18,
		FULLSCALE_16        = 0x20
	} Fullscale_t;

	typedef enum {
		AXIS_NONE			= 0,
		AXIS_X				= 1,
		AXIS_Y				= 2,
		AXIS_Z				= 4,
		AXIS_XYZ			= 7,
	} Axis_t;

	typedef enum {
		ABW_800				= 0x00,
		ABW_200				= 0x40,
		ABW_400				= 0x80,
		ABW_50				= 0xC0,
	} AntialiasFilterBandwidth_t;

	typedef enum {
		FIFO_BYPASS_MODE    = 0x00,
		FIFO_MODE           = 0x20,
		FIFO_STREAM_MODE    = 0x40,
		FIFO_STREAM_TO_FIFO = 0x60,
		FIFO_BYPASS_TO_STREAM = 0x80,
	} FifoMode_t;

	typedef enum {
		FIFO_SOURCE_WTM     = 0x80,
		FIFO_SOURCE_OVRN    = 0x40,
		FIFO_SOURCE_EMPTY   = 0x20,
	} FifoSrc_t;

	typedef enum {
		STATUS_ZYXOR		= 0x80,
		STATUS_ZOR 			= 0x40,
		STATUS_YOR 			= 0x20,
		STATUS_XOR 			= 0x10,
		STATUS_ZYXDA		= 0x08,
		STATUS_ZDA 			= 0x04,
		STATUS_YDA 			= 0x02,
		STATUS_XDA 			= 0x01,
	} Status_t;

public:
	LSM330A(SPIMaster& spi, uint8_t cs, const std::vector<GPIOPin>& pins = {}, const Matrix3f& axes_align = {1,0,0, 0,1,0, 0,0,1});
	~LSM330A();
	uint8_t ReadReg8(uint8_t reg);
	void ReadReg8(uint8_t reg, uint8_t* data);
	void WriteReg8(uint8_t reg, uint8_t data);
	void ReadData(uint8_t reg, uint8_t* data, uint16_t nbytes);
	void WriteData(uint8_t reg, uint8_t *data, uint16_t nbytes);
	void UpdateReg(uint8_t reg, uint8_t mask, uint8_t data);

	uint8_t GetDeviceID();
	//Sensor Configuration Functions
	void SetODR(ODR_t ov);
	void SetAxis(Axis_t axis);
	void SetFullScale(Fullscale_t fs);
	float GetFullScale();
	void SetBDU(State_t bdu);
	void Reboot();

	//Filtering Functions
	void SetAntiAliasingBandwidth(AntialiasFilterBandwidth_t abw);

	//FIFO Functions
	void FIFOModeEnable(State_t enable);
	void FIFOModeSet(FifoMode_t fm);
	void FIFOWatermarkEnable(State_t enable);
	void FIFOEmptyInterruptEnableP1(State_t enable);
	void FIFOWatermarkInterruptEnableP1(State_t enable);
	void FIFOOverrunInterruptEnableP1(State_t enable);
	uint8_t GetFifoSourceReg();
	bool GetFifoSourceBit(FifoSrc_t statusBIT);
	uint8_t GetFifoSourceFSS();

	uint8_t GetStatusReg();
	bool GetStatusBit(Status_t statusBIT);

	void GetAccAxesRaw(AxesRaw_t* buff);
	void GetAcc(AxesAcc_t* buff);
	uint8_t GetFifoAcc(AxesAcc_t* buff);
	Vector3f GetAccSample();

	void SetAlignment(const Matrix3f& axes_align);

protected:
	Matrix3f axes_align_;
};


#endif
