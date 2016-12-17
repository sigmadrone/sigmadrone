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
#include <string.h>
#include "lps25hb.h"

/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80)
/* Multiple byte read/write command */
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)

void LPS25HB::assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number */
  printf("Wrong parameters tmp: file %s on line %lu\r\n", file, (unsigned long)line);
  throw std::runtime_error("LPS25HB_assert_failed");
}


LPS25HB::LPS25HB(SPIMaster& spi, uint8_t cs, const std::vector<GPIOPin>& pins)
	: spi_(spi)
	, cs_(cs)
	, pins_(pins)
{
	for (auto& pin : pins_)
		pin.init();
	LPS25HB_ConfigTypeDef_st config;
	memset(&config, 0, sizeof(config));
	config.PressResolution = LPS25HB_AVGP_32;
	config.TempResolution = LPS25HB_AVGT_32;
	config.OutputDataRate = LPS25HB_ODR_25HZ;
	config.BDU = LPS25HB_BDU_CONTINUOUS_UPDATE;
	config.AutoZero = LPS25HB_ENABLE;
	config.Reset_AZ = LPS25HB_ENABLE;
	config.Sim = LPS25HB_SPI_4_WIRE;
	Set_GenericConfig(&config);
	Set_FifoModeUse(LPS25HB_DISABLE);
	Activate();
}

LPS25HB::~LPS25HB()
{
	// TODO Auto-generated destructor stub
}

void LPS25HB::ReadReg(uint8_t reg, uint16_t nbytes, uint8_t* data)
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

void LPS25HB::WriteReg(uint8_t reg, uint16_t nbytes, uint8_t *data)
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

void LPS25HB::UpdateReg(uint8_t reg, uint8_t mask, uint8_t data)
{
	uint8_t tmp;

	ReadReg(LPS25HB_CTRL_REG1, 1, &tmp);
	tmp &= ~mask;
	tmp |= (data & mask);
	WriteReg(reg, 1, &tmp);
}

/**
  * @brief  Read identification code by WHO_AM_I register
  * @param  Buffer to empty by Device identification Value.
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
uint8_t LPS25HB::Get_DeviceID()
{
	uint8_t ret;
	ReadReg(LPS25HB_WHO_AM_I_REG, 1, &ret);
	return ret;
}


/**
  * @brief  Get the LPS25HB driver version.
  * @param  None
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB::Get_DriverVersion(LPS25HB_DriverVersion_st *Version)
{
	Version->Major = LPS25HB_DriverVersion_Major;
	Version->Minor = LPS25HB_DriverVersion_Minor;
	Version->Point = LPS25HB_DriverVersion_Point;
}

/**
  * @brief  Set LPS25HB Pressure and Temperature Resolution Mode
  * @param  Pressure Resolution
	* @param  Temperature Resolution
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::Set_Avg(LPS25HB_Avgp_et avgp, LPS25HB_Avgt_et avgt)
{
	uint8_t tmp;

	assert_param(IS_LPS25HB_AVGT(avgt));
	assert_param(IS_LPS25HB_AVGP(avgp));
	ReadReg(LPS25HB_RES_CONF_REG, 1, &tmp);
	tmp &= ~(LPS25HB_AVGT_MASK | LPS25HB_AVGP_MASK );
	tmp |= (uint8_t) avgp;
	tmp |= (uint8_t) avgt;
	WriteReg(LPS25HB_RES_CONF_REG, 1, &tmp);
}


/**
  * @brief  Exit the shutdown mode for LPS25HB.
  * @param  None
  * @retval None
*/

void LPS25HB::Activate(void)
{
	Set_PowerDownMode(LPS25HB_SET);
}

/**
  * @brief  Enter the shutdown mode for LPS25HB.
  * @param  None
  * @retval None
*/
void LPS25HB::DeActivate(void)
{
	Set_PowerDownMode(LPS25HB_RESET);
}

