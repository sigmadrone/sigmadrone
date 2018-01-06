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


#ifndef LSM330G_H_
#define LSM330G_H_

#include <stdint.h>
#include "spimaster.h"
#include "d3math.h"

//these could change accordingly with the architecture
#ifndef __ARCHDEP__TYPES
#define __ARCHDEP__TYPES
typedef unsigned char u8_t;
typedef unsigned short int u16_t;
typedef short int i16_t;
typedef signed char i8_t;
#endif /*__ARCHDEP__TYPES*/

typedef u8_t LSM330G_Int1PinConf_t;
typedef u8_t LSM330G_Int2PinConf_t;
typedef u8_t LSM330G_Int1Conf_t;
typedef u8_t LSM330G_Axis_t;

#ifndef ValBit
#define ValBit(VAR,Place)         (VAR & (1<<Place))
#endif
#ifndef BIT
#define BIT(x) ( (x) )
#endif

#define LSM330G_WHO_AM_I_ADDR                            0x0F

#define LSM330G_CTRL_REG1                                0x20
#define LSM330G_ODR_BIT                                  BIT(4)
#define LSM330G_PD                                       BIT(3)
#define LSM330G_ZEN                                      BIT(2)
#define LSM330G_YEN                                      BIT(1)
#define LSM330G_XEN                                      BIT(0)
#define LSM330G_X_ENABLE                                 0x01
#define LSM330G_Y_ENABLE                                 0x02
#define LSM330G_Z_ENABLE                                 0x04


/***************CTRL2***************/
#define LSM330G_CTRL_REG2                                0x21
#define LSM330G_EXTEN                                    BIT(7)
#define LSM330G_LVLEN                                    BIT(6)
#define LSM330G_HPM1                                     BIT(5)
#define LSM330G_HPM0                                     BIT(4)
#define LSM330G_HPM                                      BIT(4)
#define LSM330G_HPFC3                                    BIT(3)
#define LSM330G_HPFC2                                    BIT(2)
#define LSM330G_HPFC1                                    BIT(1)
#define LSM330G_HPFC0                                    BIT(0)


/***************CTRL3***************/
#define LSM330G_CTRL_REG3                                0x22
#define LSM330G_I1_INT                                   BIT(7)
#define LSM330G_I1_BOOT                                  BIT(6)
#define LSM330G_H_LACTIVE                                BIT(5)
#define LSM330G_PP_OD                                    BIT(4)
#define LSM330G_I2_DRDY                                  BIT(3)
#define LSM330G_I2_WTM                                   BIT(2)
#define LSM330G_I2_ORUN                                  BIT(1)
#define LSM330G_I2_EMPTY                                 BIT(0)
#define LSM330G_I1_ON_PIN_INT1_ENABLE                    0x80
#define LSM330G_I1_ON_PIN_INT1_DISABLE                   0x00
#define LSM330G_I1_BOOT_ON_INT1_ENABLE                   0x40
#define LSM330G_I1_BOOT_ON_INT1_DISABLE                  0x00
#define LSM330G_INT1_ACTIVE_HIGH                         0x00
#define LSM330G_INT1_ACTIVE_LOW                          0x20
#define LSM330G_I2_DRDY_ON_INT2_ENABLE                   0x08
#define LSM330G_I2_DRDY_ON_INT2_DISABLE                  0x00
#define LSM330G_WTM_ON_INT2_ENABLE                       0x04
#define LSM330G_WTM_ON_INT2_DISABLE                      0x00
#define LSM330G_OVERRUN_ON_INT2_ENABLE                   0x02
#define LSM330G_OVERRUN_ON_INT2_DISABLE                  0x00
#define LSM330G_EMPTY_ON_INT2_ENABLE                     0x01
#define LSM330G_EMPTY_ON_INT2_DISABLE                    0x00


/***************CTRL4***************/
#define LSM330G_CTRL_REG4                                0x23
#define LSM330G_BDU                                      BIT(7)
#define LSM330G_BLE                                      BIT(6)
#define LSM330G_FS                                       BIT(4)
#define LSM330G_SIM                                      BIT(0)

/***************CTRL5***************/
#define LSM330G_CTRL_REG5                               0x24
#define LSM330G_BOOT                                    BIT(7)
#define LSM330G_FIFO_EN                                 BIT(6)
#define LSM330G_HPEN                                    BIT(4)
#define LSM330G_INT1_SEL1                               BIT(3)
#define LSM330G_INT1_SEL0                               BIT(2)
#define LSM330G_OUT_SEL1                                BIT(1)
#define LSM330G_OUT_SEL0                                BIT(0)

#define LSM330G_REFERENCE                               0x25

#define LSM330G_OUT_TEMP                                0x26

#define LSM330G_STATUS_REG                              0x27
#define LSM330G_STATUS_REG_ZYXOR                        0x07	// 1	:	new data set has over written the previous one
																// 0	:	no overrun has occurred (default)
#define LSM330G_STATUS_REG_ZOR                          0x06	// 0	:	no overrun has occurred (default)
																// 1	:	new Z-axis data has over written the previous one
