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
#include <stdio.h>
#include "lsm330g.h"



/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80)
/* Multiple byte read/write command */
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)

LSM330G::LSM330G(SPIMaster& spi, u8_t cs, const std::vector<GPIOPin>& pins)
	: spi_(spi)
	, cs_(cs)
	, pins_(pins)
	, axes_align_{1,0,0, 0,1,0, 0,0,1}
{
	for (auto& pin : pins_)
		pin.init();
//	configure();
}

LSM330G::~LSM330G()
{
	// TODO Auto-generated destructor stub
}

void LSM330G::ReadData(u8_t reg, u8_t* data, u16_t nbytes)
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
	spi_.read_reg(cs_, reg, data, nbytes);
}

void LSM330G::WriteData(u8_t reg, u8_t *data, u16_t nbytes)
{
	/* Configure the MS bit:
	 - When 0, the address will remain unchanged in multiple read/write commands.
	 - When 1, the address will be auto incremented in multiple read/write commands.
	 */
	if (nbytes > 0x01) {
		reg |= (uint8_t) MULTIPLEBYTE_CMD;
	}
	spi_.write_reg(cs_, reg, data, nbytes);
}

void LSM330G::ReadReg8(u8_t reg, u8_t* data)
{
	ReadData(reg, data, 1);
}

u8_t LSM330G::ReadReg8(u8_t reg)
{
	u8_t ret = 0;
	ReadData(reg, &ret, 1);
	return ret;
}

void LSM330G::WriteReg8(u8_t reg, u8_t data)
{
	spi_.write_reg(cs_, reg, &data, 1);
}

/*******************************************************************************
* Function Name  : LSM330G::SetODR
* Description    : Sets LSM330G Output Data Rate
* Input          : Output Data Rate
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetODR(ODR_t ov)
{
	u8_t value;

	ReadReg8(LSM330G_CTRL_REG1, &value);
	value &= 0x0f;
	value |= ov << 4;
	WriteReg8(LSM330G_CTRL_REG1, value);
}


/*******************************************************************************
* Function Name  : LSM330G::SetMode
* Description    : Sets LSM330G Operating Mode
* Input          : Modality (NORMAL, SLEEP, POWER_DOWN)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetMode(Mode_t md)
{
	u8_t value;

	ReadReg8(LSM330G_CTRL_REG1, &value);
	switch (md) {

	case POWER_DOWN:
		value &= 0xF7;
		value |= (MEMS_RESET << LSM330G_PD);
		break;

	case NORMAL:
		value &= 0xF7;
		value |= (MEMS_SET << LSM330G_PD);
		break;

	case SLEEP:
		value &= 0xF0;
		value |= ((MEMS_SET << LSM330G_PD) | (MEMS_RESET << LSM330G_ZEN) | (MEMS_RESET << LSM330G_YEN)
				| (MEMS_RESET << LSM330G_XEN));
		break;

	default:
		throw std::invalid_argument("LSM330G::SetMode(Mode_t md), invalid md");
	}
	WriteReg8(LSM330G_CTRL_REG1, value);
}

/*******************************************************************************
* Function Name  : LSM330G::SetAxis
* Description    : Enable/Disable LSM330G Axis
* Input          : X_ENABLE/X_DISABLE | Y_ENABLE/Y_DISABLE | Z_ENABLE/Z_DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetAxis(LSM330G_Axis_t axis)
{
	u8_t value;

	ReadReg8(LSM330G_CTRL_REG1, &value);
	value &= 0xf8;
	value |= axis;
	WriteReg8(LSM330G_CTRL_REG1, value);
}

/*******************************************************************************
* Function Name  : LSM330G::SetFullScale
* Description    : Sets the LSM330G FullScale
* Input          : FS_250/FS_500/FS_2000
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetFullScale(Fullscale_t fs)
{
	u8_t value;

	ReadReg8(LSM330G_CTRL_REG4, &value);
	value &= 0xCF;
	value |= (fs << LSM330G_FS);
	WriteReg8(LSM330G_CTRL_REG4, value);
}

float LSM330G::GetFullScale()
{
	u8_t value;

	ReadReg8(LSM330G_CTRL_REG4, &value);
	value &= 0x30;
	value = (value >> LSM330G_FS);
	if (value == 0)
		return 250.0;
	else if (value == 1)
		return 500.0;
	return 2000.0;
}

float LSM330G::GetGain()
{
	u8_t value;

	ReadReg8(LSM330G_CTRL_REG4, &value);
	value &= 0x30;
	value = (value >> LSM330G_FS);
	if (value == 0) {
		/* Full Scale 250 */
		return 0.00875;
	} else if (value == 1) {
		/* Full Scale 500 */
		return 0.0175;
	}
	/* Full Scale 2000 */
	return 0.070;
}