/**
  * @brief  Set LPS25HB Pressure Resolution Mode
  * @param  Pressure Resolution
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::Set_AvgP(LPS25HB_Avgp_et avgp)
{
	assert_param(IS_LPS25HB_AVGP(avgp));
	UpdateReg(LPS25HB_RES_CONF_REG, LPS25HB_AVGP_MASK, avgp);
}

/**
  * @brief  Set LPS25HB Temperature Resolution Mode
  * @param  Temperature Resolution
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::Set_AvgT(LPS25HB_Avgt_et avgt)
{
	assert_param(IS_LPS25HB_AVGT(avgt));
	UpdateReg(LPS25HB_RES_CONF_REG, LPS25HB_AVGT_MASK, avgt);
}

/**
  * @brief  Get LPS25HB Pressure Resolution Mode
  * @param  Pressure Resolution
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Avgp_et LPS25HB::Get_AvgP()
{
	uint8_t tmp;

	ReadReg(LPS25HB_RES_CONF_REG, 1, &tmp);
	return (LPS25HB_Avgp_et)(tmp & LPS25HB_AVGP_MASK);
}

/**
  * @brief  Get LPS25HB Temperature Resolution Mode
  * @param  Temperature Resolution
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Avgt_et LPS25HB::Get_AvgT()
{
	uint8_t tmp;

	ReadReg(LPS25HB_RES_CONF_REG, 1, &tmp);
	return (LPS25HB_Avgt_et)(tmp & LPS25HB_AVGT_MASK);
}

/**
  * @brief  Set LPS25HB Output Data Rate
  * @param  Output Data Rate
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::Set_Odr(LPS25HB_Odr_et odr)
{
	assert_param(IS_LPS25HB_ODR(odr));
	UpdateReg(LPS25HB_CTRL_REG1, LPS25HB_ODR_MASK, odr);
}

/**
  * @brief  Get LPS25HB Output Data Rate
  * @param  Buffer to empty with Output Data Rate
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Odr_et LPS25HB::Get_Odr()
{
	uint8_t tmp;

	ReadReg(LPS25HB_CTRL_REG1, 1, &tmp);
	return (LPS25HB_Odr_et)(tmp & LPS25HB_ODR_MASK);
}

/**
  * @brief  SET/RESET LPS25HB Power Down Mode bit
  * @param  LPS25HB_SET (Active Mode)/LPS25HB_RESET (Power Down Mode)
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::Set_PowerDownMode(LPS25HB_BitStatus_et pd)
{
	assert_param(IS_LPS25HB_BitStatus(pd));
	UpdateReg(LPS25HB_CTRL_REG1, LPS25HB_PD_MASK, ((uint8_t) pd) << LPS25HB_PD_BIT);
}

/**
  * @brief  Set Block Data Mode
  * @detail It is recommended to set BDU bit to �1�.
  * @detail This feature avoids reading LSB and MSB related to different samples.
  * @param 	LPS25HB_BDU_CONTINUOS_UPDATE, LPS25HB_BDU_NO_UPDATE
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */

void LPS25HB::Set_Bdu(LPS25HB_Bdu_et bdu)
{
  assert_param(IS_LPS25HB_BDUMode(bdu));
  UpdateReg(LPS25HB_CTRL_REG1, LPS25HB_BDU_MASK, (uint8_t)bdu);
}

