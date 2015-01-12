/*
 * lsm303dlhc.cpp
 *
 *  Created on: Nov 28, 2014
 *      Author: mstoilov
 */

#include <stdexcept>
#include <stdio.h>
#include "lsm303d.h"

/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80)
/* Multiple byte read/write command */
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)

LSM303D::LSM303D(SPIMaster& spi, u8_t cs, const std::vector<GPIOPin>& pins)
	: spi_(spi)
	, cs_(cs)
	, pins_(pins)
{
	for (auto& pin : pins_)
		pin.init();
//	configure();
}

LSM303D::~LSM303D()
{
	// TODO Auto-generated destructor stub
}

void LSM303D::update_data()
{
	AxesRaw_t temp;
	int32_t x = 0, y = 0, z = 0;
	int32_t samples = GetFifoSourceFSS();

	for (int32_t i = 0; i < samples; i++) {
		GetAccAxesRaw(&temp);
		x += temp.AXIS_X;
		y += temp.AXIS_Y;
		z += temp.AXIS_Z;
	}
	if (samples) {
		x /= samples;
		y /= samples;
		z /= samples;
	}

	raw.AXIS_X = x;
	raw.AXIS_Y = y;
	raw.AXIS_Z = z;
}

void LSM303D::configure()
{
	SetODR_M(ODR_100Hz_M);
	SetTemperature(MEMS_ENABLE);
	SetFullScaleMag(FULLSCALE_2_GA);
	SetModeMag(CONTINUOUS_MODE);

	SetODR(ODR_50Hz);
	SetAxis(X_ENABLE | Y_ENABLE | Z_ENABLE);
	SetFullScale(FULLSCALE_2);
	FIFOModeSet(FIFO_BYPASS_MODE);
	SetBDU(MEMS_ENABLE);

	SetHPFMode(HPM_NORMAL_MODE);
	SetHPFCutOFF(HPFCF_1);
}

void LSM303D::acc_regs_dump()
{
//	printf("LSM303D_CTRL_REG1_A: 0x%x\n", i2cdev_.read8(accaddress_, CTRL_REG1_A));
//	printf("LSM303D_CTRL_REG2_A: 0x%x\n", i2cdev_.read8(accaddress_, CTRL_REG2_A));
//	printf("LSM303D_CTRL_REG3_A: 0x%x\n", i2cdev_.read8(accaddress_, CTRL_REG3_A));
//	printf("LSM303D_CTRL_REG4_A: 0x%x\n", i2cdev_.read8(accaddress_, CTRL_REG4_A));
//	printf("LSM303D_CTRL_REG5_A: 0x%x\n", i2cdev_.read8(accaddress_, CTRL_REG5_A));
//	printf("LSM303D_CTRL_REG6_A: 0x%x\n", i2cdev_.read8(accaddress_, CTRL_REG6_A));
}


void LSM303D::ReadData(u8_t reg, u8_t* data, u16_t nbytes)
{
	/* Configure the MS bit:
	 - When 0, the address will remain unchanged in multiple read/write commands.
	 - When 1, the address will be auto incremented in multiple read/write commands.
	 */
	if (nbytes > 0x01) {
		reg |= (uint8_t) (READWRITE_CMD | MULTIPLEBYTE_CMD);
	} else {
		reg |= (uint8_t) READWRITE_CMD;
	}
	spi_.read(cs_, reg, data, nbytes);
}

void LSM303D::WriteData(u8_t reg, u8_t *data, u16_t nbytes)
{
	/* Configure the MS bit:
	 - When 0, the address will remain unchanged in multiple read/write commands.
	 - When 1, the address will be auto incremented in multiple read/write commands.
	 */
	if (nbytes > 0x01) {
		reg |= (uint8_t) MULTIPLEBYTE_CMD;
	}
	spi_.write(cs_, reg, data, nbytes);
}

void LSM303D::ReadReg8(u8_t reg, u8_t* data)
{
	ReadData(reg, data, 1);
}

u8_t LSM303D::ReadReg8(u8_t reg)
{
	u8_t ret = 0;
	ReadData(reg, &ret, 1);
	return ret;
}