/*******************************************************************************
* Function Name  : LSM330G_SetBDU
* Description    : Enable/Disable Block Data Update Functionality
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetBDU(State_t bdu)
{
	u8_t value;

	ReadReg8(LSM330G_CTRL_REG4, &value);
	value &= 0x7F;
	value |= (bdu << LSM330G_BDU);
	WriteReg8(LSM330G_CTRL_REG4, value);
}


/*******************************************************************************
* Function Name  : LSM330G_SetBLE
* Description    : Set Endianess (MSB/LSB)
* Input          : BLE_LSB / BLE_MSB
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetBLE(Endianess_t ble)
{
	u8_t value;

	ReadReg8(LSM330G_CTRL_REG4, &value);
	value &= 0xBF;
	value |= (ble << LSM330G_BLE);
	WriteReg8(LSM330G_CTRL_REG4, value);
}


/*******************************************************************************
* Function Name  : LSM330G_HPFEnable
* Description    : Enable/Disable High Pass Filter
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::HPFEnable(State_t hpf)
{
	u8_t value;

	ReadReg8(LSM330G_CTRL_REG5, &value);
	value &= 0xEF;
	value |= (hpf << LSM330G_HPEN);
	WriteReg8(LSM330G_CTRL_REG5, value);
}

/*******************************************************************************
* Function Name  : LSM330G_Reboot
* Description    : Reboot the memory content
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::Reboot()
{
	int i;

	WriteReg8(LSM330G_CTRL_REG5, (1 << LSM330G_BOOT));
	/*
	 * Add some delay after reboot
	 */
	for (i = 0; i < 100000; i++)
		;
}
/*******************************************************************************
* Function Name  : LSM330G_SetHPFMode
* Description    : Set High Pass Filter Modality
* Input          : HPM_NORMAL_MODE_RES/HPM_REF_SIGNAL/HPM_NORMAL_MODE/HPM_AUTORESET_INT
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetHPFMode(HPFMode_t hpf)
{
	u8_t value;

	ReadReg8(LSM330G_CTRL_REG2, &value);
	value &= 0xCF;
	value |= (hpf << LSM330G_HPM);
	WriteReg8(LSM330G_CTRL_REG2, value);
}


/*******************************************************************************
* Function Name  : LSM330G_SetHPFCutOFF
* Description    : Set High Pass CUT OFF Freq
* Input          : HPFCF_0,HPFCF_1,HPFCF_2... See Table 27 of the datasheet
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetHPFCutOFF(HPFCutOffFreq_t hpf)
{
	u8_t value;

	ReadReg8(LSM330G_CTRL_REG2, &value);
	value &= 0xF0;
	value |= (hpf << LSM330G_HPFC0);
	WriteReg8(LSM330G_CTRL_REG2, value);
}


/*******************************************************************************
* Function Name  : LSM330G_SetIntPinMode
* Description    : Set Interrupt Pin Modality (push pull or Open drain)
* Input          : PUSH_PULL/OPEN_DRAIN
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetIntPinMode(IntPinMode_t pm)
{
	u8_t value;

	ReadReg8(LSM330G_CTRL_REG3, &value);
	value &= 0xEF;
	value |= (pm << LSM330G_PP_OD);
	WriteReg8(LSM330G_CTRL_REG3, value);
}


/*******************************************************************************
* Function Name  : LSM330G_SetInt1Pin
* Description    : Set Interrupt1 pin Function
* Input          : LSM330G_I1_ON_PIN_INT1_ENABLE | LSM330G_I1_BOOT_ON_INT1 | LSM330G_INT1_ACTIVE_HIGH
* example        : LSM330G_SetInt1Pin(LSM330G_I1_ON_PIN_INT1_ENABLE | LSM330G_I1_BOOT_ON_INT1_ENABLE | LSM330G_INT1_ACTIVE_LOW)
* to enable Interrupt 1 or Bootsatus on interrupt 1 pin
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetInt1Pin(LSM330G_Int1PinConf_t pinConf)
{
	u8_t value;

	ReadReg8(LSM330G_CTRL_REG3, &value);
	value &= 0x1F;
	value |= pinConf;
	WriteReg8(LSM330G_CTRL_REG3, value);
}


/*******************************************************************************
* Function Name  : LSM330G_SetInt2Pin
* Description    : Set Interrupt2 pin Function
* Input          : LSM330G_I2_DRDY_ON_INT2_ENABLE/DISABLE |
                   LSM330G_WTM_ON_INT2_ENABLE/DISABLE |
                   LSM330G_OVERRUN_ON_INT2_ENABLE/DISABLE |
                   LSM330G_EMPTY_ON_INT2_ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetInt2Pin(LSM330G_Int2PinConf_t pinConf)
{
	u8_t value;

	ReadReg8(LSM330G_CTRL_REG3, &value);
	value &= 0xF0;
	value |= pinConf;
	WriteReg8(LSM330G_CTRL_REG3, value);
}


/*******************************************************************************
* Function Name  : LSM330G_Int1LatchEnable
* Description    : Enable Interrupt 1 Latching function
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::Int1LatchEnable(State_t latch)
{
	u8_t value;

	ReadReg8(LSM330G_INT1_CFG, &value);
	value &= 0xBF;
	value |= latch << LSM330G_LIR;
	WriteReg8(LSM330G_INT1_CFG, value);
}


/*******************************************************************************
* Function Name  : LSM330G_ResetInt1Latch
* Description    : Reset Interrupt 1 Latching function
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::ResetInt1Latch(void)
{
	u8_t value;

	ReadReg8(LSM330G_INT1_SRC, &value);
}


/*******************************************************************************
* Function Name  : LSM330G_SetIntConfiguration
* Description    : Interrupt 1 Configuration
* Input          : AND/OR, INT1_LIR ZHIE_ENABLE/DISABLE | INT1_ZLIE_ENABLE/DISABLE...
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetIntConfiguration(LSM330G_Int1Conf_t ic)
{
	u8_t value = ic;

	WriteReg8(LSM330G_INT1_CFG, value);
}


/*******************************************************************************
* Function Name  : LSM330G_SetInt1Threshold
* Description    : Sets Interrupt 1 Threshold
* Input          : Threshold = [0,31]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetInt1Threshold(IntThsAxis axis, u16_t ths)
{
	u8_t value;

	switch (axis) {

	case THS_X:
		//write the threshold LSB
		value = (u8_t) (ths & 0x00ff);
		WriteReg8(LSM330G_INT1_THS_XL, value);
		//write the threshold LSB
		value = (u8_t) (ths >> 8);
		WriteReg8(LSM330G_INT1_THS_XH, value);
		break;

	case THS_Y:
		//write the threshold LSB
		value = (u8_t) (ths & 0x00ff);
		WriteReg8(LSM330G_INT1_THS_YL, value);
		//write the threshold LSB
		value = (u8_t) (ths >> 8);
		WriteReg8(LSM330G_INT1_THS_YH, value);
		break;

	case THS_Z:
		//write the threshold LSB
		value = (u8_t) (ths & 0x00ff);
		WriteReg8(LSM330G_INT1_THS_ZL, value);
		//write the threshold LSB
		value = (u8_t) (ths >> 8);
		WriteReg8(LSM330G_INT1_THS_ZH, value);
		break;
	}
}


/*******************************************************************************
* Function Name  : LSM330G_SetInt1Duration
* Description    : Sets Interrupt 1 Duration
* Input          : Duration value
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetInt1Duration(LSM330G_Int1Conf_t id) {

  if (id > 127)
    throw std::invalid_argument("LSM330G::SetInt1Duration(LSM330G_Int1Conf_t id), invalid id");
  WriteReg8(LSM330G_INT1_DURATION, id);
}


/*******************************************************************************
* Function Name  : LSM330G_FIFOModeEnable
* Description    : Sets Fifo Modality
* Input          :
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::FIFOModeEnable(FifoMode_t fm)
{
	u8_t value;

	if (fm == FIFO_DISABLE) {

		ReadReg8(LSM330G_CTRL_REG5, &value);
		value &= 0xBF;
		WriteReg8(LSM330G_CTRL_REG5, value);
	} else {
		ReadReg8(LSM330G_CTRL_REG5, &value);
		value &= 0xBF;
		value |= MEMS_SET << LSM330G_FIFO_EN;
		WriteReg8(LSM330G_CTRL_REG5, value);
		ReadReg8(LSM330G_FIFO_CTRL_REG, &value);
		value &= 0x1f;
		value |= (fm << LSM330G_FM0);
		WriteReg8(LSM330G_FIFO_CTRL_REG, value);
	}
}


/*******************************************************************************
* Function Name  : LSM330G_SetWaterMark
* Description    : Sets Watermark Value
* Input          : Watermark = [0,31]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetWaterMark(u8_t wtm)
{
	u8_t value;

	if (wtm > 31)
		wtm = 31;
	ReadReg8(LSM330G_FIFO_CTRL_REG, &value);
	value &= 0xE0;
	value |= wtm;
	WriteReg8(LSM330G_FIFO_CTRL_REG, value);
}


/*******************************************************************************
* Function Name  : LSM330G_GetSatusReg
* Description    : Read the status register
* Input          : None
* Output         : status register buffer
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
u8_t LSM330G::GetSatusReg()
{
	u8_t ret;
	ReadReg8(LSM330G_STATUS_REG, &ret);
	return ret;
}


/*******************************************************************************
* Function Name  : LSM330G_GetAngRateRaw
* Description    : Read the Angular Rate Registers
* Input          : None
* Output         : Angular Rate Registers buffer
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::GetAngRateRaw(AxesRaw_t* buff)
{
	u8_t valueL = 0;
	u8_t valueH = 0;

	ReadReg8(LSM330G_OUT_X_L, &valueL);
	ReadReg8(LSM330G_OUT_X_H, &valueH);
	buff->AXIS_X = (i16_t) ((valueH << 8) | valueL);

	ReadReg8(LSM330G_OUT_Y_L, &valueL);
	ReadReg8(LSM330G_OUT_Y_H, &valueH);
	buff->AXIS_Y = (i16_t) ((valueH << 8) | valueL);

	ReadReg8(LSM330G_OUT_Z_L, &valueL);
	ReadReg8(LSM330G_OUT_Z_H, &valueH);
	buff->AXIS_Z = (i16_t) ((valueH << 8) | valueL);
}

void LSM330G::GetAngRateDPS(AxesDPS_t* buff)
{
	AxesRaw_t raw = {0, 0, 0};
	float gain = GetGain();
	GetAngRateRaw(&raw);
	buff->AXIS_X = raw.AXIS_X * gain;
	buff->AXIS_Y = raw.AXIS_Y * gain;
	buff->AXIS_Z = raw.AXIS_Z * gain;
}

void LSM330G::GetFifoAngRateDPS(AxesDPS_t* buff)
{
	AxesRaw_t raw = {0, 0, 0};
	uint8_t count = GetFifoSourceReg() & 0x1F;
	float scale = GetGain() / ((float)count);

	buff->AXIS_X = buff->AXIS_Y = buff->AXIS_Z = 0.0;
	for (uint8_t i = 0; i < count; i++) {
		GetAngRateRaw(&raw);
		buff->AXIS_X += raw.AXIS_X * scale;
		buff->AXIS_Y += raw.AXIS_Y * scale;
		buff->AXIS_Z += raw.AXIS_Z * scale;
	}
}

/*******************************************************************************
* Function Name  : LSM330G_GetInt1Src
* Description    : Reset Interrupt 1 Latching function
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
u8_t LSM330G::GetInt1Src()
{
	u8_t ret;
	ReadReg8(LSM330G_INT1_SRC, &ret);
	return ret;
}

/*******************************************************************************
* Function Name  : LSM330G_GetDeviceID
* Description    : Get The Device ID
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
u8_t LSM330G::GetDeviceID()
{
	u8_t ret;
	ReadReg8(LSM330G_WHO_AM_I_ADDR, &ret);
	return ret;
}

/*******************************************************************************
* Function Name  : LSM330G_GetFifoSourceReg
* Description    : Read Fifo source Register
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
u8_t LSM330G::GetFifoSourceReg()
{
	u8_t ret;
	ReadReg8(LSM330G_FIFO_SRC_REG, &ret);
	return ret;
}

u8_t LSM330G::GetFifoSourceFSS()
{
	return GetFifoSourceReg() & 0x1F;
}

/*******************************************************************************
* Function Name  : LSM330G_SetOutputDataAndFifoFilters
* Description    : ENABLE/DISABLE HIGH PASS and LOW PASS filters applied to output and fifo registers
*                : See Table 8 of AN3393 for more details
* Input          : LSM330G_NONE, LSM330G_HPH, LSM330G_LPF2, LSM330G_HPFLPF2
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetOutputDataAndFifoFilters(HPF_LPF2_Enable hpf)
{
	u8_t value;

	//HPF
	ReadReg8(LSM330G_CTRL_REG5, &value);

	switch (hpf) {

	case HPF_NONE:
		value &= 0xfc;
		value |= 0x00; //hpen = x, Out_sel_1 = 0, Out_sel_0 = 0
		break;

	case HPF:
		value &= 0xfc;
		value |= 0x01; //hpen = x, Out_sel_1 = 0, Out_sel_0 = 1
		break;

	case LPF2:
		value &= 0xed;
		value |= 0x02; //hpen = 0, Out_sel_1 = 1, Out_sel_0 = x
		break;

	case HPFLPF2:
		value &= 0xed;
		value |= 0x12; //hpen = 1, Out_sel_1 = 1, Out_sel_0 = x
		break;
	}

	WriteReg8(LSM330G_CTRL_REG5, value);
}


/*******************************************************************************
* Function Name  : LSM330G_SetInt1Filters
* Description    : ENABLE/DISABLE HIGH PASS and LOW PASS filters applied to Int1 circuitery
*                : See Table 9 of AN3393 for more details
* Input          : NONE, HPH, LPF2, HPFLPF2
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetInt1Filters(HPF_LPF2_Enable hpf)
{
	u8_t value;

	//HPF
	ReadReg8(LSM330G_CTRL_REG5, &value);

	switch (hpf) {

	case HPF_NONE:
		value &= 0xf3;
		value |= 0x00 << LSM330G_INT1_SEL0; //hpen = x, Int1_sel_1 = 0, Int1_sel_0 = 0
		break;

	case HPF:
		value &= 0xf3;
		value |= 0x01 << LSM330G_INT1_SEL0; //hpen = x, Int1_sel_1 = 0, Int1_sel_0 = 1
		break;

	case LPF2:
		value &= 0xe7;
		value |= 0x02 << LSM330G_INT1_SEL0; //hpen = 0, Int1_sel_1 = 1, Int1_sel_0 = x
		break;

	case HPFLPF2:
		value &= 0xe7;
		value |= 0x01 << LSM330G_HPEN;
		value |= 0x02 << LSM330G_INT1_SEL0; //hpen = 1, Int1_sel_1 = 1, Int1_sel_0 = x
		break;
	}

	WriteReg8(LSM330G_CTRL_REG5, value);
}


/*******************************************************************************
* Function Name  : LSM330G_SetSPIInterface
* Description    : Set SPI mode: 3 Wire Interface OR 4 Wire Interface
* Input          : LSM330G_SPI_3_WIRE, LSM330G_SPI_4_WIRE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void LSM330G::SetSPIInterface(SPIMode_t spi)
{
	u8_t value;

	ReadReg8(LSM330G_CTRL_REG4, &value);
	value &= 0xFE;
	value |= spi << LSM330G_SIM;
	WriteReg8(LSM330G_CTRL_REG4, value);
}

void LSM330G::SetAlignment(const Matrix3f& axes_align)
{
	axes_align_ = axes_align;
}

Vector3f LSM330G::GetSample()
{
	LSM330G::AxesDPS_t axes = {0,0,0};
	GetAngRateDPS(&axes);
	return axes_align_ * Vector3f(axes.AXIS_X, axes.AXIS_Y, axes.AXIS_Z);
}