/**
  * @brief  Get Block Data Mode
  * @param 	Buffer to empty whit thw bdu mode read from sensor
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Bdu_et LPS25HB::Get_Bdu()
{
	uint8_t tmp;

	ReadReg(LPS25HB_CTRL_REG1, 1, &tmp);
	return (LPS25HB_Bdu_et) (tmp & LPS25HB_BDU_MASK );
}

/**
  * @brief  Enable/Disable Interrupt Circuit.
  * @param  LPS25HB_ENABLE/LPS25HB_DISABLE
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::Set_InterruptCircuitEnable(LPS25HB_State_et diff_en)
{
	assert_param(IS_LPS25HB_State(diff_en));
	UpdateReg(LPS25HB_CTRL_REG1, LPS25HB_DIFF_EN_MASK, ((uint8_t) diff_en) << LPS25HB_DIFF_EN_BIT);
}

/**
* @brief  Get the Interrupt Circuit bit.
* @param Buffer to empty whit thw diff_en mode read from sensor
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_State_et LPS25HB::Get_InterruptCircuitEnable()
{
  uint8_t tmp;

  ReadReg(LPS25HB_CTRL_REG1, 1, &tmp);
  return (LPS25HB_State_et)((tmp & LPS25HB_DIFF_EN_MASK)>>LPS25HB_DIFF_EN_BIT);
}



/**
* @brief  Set ResetAutoZero Function bit
* @details REF_P reg (@0x08..0A) set pressure reference to default tmp RPDS reg (0x39/3A).
* @param  None
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB::ResetAZ(void)
{
	UpdateReg(LPS25HB_CTRL_REG1, LPS25HB_RESET_AZ_MASK, LPS25HB_RESET_AZ_MASK);
}

/**
* @brief  Set SPI mode: 3 Wire Interface or 4 Wire Interface
* @param   LPS25HB_SPI_3_WIRE, LPS25HB_SPI_4_WIRE
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB::Set_SpiInterface(LPS25HB_SPIMode_et spimode)
{
	assert_param(IS_LPS25HB_SPIMode(spimode));
	UpdateReg(LPS25HB_CTRL_REG1, LPS25HB_SIM_MASK, spimode);
}

/**
* @brief  Get SPI mode: 3 Wire Interface or 4 Wire Interface
* @param  Buffer to empty with spi mode read from Sensor
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_SPIMode_et LPS25HB::Get_SpiInterface()
{
	uint8_t tmp;

	ReadReg(LPS25HB_CTRL_REG1, 1, &tmp);
	return (LPS25HB_SPIMode_et) (tmp & LPS25HB_SIM_MASK );
}

/**
  * @brief  Enable/Disable I2C Mode
  * @param State: Enable (reset bit)/ Disable (set bit)
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::Set_I2C(LPS25HB_State_et statei2c)
{
	uint8_t tmp;

	assert_param(IS_LPS25HB_State(statei2c));
	ReadReg(LPS25HB_CTRL_REG2, 1, &tmp);
	tmp &= ~LPS25HB_I2C_MASK;
	if (statei2c == LPS25HB_DISABLE) {
		tmp |= LPS25HB_I2C_MASK;
	}
	WriteReg(LPS25HB_CTRL_REG2, 1, &tmp);
}

/**
* @brief  Set the one-shot bit in order to start acquisition when the ONE SHOT mode
          has been selected by the ODR configuration.
* @detail Once the measurement is done, ONE_SHOT bit will self-clear.
* @param  None
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::StartOneShotMeasurement(void)
{
	UpdateReg(LPS25HB_CTRL_REG2, LPS25HB_ONE_SHOT_MASK, LPS25HB_ONE_SHOT_MASK);
}

/**
* @brief  Set AutoZero Function bit
* @detail When set to �1�, the actual pressure output is copied in the REF_P reg (@0x08..0A)
* @param  LPS25HB_SET/LPS25HB_RESET
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::Set_AutoZeroFunction(LPS25HB_BitStatus_et autozero)
{
	UpdateReg(LPS25HB_CTRL_REG2, LPS25HB_AUTO_ZERO_MASK, ((uint8_t)autozero) << LPS25HB_AUTO_ZERO_BIT);
}


/**
  * @brief   Software Reset. Self-clearing upon completion
  * @param   None
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::SwReset(void)
{
	uint8_t tmp;

	ReadReg(LPS25HB_CTRL_REG2, 1, &tmp);
	tmp |= (0x01 << LPS25HB_SW_RESET_BIT);
	WriteReg(LPS25HB_CTRL_REG2, 1, &tmp);
}

/**
* @brief  Reboot Memory Content
* @param 	 None
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB::MemoryBoot(void)
{
	uint8_t tmp;

	ReadReg(LPS25HB_CTRL_REG2, 1, &tmp);
	tmp |= (0x01 << LPS25HB_BOOT_BIT);
	WriteReg(LPS25HB_CTRL_REG2, 1, &tmp);
}

/**
* @brief   Software Reset ann Reboot Memory Content.
* @detail  The device is reset to the power on configuration if the SWRESET bit is set to �1�
           and BOOT is set to �1�; Self-clearing upon completion.
* @param  None
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::SwResetAndMemoryBoot(void)
{
	uint8_t tmp;

	ReadReg(LPS25HB_CTRL_REG2, 1, &tmp);
	tmp |= ((0x01 << LPS25HB_SW_RESET_BIT) | (0x01 << LPS25HB_BOOT_BIT));
	WriteReg(LPS25HB_CTRL_REG2, 1, &tmp);
}

/**
  * @brief   Enable/Disable FIFO Mode
  * @param   LPS25HB_ENABLE/LPS25HB_DISABLE
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::Set_FifoModeUse(LPS25HB_State_et status)
{
	assert_param(IS_LPS25HB_State(status));
	UpdateReg(LPS25HB_CTRL_REG2, LPS25HB_FIFO_EN_MASK, ((uint8_t) status) << LPS25HB_FIFO_EN_BIT);
}

/**
* @brief    Enable/Disable FIFO Watermark Level Use
* @param    LPS25HB_ENABLE/LPS25HB_DISABLE
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB::Set_FifoWatermarkLevelUse(LPS25HB_State_et status)
{
	assert_param(IS_LPS25HB_State(status));
	UpdateReg(LPS25HB_CTRL_REG2, LPS25HB_WTM_EN_MASK, ((uint8_t) status) << LPS25HB_WTM_EN_BIT);
}

/**
* @brief   Enable/Disable 1 HZ ODR Decimation
* @param   LPS25HB_ENABLE/LPS25HB_DISABLE
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB::Set_FifoMeanDecUse(LPS25HB_State_et status)
{
	assert_param(IS_LPS25HB_State(status));
	UpdateReg(LPS25HB_CTRL_REG2, LPS25HB_FIFO_MEAN_MASK, ((uint8_t) status) << LPS25HB_FIFO_MEAN_BIT);
}

/**
* @brief   Enable/Disable Interrupt Active High (default tmp 0) or Low(tmp 1)
* @param   LPS25HB_ENABLE/LPS25HB_DISABLE
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB::Set_InterruptActiveLevel(LPS25HB_State_et status)
{
	assert_param(IS_LPS25HB_State(status));
	UpdateReg(LPS25HB_CTRL_REG3, LPS25HB_INT_H_L_MASK, ((uint8_t) status) << LPS25HB_INT_H_L_BIT);
}

/**
* @brief   Push-pull/open drain selection on interrupt pads. Default tmp: 0
* @param LPS25HB_PushPull/LPS25HB_OpenDrain
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB::Set_InterruptOutputType(LPS25HB_OutputType_et output)
{
	assert_param(IS_LPS25HB_OutputType(output));
	UpdateReg(LPS25HB_CTRL_REG3, LPS25HB_PP_OD_MASK, (uint8_t)output);
}

/**
* @brief   Set Data signal on INT1 pad control bits. Default tmp: 00
* @param   LPS25HB_DATA,LPS25HB_P_HIGH_LPS25HB_P_LOW,LPS25HB_P_LOW_HIGH
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB::Set_InterruptControlConfig(LPS25HB_OutputSignalConfig_et config)
{
	assert_param(IS_LPS25HB_OutputSignal(config));
	UpdateReg(LPS25HB_CTRL_REG3, (LPS25HB_INT1_S2_MASK | LPS25HB_INT1_S1_MASK), (uint8_t)config);
}

/**
  * @brief   Set INT1 interrupt pins configuration
  * @param   LPS25HB_EMPTY,LPS25HB_WTM,LPS25HB_OVR,LPS25HB_DATA_READY
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::Set_InterruptDataConfig(LPS25HB_DataSignalType_et signal)
{
  assert_param(IS_LPS25HB_DataSignal(signal));
  UpdateReg(
		  LPS25HB_CTRL_REG4,
		  (LPS25HB_P1_EMPTY_MASK | LPS25HB_P1_WTM_MASK | LPS25HB_P1_OVERRUN_MASK  | LPS25HB_P1_DRDY_MASK ),
		  (uint8_t)signal);
}

/**
  * @brief   Enable\Disable Interrupt Generation on differential pressure low and/or high event
  * @param   LPS25HB_DISABLE_INT, LPS25HB_PHE,LPS25HB_PLE,LPS25HB_PLE_PHE
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::Set_InterruptDifferentialConfig(LPS25HB_InterruptDiffConfig_et config)
{
	uint8_t tmp;

	assert_param(IS_LPS25HB_InterruptDiff(config));
	ReadReg(LPS25HB_INTERRUPT_CFG_REG, 1, &tmp);
	tmp &= ~(LPS25HB_PL_E_MASK | LPS25HB_PH_E_MASK);
	tmp |= (uint8_t) config;
	if (config != LPS25HB_DISABLE_INT) {
		/* Enable DIFF_EN bit in CTRL_REG1 */
		Set_InterruptCircuitEnable(LPS25HB_ENABLE);
	}
	WriteReg(LPS25HB_INTERRUPT_CFG_REG, 1, &tmp);
}