#define LSM330G_STATUS_REG_YOR                          0x05	// 0	:	no overrun has occurred (default)
																// 1	:	new Y-axis data has over written the previous one
#define LSM330G_STATUS_REG_XOR                          0x04	// 0	:	no overrun has occurred (default)
																// 1	:	new X-axis data has over written the previous one
#define LSM330G_STATUS_REG_ZYXDA                        0x03	// 0	:	a new set of data is not yet avvious one
                                                        		// 1	:	a new set of data is available
#define LSM330G_STATUS_REG_ZDA                          0x02	// 0	:	a new data for the Z-Axis is not availvious one
                                                        		// 1	:	a new data for the Z-Axis is available
#define LSM330G_STATUS_REG_YDA                          0x01	// 0	:	a new data for the Y-Axis is not available
																// 1	:	a new data for the Y-Axis is available
#define LSM330G_STATUS_REG_XDA                          0x00	// 0	:	a new data for the X-Axis is not available

#define LSM330G_DATAREADY_BIT                           LSM330G_STATUS_REG_ZYXDA


//OUTPUT REGISTER
#define LSM330G_OUT_X_L                                 0x28
#define LSM330G_OUT_X_H                                 0x29
#define LSM330G_OUT_Y_L                                 0x2A
#define LSM330G_OUT_Y_H                                 0x2B
#define LSM330G_OUT_Z_L                                 0x2C
#define LSM330G_OUT_Z_H                                 0x2D


/*************GYROSCOPE FIFO CONTROL REGISTER**************/
#define LSM330G_FIFO_CTRL_REG                           0x2E
#define LSM330G_FM0                                     BIT(5)

/*************GYROSCOPE FIFO SOURCE CONTROL REGISTER**************/
#define LSM330G_FIFO_SRC_REG                            0x2F

/**************GYROSCOPE INTERRUPT REGISTERS***************/
#define LSM330G_INT1_CFG                                0x30
#define LSM330G_INT1_SRC                                0x31
#define LSM330G_INT1_THS_XH                             0x32
#define LSM330G_INT1_THS_XL                             0x33
#define LSM330G_INT1_THS_YH                             0x34
#define LSM330G_INT1_THS_YL                             0x35
#define LSM330G_INT1_THS_ZH                             0x36
#define LSM330G_INT1_THS_ZL                             0x37
#define LSM330G_INT1_DURATION                           0x38
#define LSM330G_LIR                                     BIT(6)
#define LSM330G_ANDOR                                   BIT(7)
#define LSM330G_ZHIE                                    BIT(5)
#define LSM330G_ZLIE                                    BIT(4)
#define LSM330G_YHIE                                    BIT(3)
#define LSM330G_YLIE                                    BIT(2)
#define LSM330G_XHIE                                    BIT(1)
#define LSM330G_XLIE                                    BIT(0)
#define LSM330G_INT1_AND                                0x80
#define LSM330G_INT1_OR                                 0x00
#define LSM330G_INT1_LIR_ENABLE                         0x40
#define LSM330G_INT1_LIR_DISABLE                        0x00
#define LSM330G_INT1_ZHIE_ENABLE                        0x20
#define LSM330G_INT1_ZHIE_DISABLE                       0x00
#define LSM330G_INT1_ZLIE_ENABLE                        0x10
#define LSM330G_INT1_ZLIE_DISABLE                       0x00
#define LSM330G_INT1_YHIE_ENABLE                        0x08
#define LSM330G_INT1_YHIE_DISABLE                       0x00
#define LSM330G_INT1_YLIE_ENABLE                        0x04
#define LSM330G_INT1_YLIE_DISABLE                       0x00
#define LSM330G_INT1_XHIE_ENABLE                        0x02
#define LSM330G_INT1_XHIE_DISABLE                       0x00
#define LSM330G_INT1_XLIE_ENABLE                        0x01
#define LSM330G_INT1_XLIE_DISABLE                       0x00

class LSM330G {
protected:
	SPIMaster& spi_;
	u8_t cs_;
	std::vector<GPIOPin> pins_;

public:
	typedef enum {
		MEMS_SET            = 0x01,
		MEMS_RESET          = 0x00
	} Reset_t;

	typedef enum {
		MEMS_ENABLE         = 0x01,
		MEMS_DISABLE        = 0x00
	} State_t;

	typedef enum {
		ODR_95Hz_BW_12_5              =               0x00,
		ODR_95Hz_BW_25                =               0x01,
		ODR_190Hz_BW_12_5             =               0x04,
		ODR_190Hz_BW_25               =               0x05,
		ODR_190Hz_BW_50               =               0x06,
		ODR_190Hz_BW_70               =               0x07,
		ODR_380Hz_BW_20               =               0x08,
		ODR_380Hz_BW_25               =               0x09,
		ODR_380Hz_BW_50               =               0x0A,
		ODR_380Hz_BW_100              =               0x0B,
		ODR_760Hz_BW_30               =               0x0C,
		ODR_760Hz_BW_35               =               0x0D,
		ODR_760Hz_BW_50               =               0x0E,
		ODR_760Hz_BW_100              =               0x0F
	} ODR_t;