void LSM303D::WriteReg8(u8_t reg, u8_t data)
{
	spi_.write(cs_, reg, &data, 1);
}

/*******************************************************************************
 * Function Name  : SetODR
 * Description    : Sets LSM303DLHC Output Data Rate Accelerometer
 * Input          : Output Data Rate
 * Output         : None
 * Return         : None
 *******************************************************************************/
void LSM303D::SetODR(ODR_t ov)
{
	u8_t value;

	ReadReg8(LSM303D_CTRL_REG1, &value);
	value &= 0x0f;
	value |= ov << LSM303D_ODR_BIT;
	WriteReg8(LSM303D_CTRL_REG1, value);
}

/*******************************************************************************
 * Function Name  : SetODR_M
 * Description    : Sets LSM303DLHC Output Data Rate Magnetometer
 * Input          : Output Data Rate
 * Output         : None
 * Return         : None
 *******************************************************************************/
void LSM303D::SetODR_M(ODR_M_t ov)
{
	u8_t value;

	ReadReg8(LSM303D_CTRL_REG5, &value);
	value &= 0xE3;
	value |= ov << LSM303D_M_ODR;
	WriteReg8(LSM303D_CTRL_REG5, value);
}

/*******************************************************************************
 * Function Name  : SetTemperature
 * Description    : Sets LSM303DLHC Output Temperature
 * Input          : MEMS_ENABLE, MEMS_DISABLE
 * Output         : None
 * Return         : None
 *******************************************************************************/
void LSM303D::SetTemperature(State_t state)
{
	u8_t value;

	ReadReg8(LSM303D_CTRL_REG5, &value);
	value &= 0x7F;
	value |= state << LSM303D_TEMP_EN;
	WriteReg8(LSM303D_CTRL_REG5, value);
}

void LSM303D::SetMagneticResolution(Resolution_t resolution)
{
	u8_t value;

	ReadReg8(LSM303D_CTRL_REG5, &value);
	value &= 0x9F;
	value |= resolution << LSM303D_M_RES;
	WriteReg8(LSM303D_CTRL_REG5, value);
}

/*******************************************************************************
 * Function Name  : SetModeMag
 * Description    : Sets LSM303DLHC Magnetometer Modality
 * Input          : Modality (CONTINUOUS_MODE, or SINGLE_MODE, or SLEEP_MODE)
 * Output         : None
 * Return         : None
 *******************************************************************************/
void LSM303D::SetModeMag(Mode_M_t Mode)
{
	u8_t value;

	ReadReg8(LSM303D_CTRL_REG7, &value);
	value &= 0xF4;
	value |= Mode << LSM303D_MD;
	WriteReg8(LSM303D_CTRL_REG7, value);
}

/*******************************************************************************
 * Function Name  : GetTempRaw
 * Description    : Read the Temperature Values by TEMP_OUT Output Registers
 * Input          : Value to empty (16 Bit two's complement)
 * Output         : None
 * Return         : None
 *******************************************************************************/
void LSM303D::GetTempRaw(i16_t* val)
{
	u8_t valueH, valueL;

	ReadReg8(LSM303D_TEMP_OUT_H_M, &valueH);
	ReadReg8(LSM303D_TEMP_OUT_L_M, &valueL);
	*val = ((i16_t) (valueH) << 8) | ((i16_t) (valueL));
}

/*******************************************************************************
 * Function Name  : SetAxis
 * Description    : Enable/Disable LSM303DLHC Axis
 * Input          : X_ENABLE/X_DISABLE | Y_ENABLE/Y_DISABLE | Z_ENABLE/Z_DISABLE
 * Output         : None
 * Note           : You MUST use all input variable in the argument, as example
 * Return         : None
 *******************************************************************************/
void LSM303D::SetAxis(Axis_t axis)
{
	u8_t value;

	ReadReg8(LSM303D_CTRL_REG1, &value);
	value &= 0xF8;
	value |= (0x07 & axis);
	WriteReg8(LSM303D_CTRL_REG1, value);
}

/*******************************************************************************
 * Function Name  : SetFullScale
 * Description    : Sets the LSM303DLHC FullScale
 * Input          : FULLSCALE_2/FULLSCALE_4/FULLSCALE_8/FULLSCALE_16
 * Output         : None
 * Return         : None
 *******************************************************************************/