/**
  * @brief   Enable/Disable Latch Interrupt request into INT_SOURCE register.
  * @param   LPS25HB_ENABLE/LPS25HB_DISABLE
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::LatchInterruptRequest(LPS25HB_State_et status)
{
	assert_param(IS_LPS25HB_State(status));
	UpdateReg(LPS25HB_INTERRUPT_CFG_REG, LPS25HB_LIR_MASK, (((uint8_t) status) << LPS25HB_LIR_BIT));
}

/**
  * @brief   Get the Interrupt Generation on differential pressure status event.
  * @detail  The INT_SOURCE register is cleared by reading it.
  * @param  Status Event Flag: PH,PL,IA
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_InterruptDiffStatus_st LPS25HB::Get_InterruptDifferentialEventStatus()
{
	LPS25HB_InterruptDiffStatus_st interruptsource;
	uint8_t tmp;

	ReadReg(LPS25HB_INTERRUPT_SOURCE_REG, 1, &tmp);
	interruptsource.PH = (uint8_t) (tmp & LPS25HB_PH_MASK );
	interruptsource.PL = (uint8_t) ((tmp & LPS25HB_PL_MASK ) >> LPS25HB_PL_BIT);
	interruptsource.IA = (uint8_t) ((tmp & LPS25HB_IA_MASK ) >> LPS25HB_IA_BIT);
	return interruptsource;
}

/**
  * @brief   Get the status of Pressure and Temperature data
  * @param   Data Status Flag:  TempDataAvailable, TempDataOverrun, PressDataAvailable, PressDataOverrun
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_DataStatus_st LPS25HB::Get_DataStatus()
{
	uint8_t tmp;
	LPS25HB_DataStatus_st datastatus;

	ReadReg(LPS25HB_STATUS_REG, 1, &tmp);
	datastatus.TempDataAvailable = (tmp & LPS25HB_TDA_MASK );
	datastatus.PressDataAvailable = (uint8_t) ((tmp & LPS25HB_PDA_MASK ) >> LPS25HB_PDA_BIT);
	datastatus.TempDataOverrun = (uint8_t) ((tmp & LPS25HB_TOR_MASK ) >> LPS25HB_TOR_BIT);
	datastatus.PressDataOverrun = (uint8_t) ((tmp & LPS25HB_POR_MASK ) >> LPS25HB_POR_BIT);
	return datastatus;
}

/**
* @brief    Get the raw pressure tmp
* @detail   The data are expressed as PRESS_OUT_H/_L/_XL in 2�s complement.
            Pout(hPA)=PRESS_OUT / 4096
* @param    The buffer to empty with the pressure raw tmp
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
int32_t LPS25HB::Get_RawPressure()
{
	int32_t raw_press;
	uint8_t buffer[3];
	uint32_t tmp = 0;

	ReadReg(LPS25HB_PRESS_OUT_XL_REG, 3, buffer);
	/* Build the raw data */
	for (uint8_t i = 0; i < 3; i++)
		tmp |= (((uint32_t) buffer[i]) << (8 * i));
	/* convert the 2's complement 24 bit to 2's complement 32 bit */
	if (tmp & 0x00800000)
		tmp |= 0xFF000000;
	raw_press = ((int32_t) tmp);
	return raw_press;
}