	typedef enum {
		POWER_DOWN                    =               0x00,
		SLEEP                         =               0x01,
		NORMAL                        =               0x02
	} Mode_t;

	typedef enum {
		HPM_NORMAL_MODE_RES           =               0x00,
		HPM_REF_SIGNAL                =               0x01,
		HPM_NORMAL_MODE               =               0x02,
		HPM_AUTORESET_INT             =               0x03
	} HPFMode_t;

	typedef enum {
		HPFCF_0                       =               0x00,
		HPFCF_1                       =               0x01,
		HPFCF_2                       = 	          0x02,
		HPFCF_3                       =               0x03,
		HPFCF_4                       =               0x04,
		HPFCF_5                       =               0x05,
		HPFCF_6                       =               0x06,
		HPFCF_7                       =               0x07,
		HPFCF_8                       =               0x08,
		HPFCF_9                       =               0x09
	} HPFCutOffFreq_t;

	typedef enum {
		PUSH_PULL                     =                0x00,
		OPEN_DRAIN                    =                0x01
	} IntPinMode_t;

	typedef enum {
		FULLSCALE_250                 =                0x00,
		FULLSCALE_500                 =                0x01,
		FULLSCALE_2000                =                0x02
	} Fullscale_t;

	typedef enum {
		BLE_LSB                       =                0x00,
		BLE_MSB                       =                0x01
	} Endianess_t;

	typedef enum {
		SPI_4_WIRE                    =                0x00,
		SPI_3_WIRE                    =                0x01
	} SPIMode_t;

	typedef enum {
		FIFO_DISABLE                  =                0x05,
		FIFO_BYPASS_MODE              =                0x00,
		FIFO_MODE                     =                0x01,
		FIFO_STREAM_MODE              =                0x02,
		FIFO_STREAM_TO_FIFO_MODE      =                0x03,
		FIFO_BYPASS_TO_STREAM_MODE    =                0x04
	} FifoMode_t;

	typedef enum {
		HPF_NONE                      =                0x00,
		HPF                           =                0x01,
		LPF2                          =                0x02,
		HPFLPF2                       =                0x03
	} HPF_LPF2_Enable;

	typedef enum {
		THS_X                         =                0x00,
		THS_Y                         =                0x01,
		THS_Z                         =                0x02
	} IntThsAxis;

	typedef struct {
		i16_t AXIS_X;
		i16_t AXIS_Y;
		i16_t AXIS_Z;
	} AxesRaw_t;

	typedef struct {
		float AXIS_X;
		float AXIS_Y;
		float AXIS_Z;
	} AxesDPS_t;

public:
	AxesRaw_t raw;

public:
	LSM330G(SPIMaster& spi, u8_t cs, const std::vector<GPIOPin>& pins = {});
	~LSM330G();
	u8_t ReadReg8(u8_t reg);
	void ReadReg8(u8_t reg, u8_t* data);
	void WriteReg8(u8_t reg, u8_t data);
	void ReadData(u8_t reg, u8_t* data, u16_t nbytes);
	void WriteData(u8_t reg, u8_t *data, u16_t nbytes);

	/**********Sensor Configuration Functions***********/
	void SetODR(ODR_t ov);
	void SetMode(Mode_t md);
	void SetAxis(LSM330G_Axis_t axis);
	void SetFullScale(Fullscale_t fs);
	float GetFullScale();
	float GetGain();
	void SetBDU(State_t bdu);
	void SetBLE(Endianess_t ble);
	void SetSPIInterface(SPIMode_t spi);
	void Reboot();

	/***************Filtering Functions****************/
	void SetHPFMode(HPFMode_t hpf);
	void SetHPFCutOFF(HPFCutOffFreq_t hpf);
	void HPFEnable(State_t hpf);
	void SetOutputDataAndFifoFilters(HPF_LPF2_Enable hpf);
	void SetInt1Filters(HPF_LPF2_Enable hpf);

	/***************Interrupt Functions****************/
	void SetIntPinMode(IntPinMode_t pm);
	void SetInt1Pin(LSM330G_Int1PinConf_t pinConf);
	void SetInt2Pin(LSM330G_Int2PinConf_t pinConf);
	void Int1LatchEnable(State_t latch);
	void ResetInt1Latch(void);
	void SetIntConfiguration(LSM330G_Int1Conf_t ic);
	void SetInt1Threshold(IntThsAxis axis, u16_t ths);
	void SetInt1Duration(LSM330G_Int1Conf_t id);

	/*****************FIFO Functions******************/
	void FIFOModeEnable(FifoMode_t fm);
	void SetWaterMark(u8_t wtm);

	/****************Reading Functions*****************/
	u8_t GetSatusReg();
	void GetAngRateRaw(AxesRaw_t* buff);
	void GetAngRateDPS(AxesDPS_t* buff);
	void GetFifoAngRateDPS(AxesDPS_t* buff);
	u8_t GetFifoSourceReg();
	u8_t GetFifoSourceFSS();
	u8_t GetInt1Src();
	u8_t GetDeviceID();
	void SetAlignment(const Matrix3f& axes_align);
	Vector3f GetSample();

protected:
	Matrix3f axes_align_;

};


#endif