void LSM303D::SetFullScale(Fullscale_t fs)
{
	u8_t value;

	ReadReg8(LSM303D_CTRL_REG2, &value);
	value &= 0xC3;
	value |= (fs << LSM303D_AFS);
	WriteReg8(LSM303D_CTRL_REG2, value);
}

float LSM303D::GetFullScale()
{
	u8_t value;

	ReadReg8(LSM303D_CTRL_REG2, &value);
	value >>= LSM303D_AFS;
	value &= 0x07;
	if (value == 0)
		return 2.0;
	else if (value == 1)
		return 4.0;
	else if (value == 2)
		return 6.0;
	else if (value == 3)
		return 8.0;
	return 16.0;
}

/*******************************************************************************
 * Function Name  : SetFullScaleMag
 * Description    : Sets the Magnetic LSM303DLHC FullScale
 * Input          : FULLSCALE_2_GA/FULLSCALE_4_GA/FULLSCALE_8_GA/FULLSCALE_12_GA
 * Output         : None
 * Return         : None
 *******************************************************************************/
void LSM303D::SetFullScaleMag(FullscaleMag_t fs)
{
	u8_t value = 0;

	value |= (fs << LSM303D_MFS) & 0x60;
	WriteReg8(LSM303D_CTRL_REG6, value);
}

/*******************************************************************************
 * Function Name  : SetBDU
 * Description    : Enable/Disable Block Data Update Functionality
 * Input          : ENABLE/DISABLE
 * Output         : None
 * Return         : None
 *******************************************************************************/
void LSM303D::SetBDU(State_t bdu)
{
	u8_t value;

	ReadReg8(LSM303D_CTRL_REG1, &value);
	value &= 0xF7;
	value |= (bdu << LSM303D_BDU);
	WriteReg8(LSM303D_CTRL_REG1, value);
}

/*******************************************************************************
 * Function Name  : SetSelfTest
 * Description    : Set Self Test Modality
 * Input          : SELF_TEST_DISABLE/ST_0
 * Output         : None
 * Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
 *******************************************************************************/
void LSM303D::SetSelfTest(SelfTest_t st)
{
	u8_t value;

	ReadReg8(LSM303D_CTRL_REG2, &value);
	value &= 0xF9;
	value |= (st << LSM303D_AST);
	WriteReg8(LSM303D_CTRL_REG2, value);
}

/*******************************************************************************
 * Function Name  : SetHPFMode
 * Description    : Set High Pass Filter Modality
 * Input          : HPM_NORMAL_MODE_RES/HPM_REF_SIGNAL/HPM_NORMAL_MODE/HPM_AUTORESET_INT
 * Output         : None
 * Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
 *******************************************************************************/
void LSM303D::SetHPFMode(HPFMode_t hpm)
{
	u8_t value;

	ReadReg8(LSM303D_CTRL_REG7, &value);
	value &= 0x3F;
	value |= (hpm << LSM303D_AHPM);
	WriteReg8(LSM303D_CTRL_REG7, value);
}

/*******************************************************************************
 * Function Name  : SetHPFCutOFF
 * Description    : Set High Pass CUT OFF Freq
 * Input          : HPFCF [0,3]
 * Output         : None
 * Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
 *******************************************************************************/
void LSM303D::SetHPFCutOFF(HPFCutOffFreq_t hpf)
{
}

/*******************************************************************************
 * Function Name  : SetFilterDataSel
 * Description    : Set Filter Data Selection bypassed or sent to FIFO OUT register
 * Input          : MEMS_SET, MEMS_RESET
 * Output         : None
 * Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
 *******************************************************************************/
void LSM303D::SetFilterDataSel(State_t state)
{
}