/**
* @brief    Get the Pressure value in hPA.
* @detail   The data are expressed as PRESS_OUT_H/_L/_XL in 2�s complement.
            Pout(hPA)=PRESS_OUT / 4096
* @param    The buffer to empty with the pressure value that must be divided by 100 to get the value in hPA
* @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
*/
int32_t LPS25HB::Get_Pressure()
{
	return (Get_RawPressure() * 100) / 4096;
}

float LPS25HB::Get_PressureHpa()
{
	return ((float)Get_RawPressure()) / 4096.0f;
}

/**
* @brief    Get the Raw Temperature tmp.
* @detail   Temperature data are expressed as TEMP_OUT_H&TEMP_OUT_L as 2�s complement number.
            Tout(degC)=42.5+ (TEMP_OUT/480)
* @param    Buffer to empty with the temperature raw tmp.
* @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
  */
int16_t LPS25HB::Get_RawTemperature()
{
	uint8_t buffer[2];
	uint16_t tmp;

	ReadReg(LPS25HB_TEMP_OUT_L_REG, 2, buffer);
	/* Build the raw tmp */
	tmp = (((uint16_t) buffer[1]) << 8) + (uint16_t) buffer[0];
	return ((int16_t) tmp);
}


/**
* @brief    Get the Temperature value in �C.
* @detail   Temperature data are expressed as TEMP_OUT_H&TEMP_OUT_L as 2�s complement number.
*            Tout(degC)=42.5+ (TEMP_OUT/480)
* @param    Buffer to empty with the temperature value that must be divided by 10 to get the value in �C
* @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
*/
uint16_t LPS25HB::Get_Temperature()
{
	return Get_RawTemperature() / 48 + 425;
}

float LPS25HB::Get_TemperatureCelsius()
{
	return ((float) Get_Temperature()) / 10.0f;
}

/**
* @brief    Get the threshold tmp used for pressure interrupt generation (hPA).
* @detail   THS_P=THS_P_H&THS_P_L and is expressed as unsigned number.
            P_ths(hPA)=(THS_P)/16.
* @param    Buffer to empty with the pressure threshold in hPA
* @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
*/
int16_t LPS25HB::Get_PressureThreshold()
{
	uint8_t tempReg[2];

	ReadReg(LPS25HB_THS_P_LOW_REG, 2, tempReg);
	return (((((uint16_t) tempReg[1]) << 8) + tempReg[0]) / 16);
}

/**
* @brief    Set the threshold tmp used for pressure interrupt generation (hPA).
* @detail   THS_P=THS_P_H&THS_P_L and is expressed as unsigned number.
            P_ths(hPA)=(THS_P)/16.
* @param    Pressure threshold in hPA
* @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB::Set_PressureThreshold(int16_t P_ths)
{
	uint8_t buffer[2];

	buffer[0] = (uint8_t) (16 * P_ths);
	buffer[1] = (uint8_t) (((uint16_t) (16 * P_ths)) >> 8);
	WriteReg(LPS25HB_THS_P_LOW_REG, 2, buffer);
}


/**
* @brief  Set Fifo Mode
* @param  LPS25HB_FIFO_BYPASS_MODE, LPS25HB_FIFO_MODE, LPS25HB_FIFO_STREAM_MODE, LPS25HB_FIFO_TRIGGER_STREAMTOFIFO_MODE,
          LPS25HB_FIFO_TRIGGER_BYPASSTOSTREAM_MODE,LPS25HB_FIFO_MEAN_MODE, LPS25HB_FIFO_TRIGGER_BYPASSTOFIFO_MODE
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB::Set_FifoMode(LPS25HB_FifoMode_et fifomode)
{
  assert_param(IS_LPS25HB_FifoMode(fifomode));
  UpdateReg(LPS25HB_CTRL_FIFO_REG, LPS25HB_FMODE_MASK, (uint8_t)fifomode);
}

/**
  * @brief    Get Fifo Mode
  * @param 	  buffer to empty with fifo mode tmp
  * @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_FifoMode_et LPS25HB::Get_FifoMode()
{
	uint8_t tmp;

	ReadReg(LPS25HB_CTRL_FIFO_REG, 1, &tmp);
	tmp &= ~LPS25HB_WTM_POINT_MASK;
	return (LPS25HB_FifoMode_et) tmp;
}

/**
  * @brief    Get the Fifo Status
  * @param 	  Status Flag: FIFO_WTM,FIFO_EMPTY,FIFO_FULL,FIFO_LEVEL
  * @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_FifoStatus_st LPS25HB::Get_FifoStatus()
{
	uint8_t tmp;
	LPS25HB_FifoStatus_st status;

	ReadReg(LPS25HB_STATUS_FIFO_REG, 1, &tmp);
	status.FIFO_WTM = (uint8_t) ((tmp & LPS25HB_WTM_FIFO_MASK ) >> LPS25HB_WTM_FIFO_BIT);
	status.FIFO_FULL = (uint8_t) ((tmp & LPS25HB_FULL_FIFO_MASK ) >> LPS25HB_FULL_FIFO_BIT);
	status.FIFO_EMPTY = (uint8_t) ((tmp & LPS25HB_EMPTY_FIFO_MASK ) >> LPS25HB_EMPTY_FIFO_BIT);
	status.FIFO_LEVEL = (uint8_t) (tmp & LPS25HB_DIFF_POINT_MASK );
	return status;
}

/**
  * @brief    Set Watermark Value
  * @param 	  wtmlevel = [0,31]
  * @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::Set_FifoWatermarkLevel(uint8_t wtmlevel)
{
  assert_param(IS_LPS25HB_WtmLevel(wtmlevel));
  UpdateReg(LPS25HB_CTRL_FIFO_REG, LPS25HB_WTM_POINT_MASK, wtmlevel);
}

/**
  * @brief    Get Watermark Value
* @param 	  wtmlevel tmp read from sensor
  * @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
  */
