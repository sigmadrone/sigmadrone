/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : L3GD20.c
* Author             : MSH Application Team
* Author             : Andrea Labombarda
* Version            : $Revision:$
* Date               : $Date:$
* Description        : L3GD20 driver file
*                      
* HISTORY:
* Date               |	Modification                    |	Author
* 25/01/2012         |	Initial Revision                |	Andrea Labombarda
* 27/08/2012		 |  Modified to support multiple drivers in the same program                |	Antonio Micali
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOFTWARE IS SPECIFICALLY DESIGNED FOR EXCLUSIVE USE WITH ST PARTS.
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "l3gd20_device.h"
#include <stdexcept>

L3GD20::L3GD20(SPIMaster& spi, u8_t cs)
	: spi_(spi)
	, cs_(cs)
{
}

/*******************************************************************************
* Function Name		: L3GD20_ReadReg
* Description		: Generic Reading function. It must be fullfilled with either
*					: I2C or SPI reading functions					
* Input				: Register Address
* Output			: Data REad
* Return			: None
*******************************************************************************/
void L3GD20::ReadReg8(u8_t Reg, u8_t* Data)
{
	spi_.read(cs_, Reg, Data, 1);
}

/*******************************************************************************
* Function Name		: L3GD20_WriteReg
* Description		: Generic Writing function. It must be fullfilled with either
*					: I2C or SPI writing function
* Input				: Register Address, Data to be written
* Output			: None
* Return			: None
*******************************************************************************/
void L3GD20::WriteReg8(u8_t Reg, u8_t Data)
{
	spi_.write(cs_, Reg, &Data, 1);
}