/*******************************************************************************
 * Function Name  : SetInt1Pin
 * Description    : Set Interrupt1 pin Function
 * Input          :   LSM303D_INT1_BOOT_ENABLE/DISABLE
 * 					| LSM303D_INT1_CLICK_ENABLE/DISABLE
 * 					| LSM303D_INT1_IG1_ENABLE/DISABLE
 * 					| LSM303D_INT1_IG2_ENABLE/DISABLE
 * 					| LSM303D_INT1_IGM_ENABLE/DISABLE
 * 					| LSM303D_INT1_DRDY_A_ENABLE/DISABLE
 * 					| LSM303D_INT1_DRDY_M_ENABLE/DISABLE
 * 					| LSM303D_INT1_EMPTY_ENABLE/DISABLE
 *
 * Note           : To enable Interrupt signals on INT1 Pad (You MUST use all input variable in the argument, as example)
 * Output         : None
 * Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
 *******************************************************************************/
void LSM303D::SetInt1Pin(IntPinConf_t pinConf)
{
	u8_t value;

	ReadReg8(LSM303D_CTRL_REG3, &value);
	value &= 0x00;
	value |= pinConf;
	WriteReg8(LSM303D_CTRL_REG3, value);
}

/*******************************************************************************
 * Function Name  : SetInt2Pin
 * Description    : Set Interrupt2 pin Function
 * Input          :   LSM303D_INT2_CLICK_ENABLE/DISABLE
 * 					| LSM303D_INT2_IG1_ENABLE/DISABLE
 * 					| LSM303D_INT2_IG2_ENABLE/DISABLE
 * 					| LSM303D_INT2_IGM_ENABLE/DISABLE
 * 					| LSM303D_INT2_DRDY_A_ENABLE/DISABLE
 * 					| LSM303D_INT2_DRDY_M_ENABLE/DISABLE
 * 					| LSM303D_INT2_OVERRUN_ENABLE/DISABLE
 * 					| LSM303D_INT2_FTH_ENABLE/DISABLE
 *
 * Note           : To enable Interrupt signals on INT2 Pad (You MUST use all input variable in the argument, as example)
 * Output         : None
 * Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
 *******************************************************************************/
void LSM303D::SetInt2Pin(IntPinConf_t pinConf)
{
	u8_t value;

	ReadReg8(LSM303D_CTRL_REG4, &value);
	value &= 0x00;
	value |= pinConf;
	WriteReg8(LSM303D_CTRL_REG4, value);
}

/*******************************************************************************
 * Function Name  : Int1LatchEnable
 * Description    : Enable Interrupt 1 Latching function
 * Input          : ENABLE/DISABLE
 * Output         : None
 * Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
 *******************************************************************************/
void LSM303D::Int1LatchEnable(State_t latch)
{
	u8_t value;

	ReadReg8(LSM303D_CTRL_REG5, &value);
	value &= 0xFE;
	value |= latch << LSM303D_LIR1;
	WriteReg8(LSM303D_CTRL_REG5, value);
}

/*******************************************************************************
 * Function Name  : Int2LatchEnable
 * Description    : Enable Interrupt 2 Latching function
 * Input          : ENABLE/DISABLE
 * Output         : None
 * Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
 *******************************************************************************/
void LSM303D::Int2LatchEnable(State_t latch)
{
	u8_t value;

	ReadReg8(LSM303D_CTRL_REG5, &value);
	value &= 0xFD;
	value |= latch << LSM303D_LIR2;
	WriteReg8(LSM303D_CTRL_REG5, value);
}

void LSM303D::SetIGConfiguration(IntConf_t ic)
{
}

void LSM303D::FIFOModeEnable(State_t enable)
{
	u8_t value;
	ReadReg8(LSM303D_CTRL_REG0, &value);
	value &= 0xBF;
	value |= enable << LSM303D_FIFO_EN;
	WriteReg8(LSM303D_CTRL_REG0, value);
}

/*******************************************************************************
 * Function Name  : FIFOModeEnable
 * Description    : Sets Fifo Modality
 * Input          : FIFO_DISABLE, FIFO_BYPASS_MODE, FIFO_MODE, FIFO_STREAM_MODE, FIFO_TRIGGER_MODE
 * Output         : None
 * Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
 *******************************************************************************/
void LSM303D::FIFOModeSet(FifoMode_t fm)
{
	u8_t value;

	if (fm == FIFO_BYPASS_MODE) {
		FIFOModeEnable(MEMS_DISABLE);
	} else {
		FIFOModeEnable(MEMS_ENABLE);
	}
	ReadReg8(LSM303D_FIFO_CTRL_REG_A, &value);
	value &= 0x1f;
	value |= (fm << LSM303D_FM);
	WriteReg8(LSM303D_FIFO_CTRL_REG_A, value);
}