uint8_t LPS25HB::Get_FifoWatermarkLevel()
{
	uint8_t wtmlevel;

	ReadReg(LPS25HB_CTRL_FIFO_REG, 1, &wtmlevel);
	wtmlevel &= LPS25HB_WTM_POINT_MASK;
	return wtmlevel;
}

/**
  * @brief   Set the number of sample to perform moving average when FIFO_MEAN_MODE is used
  * @param 	 LPS25HB_FIFO_SAMPLE_2,LPS25HB_FIFO_SAMPLE_4,LPS25HB_FIFO_SAMPLE_8,LPS25HB_FIFO_SAMPLE_16,LPS25HB_FIFO_SAMPLE_32
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::Set_FifoSampleSize(LPS25HB_FifoMeanModeSample_et samplesize)
{
	assert_param(IS_LPS25HB_FifoMeanModeSample(samplesize));
	UpdateReg(LPS25HB_CTRL_FIFO_REG, LPS25HB_WTM_POINT_MASK, (uint8_t) samplesize);
}

/**
  * @brief   Get the number of sample to perform moving average when FIFO_MEAN_MODE is used
* @param 	 buffer to empty with sample size tmp
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_FifoMeanModeSample_et LPS25HB::Get_FifoSampleSize()
{
	uint8_t tmp;

	ReadReg(LPS25HB_CTRL_FIFO_REG, 1, &tmp);
	tmp &= LPS25HB_WTM_POINT_MASK;
	return (LPS25HB_FifoMeanModeSample_et) tmp;
}

/**
* @brief   Get the reference pressure after soldering for computing differential pressure (hPA)
* @param   buffer to empty with the he pressure tmp (hPA)
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
int16_t LPS25HB::Get_PressureOffsetValue()
{
	uint8_t buffer[2];
	int16_t raw_press;

	ReadReg(LPS25HB_RPDS_L_REG, 2, buffer);
	raw_press = (int16_t) ((((uint16_t) buffer[1]) << 8) + (uint16_t) buffer[0]);
	return (raw_press * 100) / 4096;
}

/**
* @brief   Set Generic Configuration
* @param   Struct to empty with the chosen tmp
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB::Set_GenericConfig(LPS25HB_ConfigTypeDef_st* pxLPS25HBInit)
{
	/* Step 1. Init REF_P register*/
	/* The REF_P is the Reference Pressure. Its reset tmp is 0x00*/
	/* The REF_P will be set to the defualt RPDS (0x39h) tmp  if Reset_AZ is enabled.*/
	/* The REF_P will be set the actual pressure output if AutoZero is enabled*/

	if ((pxLPS25HBInit->Reset_AZ) == LPS25HB_ENABLE) {
		ResetAZ();
	} else if ((pxLPS25HBInit->AutoZero) == LPS25HB_ENABLE) {
		Set_AutoZeroFunction(LPS25HB_SET);
	}

	/* Step 2. Init the Pressure and Temperature Resolution*/
	Set_Avg(pxLPS25HBInit->PressResolution, pxLPS25HBInit->TempResolution);

	/* Step 3. Init the Output Data Rate*/
	Set_Odr(pxLPS25HBInit->OutputDataRate);

	/*Step 4. BDU bit is used to inhibit the output registers update between the reading of upper and
	 lower register parts. In default mode (BDU = �0�), the lower and upper register parts are
	 updated continuously. If it is not sure to read faster than output data rate, it is recommended
	 to set BDU bit to �1�. In this way, after the reading of the lower (upper) register part, the
	 content of that output registers is not updated until the upper (lower) part is read too.
	 This feature avoids reading LSB and MSB related to different samples.*/
	Set_Bdu(pxLPS25HBInit->BDU);

	/*Step 5. SIM bit selects the SPI serial interface mode.*/
	/* This feature has effect only if SPI interface is used*/
	Set_SpiInterface(pxLPS25HBInit->Sim);
}