/*******************************************************************************
* Function Name  : L3GD20_SetODR
* Description    : Sets L3GD20 Output Data Rate
* Input          : Output Data Rate
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetODR(L3GD20_ODR_t ov)
{
	u8_t value;

	ReadReg8(L3GD20_CTRL_REG1, &value);
	value &= 0x0f;
	value |= ov << 4;
	WriteReg8(L3GD20_CTRL_REG1, value);
}


/*******************************************************************************
* Function Name  : L3GD20_SetMode
* Description    : Sets L3GD20 Operating Mode
* Input          : Modality (NORMAL, SLEEP, POWER_DOWN)
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetMode(L3GD20_Mode_t md)
{
	u8_t value;

	ReadReg8(L3GD20_CTRL_REG1, &value);
	switch (md) {

	case L3GD20_POWER_DOWN:
		value &= 0xF7;
		value |= (MEMS_RESET << L3GD20_PD);
		break;

	case L3GD20_NORMAL:
		value &= 0xF7;
		value |= (MEMS_SET << L3GD20_PD);
		break;

	case L3GD20_SLEEP:
		value &= 0xF0;
		value |= ((MEMS_SET << L3GD20_PD) | (MEMS_RESET << L3GD20_ZEN) | (MEMS_RESET << L3GD20_YEN)
				| (MEMS_RESET << L3GD20_XEN));
		break;

	default:
		throw std::invalid_argument("L3GD20::SetMode(L3GD20_Mode_t md), invalid md");
	}
	WriteReg8(L3GD20_CTRL_REG1, value);
}


/*******************************************************************************
* Function Name  : L3GD20_SetAxis
* Description    : Enable/Disable L3GD20 Axis
* Input          : X_ENABLE/X_DISABLE | Y_ENABLE/Y_DISABLE | Z_ENABLE/Z_DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetAxis(L3GD20_Axis_t axis)
{
	u8_t value;

	ReadReg8(L3GD20_CTRL_REG1, &value);
	value &= 0xf8;
	value |= axis;
	WriteReg8(L3GD20_CTRL_REG1, value);
}


/*******************************************************************************
* Function Name  : L3GD20_SetFullScale
* Description    : Sets the L3GD20 FullScale
* Input          : FS_250/FS_500/FS_2000
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetFullScale(L3GD20_Fullscale_t fs)
{
	u8_t value;

	ReadReg8(L3GD20_CTRL_REG4, &value);
	value &= 0xCF;
	value |= (fs << L3GD20_FS);
	WriteReg8(L3GD20_CTRL_REG4, value);
}


/*******************************************************************************
* Function Name  : L3GD20_SetBDU
* Description    : Enable/Disable Block Data Update Functionality
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetBDU(State_t bdu)
{
	u8_t value;

	ReadReg8(L3GD20_CTRL_REG4, &value);
	value &= 0x7F;
	value |= (bdu << L3GD20_BDU);
	WriteReg8(L3GD20_CTRL_REG4, value);
}


/*******************************************************************************
* Function Name  : L3GD20_SetBLE
* Description    : Set Endianess (MSB/LSB)
* Input          : BLE_LSB / BLE_MSB
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetBLE(L3GD20_Endianess_t ble)
{
	u8_t value;

	ReadReg8(L3GD20_CTRL_REG4, &value);
	value &= 0xBF;
	value |= (ble << L3GD20_BLE);
	WriteReg8(L3GD20_CTRL_REG4, value);
}


/*******************************************************************************
* Function Name  : L3GD20_HPFEnable
* Description    : Enable/Disable High Pass Filter
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::HPFEnable(State_t hpf)
{
	u8_t value;

	ReadReg8(L3GD20_CTRL_REG5, &value);
	value &= 0xEF;
	value |= (hpf << L3GD20_HPEN);
	WriteReg8(L3GD20_CTRL_REG5, value);
}


/*******************************************************************************
* Function Name  : L3GD20_SetHPFMode
* Description    : Set High Pass Filter Modality
* Input          : HPM_NORMAL_MODE_RES/HPM_REF_SIGNAL/HPM_NORMAL_MODE/HPM_AUTORESET_INT
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetHPFMode(L3GD20_HPFMode_t hpf)
{
	u8_t value;

	ReadReg8(L3GD20_CTRL_REG2, &value);
	value &= 0xCF;
	value |= (hpf << L3GD20_HPM);
	WriteReg8(L3GD20_CTRL_REG2, value);
}


/*******************************************************************************
* Function Name  : L3GD20_SetHPFCutOFF
* Description    : Set High Pass CUT OFF Freq
* Input          : HPFCF_0,HPFCF_1,HPFCF_2... See Table 27 of the datasheet
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetHPFCutOFF(L3GD20_HPFCutOffFreq_t hpf)
{
	u8_t value;

	ReadReg8(L3GD20_CTRL_REG2, &value);
	value &= 0xF0;
	value |= (hpf << L3GD20_HPFC0);
	WriteReg8(L3GD20_CTRL_REG2, value);
}


/*******************************************************************************
* Function Name  : L3GD20_SetIntPinMode
* Description    : Set Interrupt Pin Modality (push pull or Open drain)
* Input          : PUSH_PULL/OPEN_DRAIN
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetIntPinMode(L3GD20_IntPinMode_t pm)
{
	u8_t value;

	ReadReg8(L3GD20_CTRL_REG3, &value);
	value &= 0xEF;
	value |= (pm << L3GD20_PP_OD);
	WriteReg8(L3GD20_CTRL_REG3, value);
}


/*******************************************************************************
* Function Name  : L3GD20_SetInt1Pin
* Description    : Set Interrupt1 pin Function
* Input          : L3GD20_I1_ON_PIN_INT1_ENABLE | L3GD20_I1_BOOT_ON_INT1 | L3GD20_INT1_ACTIVE_HIGH
* example        : L3GD20_SetInt1Pin(L3GD20_I1_ON_PIN_INT1_ENABLE | L3GD20_I1_BOOT_ON_INT1_ENABLE | L3GD20_INT1_ACTIVE_LOW) 
* to enable Interrupt 1 or Bootsatus on interrupt 1 pin
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetInt1Pin(L3GD20_Int1PinConf_t pinConf)
{
	u8_t value;

	ReadReg8(L3GD20_CTRL_REG3, &value);
	value &= 0x1F;
	value |= pinConf;
	WriteReg8(L3GD20_CTRL_REG3, value);
}


/*******************************************************************************
* Function Name  : L3GD20_SetInt2Pin
* Description    : Set Interrupt2 pin Function
* Input          : L3GD20_I2_DRDY_ON_INT2_ENABLE/DISABLE | 
                   L3GD20_WTM_ON_INT2_ENABLE/DISABLE | 
                   L3GD20_OVERRUN_ON_INT2_ENABLE/DISABLE | 
                   L3GD20_EMPTY_ON_INT2_ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetInt2Pin(L3GD20_Int2PinConf_t pinConf)
{
	u8_t value;

	ReadReg8(L3GD20_CTRL_REG3, &value);
	value &= 0xF0;
	value |= pinConf;
	WriteReg8(L3GD20_CTRL_REG3, value);
}


/*******************************************************************************
* Function Name  : L3GD20_Int1LatchEnable
* Description    : Enable Interrupt 1 Latching function
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::Int1LatchEnable(State_t latch)
{
	u8_t value;

	ReadReg8(L3GD20_INT1_CFG, &value);
	value &= 0xBF;
	value |= latch << L3GD20_LIR;
	WriteReg8(L3GD20_INT1_CFG, value);
}


/*******************************************************************************
* Function Name  : L3GD20_ResetInt1Latch
* Description    : Reset Interrupt 1 Latching function
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::ResetInt1Latch(void)
{
	u8_t value;

	ReadReg8(L3GD20_INT1_SRC, &value);
}


/*******************************************************************************
* Function Name  : L3GD20_SetIntConfiguration
* Description    : Interrupt 1 Configuration
* Input          : AND/OR, INT1_LIR ZHIE_ENABLE/DISABLE | INT1_ZLIE_ENABLE/DISABLE...
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetIntConfiguration(L3GD20_Int1Conf_t ic)
{
	u8_t value = ic;

	WriteReg8(L3GD20_INT1_CFG, value);
}


/*******************************************************************************
* Function Name  : L3GD20_SetInt1Threshold
* Description    : Sets Interrupt 1 Threshold
* Input          : Threshold = [0,31]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetInt1Threshold(L3GD20_IntThsAxis axis, u16_t ths)
{
	u8_t value;

	switch (axis) {

	case L3GD20_THS_X:
		//write the threshold LSB
		value = (u8_t) (ths & 0x00ff);
		WriteReg8(L3GD20_INT1_THS_XL, value);
		//write the threshold LSB
		value = (u8_t) (ths >> 8);
		WriteReg8(L3GD20_INT1_THS_XH, value);
		break;

	case L3GD20_THS_Y:
		//write the threshold LSB
		value = (u8_t) (ths & 0x00ff);
		WriteReg8(L3GD20_INT1_THS_YL, value);
		//write the threshold LSB
		value = (u8_t) (ths >> 8);
		WriteReg8(L3GD20_INT1_THS_YH, value);
		break;

	case L3GD20_THS_Z:
		//write the threshold LSB
		value = (u8_t) (ths & 0x00ff);
		WriteReg8(L3GD20_INT1_THS_ZL, value);
		//write the threshold LSB
		value = (u8_t) (ths >> 8);
		WriteReg8(L3GD20_INT1_THS_ZH, value);
		break;
	}
}


/*******************************************************************************
* Function Name  : L3GD20_SetInt1Duration
* Description    : Sets Interrupt 1 Duration
* Input          : Duration value
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetInt1Duration(L3GD20_Int1Conf_t id) {
 
  if (id > 127)
    throw std::invalid_argument("L3GD20::SetInt1Duration(L3GD20_Int1Conf_t id), invalid id");
  WriteReg8(L3GD20_INT1_DURATION, id);
}


/*******************************************************************************
* Function Name  : L3GD20_FIFOModeEnable
* Description    : Sets Fifo Modality
* Input          : 
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::FIFOModeEnable(L3GD20_FifoMode_t fm)
{
	u8_t value;

	if (fm == L3GD20_FIFO_DISABLE) {

		ReadReg8(L3GD20_CTRL_REG5, &value);
		value &= 0xBF;
		WriteReg8(L3GD20_CTRL_REG5, value);
	} else {
		ReadReg8(L3GD20_CTRL_REG5, &value);
		value &= 0xBF;
		value |= MEMS_SET << L3GD20_FIFO_EN;
		WriteReg8(L3GD20_CTRL_REG5, value);
		ReadReg8(L3GD20_FIFO_CTRL_REG, &value);
		value &= 0x1f;
		value |= (fm << L3GD20_FM0);
		WriteReg8(L3GD20_FIFO_CTRL_REG, value);
	}
}


/*******************************************************************************
* Function Name  : L3GD20_SetWaterMark
* Description    : Sets Watermark Value
* Input          : Watermark = [0,31]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetWaterMark(u8_t wtm)
{
	u8_t value;

	if (wtm > 31)
		wtm = 31;
	ReadReg8(L3GD20_FIFO_CTRL_REG, &value);
	value &= 0xE0;
	value |= wtm;
	WriteReg8(L3GD20_FIFO_CTRL_REG, value);
}


/*******************************************************************************
* Function Name  : L3GD20_GetSatusReg
* Description    : Read the status register
* Input          : None
* Output         : status register buffer
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
u8_t L3GD20::GetSatusReg()
{
	u8_t ret;
	ReadReg8(L3GD20_STATUS_REG, &ret);
	return ret;
}


/*******************************************************************************
* Function Name  : L3GD20_GetAngRateRaw
* Description    : Read the Angular Rate Registers
* Input          : None
* Output         : Angular Rate Registers buffer
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::GetAngRateRaw(AxesRaw_t* buff)
{
	u8_t valueL = 0;
	u8_t valueH = 0;

	ReadReg8(L3GD20_OUT_X_L, &valueL);
	ReadReg8(L3GD20_OUT_X_H, &valueH);
	buff->AXIS_X = (i16_t) ((valueH << 8) | valueL);

	ReadReg8(L3GD20_OUT_Y_L, &valueL);
	ReadReg8(L3GD20_OUT_Y_H, &valueH);
	buff->AXIS_Y = (i16_t) ((valueH << 8) | valueL);

	ReadReg8(L3GD20_OUT_Z_L, &valueL);
	ReadReg8(L3GD20_OUT_Z_H, &valueH);
	buff->AXIS_Z = (i16_t) ((valueH << 8) | valueL);
}


/*******************************************************************************
* Function Name  : L3GD20_GetInt1Src
* Description    : Reset Interrupt 1 Latching function
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
u8_t L3GD20::GetInt1Src()
{
	u8_t ret;
	ReadReg8(L3GD20_INT1_SRC, &ret);
	return ret;
}

/*******************************************************************************
* Function Name  : L3GD20_GetDeviceID
* Description    : Get The Device ID
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
u8_t L3GD20::GetDeviceID()
{
	u8_t ret;
	ReadReg8(L3GD20_WHO_AM_I_ADDR, &ret);
	return ret;
}

/*******************************************************************************
* Function Name  : L3GD20_GetFifoSourceReg
* Description    : Read Fifo source Register
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
u8_t L3GD20::GetFifoSourceReg()
{
	u8_t ret;
	ReadReg8(L3GD20_FIFO_SRC_REG, &ret);
	return ret;
}


/*******************************************************************************
* Function Name  : L3GD20_SetOutputDataAndFifoFilters
* Description    : ENABLE/DISABLE HIGH PASS and LOW PASS filters applied to output and fifo registers
*                : See Table 8 of AN3393 for more details
* Input          : L3GD20_NONE, L3GD20_HPH, L3GD20_LPF2, L3GD20_HPFLPF2
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetOutputDataAndFifoFilters(L3GD20_HPF_LPF2_Enable hpf)
{
	u8_t value;

	//HPF
	ReadReg8(L3GD20_CTRL_REG5, &value);

	switch (hpf) {

	case L3GD20_NONE:
		value &= 0xfc;
		value |= 0x00; //hpen = x, Out_sel_1 = 0, Out_sel_0 = 0
		break;

	case L3GD20_HPF:
		value &= 0xfc;
		value |= 0x01; //hpen = x, Out_sel_1 = 0, Out_sel_0 = 1
		break;

	case L3GD20_LPF2:
		value &= 0xed;
		value |= 0x02; //hpen = 0, Out_sel_1 = 1, Out_sel_0 = x
		break;

	case L3GD20_HPFLPF2:
		value &= 0xed;
		value |= 0x12; //hpen = 1, Out_sel_1 = 1, Out_sel_0 = x
		break;
	}

	WriteReg8(L3GD20_CTRL_REG5, value);
}


/*******************************************************************************
* Function Name  : L3GD20_SetInt1Filters
* Description    : ENABLE/DISABLE HIGH PASS and LOW PASS filters applied to Int1 circuitery
*                : See Table 9 of AN3393 for more details
* Input          : NONE, HPH, LPF2, HPFLPF2
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetInt1Filters(L3GD20_HPF_LPF2_Enable hpf)
{
	u8_t value;

	//HPF
	ReadReg8(L3GD20_CTRL_REG5, &value);

	switch (hpf) {

	case L3GD20_NONE:
		value &= 0xf3;
		value |= 0x00 << L3GD20_INT1_SEL0; //hpen = x, Int1_sel_1 = 0, Int1_sel_0 = 0
		break;

	case L3GD20_HPF:
		value &= 0xf3;
		value |= 0x01 << L3GD20_INT1_SEL0; //hpen = x, Int1_sel_1 = 0, Int1_sel_0 = 1
		break;

	case L3GD20_LPF2:
		value &= 0xe7;
		value |= 0x02 << L3GD20_INT1_SEL0; //hpen = 0, Int1_sel_1 = 1, Int1_sel_0 = x
		break;

	case L3GD20_HPFLPF2:
		value &= 0xe7;
		value |= 0x01 << L3GD20_HPEN;
		value |= 0x02 << L3GD20_INT1_SEL0; //hpen = 1, Int1_sel_1 = 1, Int1_sel_0 = x
		break;
	}

	WriteReg8(L3GD20_CTRL_REG5, value);
}


/*******************************************************************************
* Function Name  : L3GD20_SetSPIInterface
* Description    : Set SPI mode: 3 Wire Interface OR 4 Wire Interface
* Input          : L3GD20_SPI_3_WIRE, L3GD20_SPI_4_WIRE
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
void L3GD20::SetSPIInterface(L3GD20_SPIMode_t spi)
{
	u8_t value;

	ReadReg8(L3GD20_CTRL_REG4, &value);
	value &= 0xFE;
	value |= spi << L3GD20_SIM;
	WriteReg8(L3GD20_CTRL_REG4, value);
}