/*******************************************************************************
 * Function Name  : SetWaterMark
 * Description    : Sets Watermark Value
 * Input          : Watermark = [0,31]
 * Output         : None
 * Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
 *******************************************************************************/
void LSM303D::SetThreshold(u8_t ths)
{
	u8_t value;

	if (ths > 31)
		throw std::range_error("lsm303dlhc::SetWaterMark ths is invalid");
	ReadReg8(LSM303D_FIFO_CTRL_REG_A, &value);
	value &= 0xE0;
	value |= ths;
	WriteReg8(LSM303D_FIFO_CTRL_REG_A, value);
}

/*******************************************************************************
 * Function Name  : GetSatusReg
 * Description    : Read the status register
 * Input          : char to empity by Status Reg Value
 * Output         : None
 * Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
 *******************************************************************************/
void LSM303D::GetSatusReg(u8_t* val)
{
	ReadReg8(LSM303D_STATUS_REG_A, val);
}

/*******************************************************************************
 * Function Name  : GetSatusBIT
 * Description    : Read the status register BIT
 * Input          : LSM303D_STATUS_REG_ZYXOR, LSM303D_STATUS_REG_ZOR, LSM303D_STATUS_REG_YOR, LSM303D_STATUS_REG_XOR,
 LSM303D_STATUS_REG_ZYXDA, LSM303D_STATUS_REG_ZDA, LSM303D_STATUS_REG_YDA, LSM303D_STATUS_REG_XDA, DATAREADY_BIT
 * Output         : status register BIT
 * Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
 *******************************************************************************/
bool LSM303D::GetSatusBit(u8_t statusBIT)
{
	u8_t value;

	ReadReg8(LSM303D_STATUS_REG_A, &value);

	switch (statusBIT) {
	case LSM303D_STATUS_REG_ZYXOR:
		return (value &= LSM303D_STATUS_REG_ZYXOR);
	case LSM303D_STATUS_REG_ZOR:
		return (value &= LSM303D_STATUS_REG_ZOR);
	case LSM303D_STATUS_REG_YOR:
		return (value &= LSM303D_STATUS_REG_YOR);
	case LSM303D_STATUS_REG_XOR:
		return (value &= LSM303D_STATUS_REG_XOR);
	case LSM303D_STATUS_REG_ZYXDA:
		return (value &= LSM303D_STATUS_REG_ZYXDA);
	case LSM303D_STATUS_REG_ZDA:
		return (value &= LSM303D_STATUS_REG_ZDA);
	case LSM303D_STATUS_REG_YDA:
		return (value &= LSM303D_STATUS_REG_YDA);
	case LSM303D_STATUS_REG_XDA:
		return (value &= LSM303D_STATUS_REG_XDA);
	default:
		throw std::range_error("lsm303dlhc::GetSatusBit statusBIT is invalid");
	}
	return false;
}

/*******************************************************************************
 * Function Name  : GetAccAxesRaw
 * Description    : Read the Acceleration Values Output Registers
 * Input          : buffer to empity by AccAxesRaw_t Typedef
 * Output         : None
 * Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
 *******************************************************************************/
void LSM303D::GetAccAxesRaw(AxesRaw_t* buff)
{
	u8_t valueL;
	u8_t valueH;

	ReadReg8(LSM303D_OUT_X_L_A, &valueL);
	ReadReg8(LSM303D_OUT_X_H_A, &valueH);
	buff->AXIS_X = (i16_t) ((valueH << 8) | valueL);

	ReadReg8(LSM303D_OUT_Y_L_A, &valueL);
	ReadReg8(LSM303D_OUT_Y_H_A, &valueH);
	buff->AXIS_Y = (i16_t) ((valueH << 8) | valueL);

	ReadReg8(LSM303D_OUT_Z_L_A, &valueL);
	ReadReg8(LSM303D_OUT_Z_H_A, &valueH);
	buff->AXIS_Z = (i16_t) ((valueH << 8) | valueL);
}