/**
* @brief   Get Generic Configuration
* @param   Struct to empty with the  tmp read from sensor
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB::Get_GenericConfig(LPS25HB_ConfigTypeDef_st* pxLPS25HBInit)
{
	uint8_t tmp;

	ReadReg(LPS25HB_RES_CONF_REG, 1, &tmp);

	pxLPS25HBInit->PressResolution = (LPS25HB_Avgp_et) (tmp & LPS25HB_AVGP_MASK );
	pxLPS25HBInit->TempResolution = (LPS25HB_Avgt_et) (tmp & LPS25HB_AVGT_MASK );

	ReadReg(LPS25HB_CTRL_REG1, 1, &tmp);
	pxLPS25HBInit->OutputDataRate = (LPS25HB_Odr_et) (tmp & LPS25HB_ODR_MASK );
	pxLPS25HBInit->BDU = (LPS25HB_Bdu_et) (tmp & LPS25HB_BDU_MASK );
	pxLPS25HBInit->Sim = (LPS25HB_SPIMode_et) (tmp & LPS25HB_SIM_MASK );

	ReadReg(LPS25HB_CTRL_REG2, 1, &tmp);
	pxLPS25HBInit->AutoZero = (LPS25HB_State_et) ((tmp & LPS25HB_RESET_AZ_MASK ) >> LPS25HB_AUTO_ZERO_BIT);
}

/**
* @brief   Set Interrupt Configuration
* @param    Struct to empty with the chosen tmp
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB::Set_InterruptConfig(LPS25HB_InterruptTypeDef_st* pLPS25HBInt)
{
	Set_InterruptActiveLevel(pLPS25HBInt->INT_H_L);
	Set_InterruptOutputType(pLPS25HBInt->PP_OD);
	Set_InterruptControlConfig(pLPS25HBInt->OutputSignal_INT1);
	if (pLPS25HBInt->OutputSignal_INT1 == LPS25HB_DATA) {
		Set_InterruptDataConfig(pLPS25HBInt->DataInterrupt_INT1);
	}
	LatchInterruptRequest(pLPS25HBInt->LatchIRQ);
	Set_PressureThreshold(pLPS25HBInt->fP_threshold);

	/*DIFF_EN bit is used to enable the circuitry for the computing of differential pressure output.*/
	/*It is suggested to turn on the circuitry only after the configuration of REF_P_x and THS_P_x.*/
	Set_InterruptDifferentialConfig(pLPS25HBInt->PressureInterrupt);
}

/**
* @brief   Get Interrupt Configuration
* @param   Struct to empty with the tmp read from sensor
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB::Get_InterruptConfig(LPS25HB_InterruptTypeDef_st* pLPS25HBInt)
{
	uint8_t tmp;

	ReadReg(LPS25HB_CTRL_REG3, 1, &tmp);

	pLPS25HBInt->INT_H_L = (LPS25HB_State_et) ((tmp & LPS25HB_INT_H_L_MASK ) >> LPS25HB_INT_H_L_BIT);
	pLPS25HBInt->PP_OD = (LPS25HB_OutputType_et) (tmp & LPS25HB_PP_OD_MASK );
	pLPS25HBInt->OutputSignal_INT1 = (LPS25HB_OutputSignalConfig_et) ((tmp & 0x03));

	if (pLPS25HBInt->OutputSignal_INT1 == LPS25HB_DATA) {
		ReadReg(LPS25HB_CTRL_REG4, 1, &tmp);
		pLPS25HBInt->DataInterrupt_INT1 = (LPS25HB_DataSignalType_et) (tmp &= 0x0F);
	}
	ReadReg(LPS25HB_INTERRUPT_CFG_REG, 1, &tmp);
	pLPS25HBInt->LatchIRQ = (LPS25HB_State_et) ((tmp & LPS25HB_LIR_MASK ) >> LPS25HB_LIR_BIT);
	pLPS25HBInt->PressureInterrupt = (LPS25HB_InterruptDiffConfig_et) (tmp & LPS25HB_PE_MASK );
	pLPS25HBInt->fP_threshold = Get_PressureThreshold();
}

/**
  * @brief   Set Fifo Configuration
* @param 	 Struct to empty with the chosen tmp
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::Set_FifoConfig(LPS25HB_FIFOTypeDef_st* pLPS25HBFIFO)
{
	if (pLPS25HBFIFO->FIFO_MODE == LPS25HB_FIFO_BYPASS_MODE) {
		/* FIFO Disable-> FIFO_EN bit=0 in CTRL_REG2*/
		Set_FifoModeUse(LPS25HB_DISABLE);
	} else {
		/* FIFO Enable-> FIFO_EN bit=1 in CTRL_REG2*/
		Set_FifoModeUse(LPS25HB_ENABLE);
		if (pLPS25HBFIFO->FIFO_MODE == LPS25HB_FIFO_MEAN_MODE) {
			Set_FifoSampleSize(pLPS25HBFIFO->MEAN_MODE_SAMPLE);
			if (pLPS25HBFIFO->FIFO_MEAN_DEC)
				Set_FifoMeanDecUse(LPS25HB_ENABLE);
		} else {
			if (pLPS25HBFIFO->WTM_INT) {
				Set_FifoWatermarkLevelUse(LPS25HB_ENABLE);
				Set_FifoWatermarkLevel(pLPS25HBFIFO->WTM_LEVEL);
			}
		}
	}

	Set_FifoMode(pLPS25HBFIFO->FIFO_MODE);
}