void LSM303D::GetAcc(AxesAcc_t* buff)
{
	AxesRaw_t raw = {0, 0, 0};
	float fullscale = GetFullScale();
	GetAccAxesRaw(&raw);
	buff->AXIS_X = raw.AXIS_X * fullscale / 32768.0;
	buff->AXIS_Y = raw.AXIS_Y * fullscale / 32768.0;
	buff->AXIS_Z = raw.AXIS_Z * fullscale / 32768.0;
}

void LSM303D::GetFifoAcc(AxesAcc_t* buff)
{
	AxesRaw_t raw = {0, 0, 0};
	uint8_t count = GetFifoSourceFSS();
	float scale = GetFullScale() / 32768.0 / ((float)count);

	buff->AXIS_X = buff->AXIS_Y = buff->AXIS_Z = 0.0;
	for (uint8_t i = 0; i < count; i++) {
		GetAccAxesRaw(&raw);
		buff->AXIS_X += raw.AXIS_X * scale;
		buff->AXIS_Y += raw.AXIS_Y * scale;
		buff->AXIS_Z += raw.AXIS_Z * scale;
	}
}

/*******************************************************************************
 * Function Name  : GetMagAxesRaw
 * Description    : Read the Magnetometer Values Output Registers
 * Input          : buffer to empity by MagAxesRaw_t Typedef
 * Output         : None
 * Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
 *******************************************************************************/
void LSM303D::GetMagAxesRaw(AxesRaw_t* buff)
{
	u8_t valueL;
	u8_t valueH;

	ReadReg8(LSM303D_OUT_X_L_M, &valueL);
	ReadReg8(LSM303D_OUT_X_H_M, &valueH);
	buff->AXIS_X = (i16_t) ((valueH << 8) | valueL) / 16;

	ReadReg8(LSM303D_OUT_Y_L_M, &valueL);
	ReadReg8(LSM303D_OUT_Y_H_M, &valueH);
	buff->AXIS_Y = (i16_t) ((valueH << 8) | valueL) / 16;

	ReadReg8(LSM303D_OUT_Z_L_M, &valueL);
	ReadReg8(LSM303D_OUT_Z_H_M, &valueH);
	buff->AXIS_Z = (i16_t) ((valueH << 8) | valueL) / 16;
}


/*******************************************************************************
 * Function Name  : GetFifoSourceReg
 * Description    : Read Fifo source Register
 * Input          : Byte to empity by FIFO source register value
 * Output         : None
 * Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
 *******************************************************************************/
void LSM303D::GetFifoSourceReg(u8_t* val)
{
	ReadReg8(LSM303D_FIFO_SRC_REG_A, val);
}

/*******************************************************************************
 * Function Name  : GetFifoSourceBit
 * Description    : Read Fifo WaterMark source bit
 * Input          : FIFO_SRC_WTM, FIFO_SRC_OVRUN, FIFO_SRC_EMPTY
 * Output         : None
 * Return         : Status of BIT [MEMS_ERROR, MEMS_SUCCESS]
 *******************************************************************************/
bool LSM303D::GetFifoSourceBit(u8_t statusBIT)
{
	u8_t value;

	ReadReg8(LSM303D_FIFO_SRC_REG_A, &value);

	if (statusBIT == LSM303D_FIFO_SRC_FTH) {
		return (value &= LSM303D_FIFO_SRC_FTH);
	}
	if (statusBIT == LSM303D_FIFO_SRC_OVRUN) {
		return (value &= LSM303D_FIFO_SRC_OVRUN);
	}
	if (statusBIT == LSM303D_FIFO_SRC_EMPTY) {
		return (value &= LSM303D_FIFO_SRC_EMPTY);
	}
	throw std::range_error("lsm303dlhc::GetFifoSourceBit statusBIT is invalid");
	return false;
}

/*******************************************************************************
 * Function Name  : GetFifoSourceFSS
 * Description    : Read current number of unread samples stored in FIFO
 * Input          : Byte to empity by FIFO unread sample value
 * Output         : None
 * Return         : Status [value of FSS]
 *******************************************************************************/
u8_t LSM303D::GetFifoSourceFSS()
{
	u8_t value;

	ReadReg8(LSM303D_FIFO_SRC_REG_A, &value);
	value &= 0x1F;
	return value;
}