/**
  * @brief   Get Fifo Configuration
* @param 	 Struct to empty with the  tmp read from sensor
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
void LPS25HB::Get_FifoConfig(LPS25HB_FIFOTypeDef_st* pLPS25HBFIFO)
{
	uint8_t tmp;

	ReadReg(LPS25HB_CTRL_FIFO_REG, 1, &tmp);
	pLPS25HBFIFO->FIFO_MODE = (LPS25HB_FifoMode_et) (tmp & LPS25HB_FMODE_MASK );
	if (pLPS25HBFIFO->FIFO_MODE == LPS25HB_FIFO_MEAN_MODE) {
		pLPS25HBFIFO->MEAN_MODE_SAMPLE = (LPS25HB_FifoMeanModeSample_et) (tmp & LPS25HB_WTM_POINT_MASK );
		ReadReg(LPS25HB_CTRL_REG2, 1, &tmp);
		pLPS25HBFIFO->FIFO_MEAN_DEC = (LPS25HB_State_et) ((tmp & LPS25HB_FIFO_MEAN_MASK ) >> LPS25HB_FIFO_MEAN_BIT);
	} else {
		if (pLPS25HBFIFO->FIFO_MODE != LPS25HB_FIFO_BYPASS_MODE) {
			ReadReg(LPS25HB_CTRL_REG2, 1, &tmp);
			pLPS25HBFIFO->WTM_INT = (LPS25HB_State_et) ((tmp & LPS25HB_WTM_EN_MASK ) >> LPS25HB_WTM_EN_BIT);
			if (pLPS25HBFIFO->WTM_INT) {
				ReadReg(LPS25HB_CTRL_FIFO_REG, 1, &tmp);
				pLPS25HBFIFO->WTM_LEVEL = (uint8_t) (tmp & LPS25HB_WTM_POINT_MASK );
			}
		}
	}
}


/**
* @brief   Get the Reference Pressure tmp that is sum to the sensor output pressure
* @param   Buffer to empty with reference pressure tmp
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
int32_t LPS25HB::Get_ReferencePressure()
{
	uint8_t buffer[3];
	uint32_t tempVal = 0;
	int32_t raw_press;

	ReadReg(LPS25HB_REF_P_XL_REG, 3, buffer);

	/* Build the raw data */
	for (uint8_t i = 0; i < 3; i++)
		tempVal |= (((uint32_t) buffer[i]) << (8 * i));

	/* convert the 2's complement 24 bit to 2's complement 32 bit */
	if (tempVal & 0x00800000)
		tempVal |= 0xFF000000;

	raw_press = ((int32_t) tempVal);
	return (raw_press * 100) / 4096;
}


/**
* @brief  Check if the single measurement has completed.
* @param  tmp is set to 1, when the measure is completed
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
*/
uint8_t LPS25HB::IsMeasurementCompleted()
{
	uint8_t tmp;
	LPS25HB_DataStatus_st datastatus;

	ReadReg(LPS25HB_STATUS_REG, 1, &tmp);
	datastatus.TempDataAvailable = (uint8_t) (tmp & 0x01);
	datastatus.PressDataAvailable = (uint8_t) ((tmp & 0x02) >> LPS25HB_PDA_BIT);
	return (uint8_t) ((datastatus.PressDataAvailable) & (datastatus.TempDataAvailable));
}

/**
* @brief  Get the values of the last single measurement.
* @param  Pressure and temperature tmp
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB::Get_Measurement(LPS25HB_MeasureTypeDef_st *Measurement_Value)
{
	Measurement_Value->Tout = Get_Temperature();
	Measurement_Value->Pout = Get_Pressure();
}


/**
* @brief  De initialization function for LPS25HB.
*         This function put the LPS25HB in power down, make a memory boot and clear the data output flags.
* @param  None.
* @retval Error code [LPS25HB_OK, LPS25HB_ERROR].
*/
void LPS25HB::DeInit(void)
{
  LPS25HB_MeasureTypeDef_st Measurement_Value;

  /* LPS25HB in power down */
  Set_PowerDownMode(LPS25HB_RESET);

  /* Make LPS25HB Reset and Reboot */
  SwResetAndMemoryBoot();

  /* Dump of data output */
  Get_Measurement(& Measurement_Value);
}

