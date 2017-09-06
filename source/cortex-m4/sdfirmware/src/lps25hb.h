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


#ifndef LPS25HB_H_
#define LPS25HB_H_

#include <stdint.h>
#include "spimaster.h"
#include "ipressuresensor.h"

/** @addtogroup Environmental_Sensor
  * @{
  */

/** @addtogroup LPS25HB_DRIVER
  * @{
  */

/* Exported Types -------------------------------------------------------------*/
/** @defgroup LPS25HB_Exported_Types
  * @{
  */

/**
* @brief  Error type.
 */
typedef enum {LPS25HB_OK = (uint8_t)0, LPS25HB_ERROR = !LPS25HB_OK} LPS25HB_Error_et;

/**
* @brief  Enable/Disable type.
 */
typedef enum {LPS25HB_DISABLE = (uint8_t)0, LPS25HB_ENABLE = !LPS25HB_DISABLE} LPS25HB_State_et;
#define IS_LPS25HB_State(MODE) ((MODE == LPS25HB_ENABLE) || (MODE == LPS25HB_DISABLE) )

/**
* @brief  Bit status type.
 */
typedef enum {LPS25HB_RESET = (uint8_t)0, LPS25HB_SET = !LPS25HB_RESET} LPS25HB_BitStatus_et;
#define IS_LPS25HB_BitStatus(MODE) ((MODE == LPS25HB_RESET) || (MODE == LPS25HB_SET))

/**
* @brief  Pressure average.
 */
typedef enum {
  LPS25HB_AVGP_8          = (uint8_t)0x00,         /*!< Internal average on 8 samples */
  LPS25HB_AVGP_32         = (uint8_t)0x01,         /*!< Internal average on 32 samples */
  LPS25HB_AVGP_128        = (uint8_t)0x02,         /*!< Internal average on 128 samples */
  LPS25HB_AVGP_512        = (uint8_t)0x03          /*!< Internal average on 512 sample */
}LPS25HB_Avgp_et;
#define IS_LPS25HB_AVGP(AVGP) ((AVGP == LPS25HB_AVGP_8) || (AVGP == LPS25HB_AVGP_32) || \
                              (AVGP == LPS25HB_AVGP_128) || (AVGP == LPS25HB_AVGP_512))

/**
* @brief  Temperature average.
 */
typedef enum {
LPS25HB_AVGT_8          = (uint8_t)0x00,        /*!< Internal average on 8 samples */
LPS25HB_AVGT_16         = (uint8_t)0x04,        /*!< Internal average on 16 samples */
LPS25HB_AVGT_32         = (uint8_t)0x08,        /*!< Internal average on 32 samples */
LPS25HB_AVGT_64         = (uint8_t)0x0C         /*!< Internal average on 64 samples */
}LPS25HB_Avgt_et;

#define IS_LPS25HB_AVGT(AVGT) ((AVGT == LPS25HB_AVGT_8) || (AVGT == LPS25HB_AVGT_16) || \
                              (AVGT == LPS25HB_AVGT_32) || (AVGT == LPS25HB_AVGT_64))

/**
* @brief  Output data rate configuration.
 */
typedef enum {

LPS25HB_ODR_ONE_SHOT  = (uint8_t)0x00,         /*!< Output Data Rate: one shot */
LPS25HB_ODR_1HZ       = (uint8_t)0x10,         /*!< Output Data Rate: 1Hz */
LPS25HB_ODR_7HZ       = (uint8_t)0x20,         /*!< Output Data Rate: 7Hz */
LPS25HB_ODR_12_5HZ    = (uint8_t)0x30,         /*!< Output Data Rate: 12.5Hz */
LPS25HB_ODR_25HZ      = (uint8_t)0x40          /*!< Output Data Rate: 25Hz */
} LPS25HB_Odr_et;

#define IS_LPS25HB_ODR(ODR) ((ODR == LPS25HB_ODR_ONE_SHOT) || (ODR == LPS25HB_ODR_1HZ) || \
                            (ODR == LPS25HB_ODR_7HZ) || (ODR == LPS25HB_ODR_12_5HZ)|| (ODR == LPS25HB_ODR_25HZ))
/**
 * @brief  LPS25HB Spi Mode configuration.
 */
typedef enum {
  LPS25HB_SPI_4_WIRE   =  (uint8_t)0x00,
  LPS25HB_SPI_3_WIRE   =  (uint8_t)0x01
} LPS25HB_SPIMode_et;

#define IS_LPS25HB_SPIMode(MODE) ((MODE == LPS25HB_SPI_4_WIRE) || (MODE == LPS25HB_SPI_3_WIRE))

/**
* @brief  Block data update.
 */

typedef enum {
LPS25HB_BDU_CONTINUOUS_UPDATE     =  (uint8_t)0x00,  /*!< Data updated continuously */
LPS25HB_BDU_NO_UPDATE             =  (uint8_t)0x04   /*!< Data updated after a read operation */
} LPS25HB_Bdu_et;
#define IS_LPS25HB_BDUMode(MODE) ((MODE == LPS25HB_BDU_CONTINUOUS_UPDATE) || (MODE == LPS25HB_BDU_NO_UPDATE))

/**
* @brief  Data Signal on INT1 pad control bits.
 */
typedef enum
{
  LPS25HB_DATA = (uint8_t)0x00,
  LPS25HB_P_HIGH = (uint8_t)0x01,
  LPS25HB_P_LOW = (uint8_t)0x02,
  LPS25HB_P_LOW_HIGH = (uint8_t)0x03
}LPS25HB_OutputSignalConfig_et;
#define IS_LPS25HB_OutputSignal(MODE) ((MODE == LPS25HB_DATA) || (MODE == LPS25HB_P_HIGH)||\
(MODE == LPS25HB_P_LOW) || (MODE == LPS25HB_P_LOW_HIGH))

/**
 * @brief  LPS25HB INT1 Interrupt pins configuration.
 */
typedef enum
{
  LPS25HB_DISABLE_DATA_INT = (uint8_t)0x00,
  LPS25HB_DATA_READY = (uint8_t)0x01,
  LPS25HB_OVR = (uint8_t)0x02,
  LPS25HB_WTM = (uint8_t)0x04,
  LPS25HB_EMPTY      = (uint8_t)0x08
}LPS25HB_DataSignalType_et;
#define IS_LPS25HB_DataSignal(MODE) ((MODE == LPS25HB_DISABLE_DATA_INT) ||(MODE == LPS25HB_EMPTY) || (MODE ==LPS25HB_WTM)||\
                                    (MODE == LPS25HB_OVR) || (MODE == LPS25HB_DATA_READY))

/**
 * @brief  LPS25HB Push-pull/Open Drain selection on Interrupt pads.
 */
typedef enum
{
  LPS25HB_PushPull = (uint8_t)0x00,
LPS25HB_OpenDrain  = (uint8_t)0x40
}LPS25HB_OutputType_et;
#define IS_LPS25HB_OutputType(MODE) ((MODE == LPS25HB_PushPull) || (MODE == LPS25HB_OpenDrain))

/**
 * @brief  LPS25HB Interrupt Differential Configuration.
 */
typedef enum
{
  LPS25HB_DISABLE_INT = (uint8_t)0x00,
  LPS25HB_PHE = (uint8_t)0x01,
  LPS25HB_PLE = (uint8_t)0x02,
  LPS25HB_PLE_PHE = (uint8_t)0x03
}LPS25HB_InterruptDiffConfig_et;

#define IS_LPS25HB_InterruptDiff(MODE) ((MODE == LPS25HB_DISABLE_INT) || (MODE ==LPS25HB_PHE)||\
                                      (MODE == LPS25HB_PLE) || (MODE == LPS25HB_PLE_PHE))

/**
 * @brief  LPS25HB Interrupt Differential Status.
 */

typedef struct
{
  uint8_t PH;          /*!< High Differential Pressure event occured */
  uint8_t PL;          /*!< Low Differential Pressure event occured */
  uint8_t IA;          /*!< One or more interrupt events have been  generated.Interrupt Active */
}LPS25HB_InterruptDiffStatus_st;


/**
 * @brief  LPS25HB Pressure and Temperature data status.
 */
typedef struct
{
  uint8_t TempDataAvailable;           /*!< Temperature data available bit */
  uint8_t PressDataAvailable;          /*!< Pressure data available bit */
  uint8_t TempDataOverrun;             /*!< Temperature data over-run bit */
  uint8_t PressDataOverrun;            /*!< Pressure data over-run bit */
}LPS25HB_DataStatus_st;


/**
 * @brief  LPS25HB Fifo Mode.
 */

typedef enum {
LPS25HB_FIFO_BYPASS_MODE             	   = (uint8_t)0x00,	/*!< The FIFO is disabled and empty. The pressure is read directly*/
LPS25HB_FIFO_MODE                           = (uint8_t)0x20,    /*!< Stops collecting data when full */
LPS25HB_FIFO_STREAM_MODE                    = (uint8_t)0x40,    /*!< Keep the newest measurements in the FIFO*/
LPS25HB_FIFO_TRIGGER_STREAMTOFIFO_MODE      = (uint8_t)0x60,    /*!< STREAM MODE until trigger deasserted, then change to FIFO MODE*/
LPS25HB_FIFO_TRIGGER_BYPASSTOSTREAM_MODE    = (uint8_t)0x80,    /*!< BYPASS MODE until trigger deasserted, then STREAM MODE*/
LPS25HB_FIFO_MEAN_MODE                      = (uint8_t)0xC0,    /*!< FIFO is used to generate a running average filtered pressure */
LPS25HB_FIFO_TRIGGER_BYPASSTOFIFO_MODE      = (uint8_t)0xE0     /*!< BYPASS mode until trigger deasserted, then FIFO MODE*/
} LPS25HB_FifoMode_et;

#define IS_LPS25HB_FifoMode(MODE) ((MODE == LPS25HB_FIFO_BYPASS_MODE) || (MODE ==LPS25HB_FIFO_MODE)||\
				  (MODE == LPS25HB_FIFO_STREAM_MODE) || (MODE == LPS25HB_FIFO_TRIGGER_STREAMTOFIFO_MODE)||\
                                  (MODE == LPS25HB_FIFO_TRIGGER_BYPASSTOSTREAM_MODE) || (MODE == LPS25HB_FIFO_MEAN_MODE)||\
                                  (MODE == LPS25HB_FIFO_TRIGGER_BYPASSTOFIFO_MODE))


/**
 * @brief  LPS25HB Fifo Mean Mode Sample Size.
 */

typedef enum {
  LPS25HB_FIFO_SAMPLE_2    = (uint8_t)0x01,	    /*!< 2 Fifo Mean Mode samples */
  LPS25HB_FIFO_SAMPLE_4    = (uint8_t)0x03,  		/*!< 4 Fifo Mean Mode samples */
  LPS25HB_FIFO_SAMPLE_8    = (uint8_t)0x07,     /*!< 8 Fifo Mean Mode samples */
  LPS25HB_FIFO_SAMPLE_16   = (uint8_t)0x0F,     /*!< 16 Fifo Mean Mode samples */
  LPS25HB_FIFO_SAMPLE_32   = (uint8_t)0x1F  	  /*!< 32 Fifo Mean Mode samples */
} LPS25HB_FifoMeanModeSample_et;

#define IS_LPS25HB_FifoMeanModeSample(MODE) ((MODE == LPS25HB_FIFO_SAMPLE_2) || (MODE ==LPS25HB_FIFO_SAMPLE_4)||\
                                          (MODE == LPS25HB_FIFO_SAMPLE_8) || (MODE == LPS25HB_FIFO_SAMPLE_16)||\
                                          (MODE == LPS25HB_FIFO_SAMPLE_32))
/**
 * @brief  LPS25HB Fifo Satus.
 */
typedef struct {
  uint8_t FIFO_LEVEL;          /*!< FIFO Stored data level t */
  uint8_t FIFO_EMPTY;          /*!< Empy FIFO bit.1 FIFO is empty	*/
  uint8_t FIFO_FULL;           /*!< Overrun bit status. 1 FIFO is full */
  uint8_t FIFO_WTM;            /*!< Watermark Status. 1 FIFO is equal or higher then wtm level.*/
}LPS25HB_FifoStatus_st;


/**
 * @brief  LPS25HB Configuration structure definition.
 */
typedef struct
{
LPS25HB_Avgp_et 				PressResolution;         	/*!< Pressure sensor resolution */
LPS25HB_Avgt_et 				TempResolution;     		/*!< Temperature sensor resolution */
LPS25HB_Odr_et  				OutputDataRate;      		/*!< Output Data Rate */
LPS25HB_Bdu_et  				BDU;                		/*!< Enable to inhibit the output registers update between the reading of upper and lower register parts.*/
LPS25HB_State_et   		        AutoZero;			/*! < Auto zero feature enabled.the actual pressure output is copied in the REF_P*/
LPS25HB_State_et 			Reset_AZ;   			/*!< Reset the Auto ZeroFunc. The daefualt RPDS value is copied in REF_P   */
LPS25HB_SPIMode_et 		        Sim;  			 	/*!< SPI Serial Interface Mode selection */
}LPS25HB_ConfigTypeDef_st;



/**
 * @brief  LPS25HB Interrupt structure definition .
 */
typedef struct {
LPS25HB_State_et 			INT_H_L;                /*!< Interrupt active high, low. Default value: 0 */
LPS25HB_OutputType_et 			PP_OD; 			/*!< Push-pull/open drain selection on interrupt pads. Default value: 0 */
LPS25HB_OutputSignalConfig_et 		OutputSignal_INT1;	/*!< Data signal on INT1 Pad: Data,Pressure High, Preessure Low,P High or Low*/
LPS25HB_DataSignalType_et       	DataInterrupt_INT1;	/*!< Interrupt Pin Config:DRDY, WTM, OVERRUN, EMPTY*/
LPS25HB_InterruptDiffConfig_et 		PressureInterrupt;	/*!< Interrupt differential Configuration: PL_E and/or PH_E*/
LPS25HB_State_et			LatchIRQ;		/*!< Latch Interrupt request in to INT_SOURCE reg*/
int16_t 				fP_threshold;		/*!< Threshold value for pressure interrupt generation*/
}LPS25HB_InterruptTypeDef_st;

/**
 * @brief  LPS25HB FIFO structure definition.
 */
typedef struct {
LPS25HB_FifoMode_et 	        FIFO_MODE;              /*!< Fifo Mode Selection */
LPS25HB_State_et		        WTM_INT; 		/*!< Enable/Disable the watermark interrupt*/
LPS25HB_State_et			FIFO_MEAN_DEC;		/*!< Enable/Disable 1 Hz ODR decimation */
LPS25HB_FifoMeanModeSample_et 	MEAN_MODE_SAMPLE;	/*!< FIFO Mean Mode Sample Size*/
uint8_t 			WTM_LEVEL;		/*!< FIFO threshold.Watermark level setting*/
}LPS25HB_FIFOTypeDef_st;

#define IS_LPS25HB_WtmLevel(LEVEL) ((LEVEL > 0) && (LEVEL <=31))

/**
 * @brief  LPS25HB Measure Type definition.
 */
typedef struct {
  int16_t Tout;
  int32_t Pout;
}LPS25HB_MeasureTypeDef_st;


/**
 * @brief  LPS25HB Driver Version Info structure definition.
 */
typedef struct {
  uint8_t   Major;
  uint8_t   Minor;
  uint8_t Point;
}LPS25HB_DriverVersion_st;


/**
* @brief  Bitfield positioning.
*/
#define LPS25HB_BIT(x) ((uint8_t)x)

/**
* @brief  I2C address.
*/

/*SDO/SA0 (pin5) is connected to the GND*/
//#define LPS25HB_ADDRESS  (uint8_t)0xB8

/* SD0/SA0(pin 5) is connected to the voltage supply*/
#define LPS25HB_ADDRESS  (uint8_t)0xBA //SA0=1

/**
  * @brief  Set the LPS25HB driver version.
*/
#define LPS25HB_DriverVersion_Major (uint8_t)1
#define LPS25HB_DriverVersion_Minor (uint8_t)0
#define LPS25HB_DriverVersion_Point (uint8_t)0

/**
  * @}
  */


/* Exported Constants ---------------------------------------------------------*/
 /** @defgroup LPS25HB_Exported_Constants
  * @{
  */


/**
 * @addtogroup LPS25HB_Register
 * @{
*/



 /**
 * @brief Device Identification register.
 * \code
 * Read
 * Default value: 0xBD
 * 7:0 This read-only register contains the device identifier that, for LPS25HB, is set to BDh.
 * \endcode
 */

#define LPS25HB_WHO_AM_I_REG         (uint8_t)0x0F

/**
* @brief Device Identification value.
*/
#define LPS25HB_WHO_AM_I_VAL         (uint8_t)0xBD


/**
 * @brief Reference Pressure  Register(LSB data)
 * \code
 * Read/write
 * Default value: 0x00
 * 7:0 REFL7-0: Lower part of the reference pressure value that
 *      is sum to the sensor output pressure.
 * \endcode
 */
#define LPS25HB_REF_P_XL_REG         (uint8_t)0x08


/**
 * @brief Reference Pressure Register (Middle data)
 * \code
 * Read/write
 * Default value: 0x00
 * 7:0 REFL15-8: Middle part of the reference pressure value that
 *      is sum to the sensor output pressure.
 * \endcode
 */
#define LPS25HB_REF_P_L_REG          (uint8_t)0x09

/**
 * @brief Reference Pressure Register (MSB data)
 * \code
 * Read/write
 * Default value: 0x00
 * 7:0 REFL23-16 Higest part of the reference pressure value that
 *      is sum to the sensor output pressure.
 * \endcode
 */
#define LPS25HB_REF_P_H_REG          (uint8_t)0x0A


/**
 * @brief Pressure and temperature resolution mode Register
 * \code
 * Read/write
 * Default value: 0x05
 * 7:4 RFU
 * 3:2 AVGT1-AVGT0: Select the temperature internal average.
 *      AVGT1 | AVGT0 | Nr. Internal Average
 *   ------------------------------------------------------
 *       0    |  0    |     8
 *       0    |  1    |     16
 *       1    |  0    |     32
 *       1    |  1    |     64

 *
 * 1:0 AVGP1-AVGP0: Select pressure internal average.
 *      AVGP1 |  AVGP0 | Nr. Internal Average
 *   ------------------------------------------------------
 *      0    |   0    |      8
 *      0    |   1    |      32
 *      1    |   0    |      128
 *      1    |   1    |      512
 * \endcode
 */
#define LPS25HB_RES_CONF_REG     (uint8_t)0x10

#define LPS25HB_AVGT_BIT         LPS25HB_BIT(2)
#define LPS25HB_AVGP_BIT         LPS25HB_BIT(0)

#define LPS25HB_AVGT_MASK        (uint8_t)0x0C
#define LPS25HB_AVGP_MASK        (uint8_t)0x03

/**
 * @brief Control Register 1
 * \code
 * Read/write
 * Default value: 0x00
 * 7 PD: power down control. 0 - power down mode; 1 - active mode
 * 6:4 ODR2, ODR1, ODR0: output data rate selection.
 *     ODR2  | ODR1  | ODR0  | Pressure output data-rate(Hz)  | Pressure output data-rate(Hz)
 *   ----------------------------------------------------------------------------------
 *      0    |  0    |  0    |         one shot               |         one shot
 *      0    |  0    |  1    |            1                   |            1
 *      0    |  1    |  0    |            7                   |            7
 *      0    |  1    |  1    |            12.5                |            12.5
 *      1    |  0    |  0    |            25                  |            25
 *      1    |  0    |  1    |         Reserved               |         Reserved
 *      1    |  1    |  0    |         Reserved               |         Reserved
 *      1    |  1    |  1    |         Reserved               |         Reserved
 *
 * 3 DIFF_EN: Interrupt circuit Enable. 0 - interrupt generation disable; 1 - interrupt generation enable
 * 2 BDU: block data update. 0 - continuous update; 1 - output registers not updated until MSB and LSB reading.
 * 1 RESET_AZ: Reset AutoZero Function. Reset Ref_P reg, set pressure to default value in RDPS reg.  0 - disable;1 - Reset
 * 0 SIM: SPI Serial Interface Mode selection. 0 - SPI 4-wire; 1 - SPI 3-wire
 * \endcode
 */
#define LPS25HB_CTRL_REG1      (uint8_t)0x20
#define LPS25HB_PD_BIT         LPS25HB_BIT(7)
#define LPS25HB_ODR_BIT        LPS25HB_BIT(4)
#define LPS25HB_DIFF_EN_BIT    LPS25HB_BIT(3)
#define LPS25HB_BDU_BIT        LPS25HB_BIT(2)
#define LPS25HB_RESET_AZ_BIT   LPS25HB_BIT(1)
#define LPS25HB_SIM_BIT        LPS25HB_BIT(0)

#define LPS25HB_PD_MASK        (uint8_t)0x80
#define LPS25HB_ODR_MASK       (uint8_t)0x70
#define LPS25HB_DIFF_EN_MASK   (uint8_t)0x08
#define LPS25HB_BDU_MASK       (uint8_t)0x04
#define LPS25HB_RESET_AZ_MASK  (uint8_t)0x02
#define LPS25HB_SIM_MASK       (uint8_t)0x01


/**
 * @brief Control  Register 2
 * \code
 * Read/write
 * Default value: 0x00
 * 7 BOOT:  Reboot memory content. 0: normal mode; 1: reboot memory content. Self-clearing upon completation
 * 6 FIFO_EN: FIFO Enable. 0: disable; 1:  enable
 * 5 WTM_EN:  FIFO Watermark level use. 0: disable; 1: enable
 * 4 FIFO_MEAN_DEC: Enable 1 HZ decimation 0: enable; 1: disable
 * 3 I2C Enable:  0: I2C Enable; 1: SPI disable
 * 2 SWRESET: Software reset. 0: normal mode; 1: SW reset. Self-clearing upon completation
 * 1 AUTO_ZERO: Autozero enable. 0: normal mode; 1: autozero enable.
 * 0 ONE_SHOT: One shot enable. 0: waiting for start of conversion; 1: start for a new dataset
 * \endcode
 */
#define LPS25HB_CTRL_REG2      (uint8_t)0x21
#define LPS25HB_BOOT_BIT       LPS25HB_BIT(7)
#define LPS25HB_FIFO_EN_BIT    LPS25HB_BIT(6)
#define LPS25HB_WTM_EN_BIT     LPS25HB_BIT(5)
#define LPS25HB_FIFO_MEAN_BIT  LPS25HB_BIT(4)
#define LPS25HB_I2C_BIT        LPS25HB_BIT(3)
#define LPS25HB_SW_RESET_BIT   LPS25HB_BIT(2)
#define LPS25HB_AUTO_ZERO_BIT  LPS25HB_BIT(1)
#define LPS25HB_ONE_SHOT_BIT   LPS25HB_BIT(0)

#define LPS25HB_BOOT_MASK      (uint8_t)0x80
#define LPS25HB_FIFO_EN_MASK   (uint8_t)0x40
#define LPS25HB_WTM_EN_MASK    (uint8_t)0x20
#define LPS25HB_FIFO_MEAN_MASK (uint8_t)0x10
#define LPS25HB_I2C_MASK       (uint8_t)0x08
#define LPS25HB_SW_RESET_MASK  (uint8_t)0x04
#define LPS25HB_AUTO_ZERO_MASK (uint8_t)0x02
#define LPS25HB_ONE_SHOT_MASK  (uint8_t)0x01


/**
 * @brief CTRL Reg3 Interrupt Control Register
 * \code
 * Read/write
 * Default value: 0x00
 * 7 INT_H_L: Interrupt active high, low. 0:active high; 1: active low.
 * 6 PP_OD: Push-Pull/OpenDrain selection on interrupt pads. 0: Push-pull; 1: open drain.
 * 5:2 Reserved
 * 1:0 INT1_S2, INT1_S1: data signal on INT1 pad control bits.
 *    INT1_S2  | INT1_S1  | INT1 pin
 *   ------------------------------------------------------
 *        0       |      0      |     Data signal
 *        0       |      1      |     Pressure high (P_high)
 *        1       |      0      |     Pressure low (P_low)
 *        1       |      1      |     P_low OR P_high
 * \endcode
 */
#define LPS25HB_CTRL_REG3      (uint8_t)0x22
#define LPS25HB_INT_H_L_BIT    LPS25HB_BIT(7)
#define LPS25HB_PP_OD_BIT      LPS25HB_BIT(6)
#define LPS25HB_INT1_S2_BIT    LPS25HB_BIT(1)
#define LPS25HB_INT1_S1_BIT    LPS25HB_BIT(0)

#define LPS25HB_INT_H_L_MASK   (uint8_t)0x80
#define LPS25HB_PP_OD_MASK     (uint8_t)0x40
#define LPS25HB_INT1_S2_MASK   (uint8_t)0x02
#define LPS25HB_INT1_S1_MASK   (uint8_t)0x01

/**
 * @brief CTRL Reg4 Interrupt Configuration Register
 * \code
 * Read/write
 * Default value: 0x00
 * 7:4 Reserved: Keep these bits at 0
 * 3 P1_EMPTY: Empty Signal on INT1 pin.
 * 2 P1_WTM: Watermark Signal on INT1 pin.
 * 1 P1_Overrunn:Overrun Signal on INT1 pin.
 * 0 P1_DRDY: Data Ready Signal on INT1 pin.
 * \endcode
 */
#define LPS25HB_CTRL_REG4       (uint8_t)0x23
#define LPS25HB_P1_EMPTY_BIT    LPS25HB_BIT(3)
#define LPS25HB_P1_WTM_BIT      LPS25HB_BIT(2)
#define LPS25HB_P1_OVERRUN_BIT  LPS25HB_BIT(1)
#define LPS25HB_P1_DRDY_BIT     LPS25HB_BIT(0)

#define LPS25HB_P1_EMPTY_MASK    (uint8_t)0x08
#define LPS25HB_P1_WTM_MASK      (uint8_t)0x04
#define LPS25HB_P1_OVERRUN_MASK  (uint8_t)0x02
#define LPS25HB_P1_DRDY_MASK     (uint8_t)0x01

 /**
 * @brief Interrupt Differential configuration Register
 * \code
 * Read/write
 * Default value: 0x00.
 * 7:3 Reserved.
 * 2 LIR: Latch Interrupt request into INT_SOURCE register. 0 - interrupt request not latched; 1 - interrupt request latched
 * 1 PL_E: Enable interrupt generation on differential pressure low event. 0 - disable; 1 - enable
 * 0 PH_E: Enable interrupt generation on differential pressure high event. 0 - disable; 1 - enable
 * \endcode
 */
#define LPS25HB_INTERRUPT_CFG_REG  (uint8_t)0x24
#define LPS25HB_LIR_BIT            LPS25HB_BIT(2)
#define LPS25HB_PL_E_BIT           LPS25HB_BIT(1)
#define LPS25HB_PH_E_BIT           LPS25HB_BIT(0)

#define LPS25HB_LIR_MASK           (uint8_t)0x04
#define LPS25HB_PL_E_MASK          (uint8_t)0x02
#define LPS25HB_PH_E_MASK          (uint8_t)0x01
#define LPS25HB_PE_MASK          (uint8_t)0x03


 /**
 * @brief Interrupt source Register (It is cleared by reading it)
 * \code
 * Read
 * Default value: 0x00.
 * 7:3 Reserved: Keep these bits at 0
 * 2 IA: Interrupt Active.0: no interrupt has been generated; 1: one or more interrupt events have been generated.
 * 1 PL: Differential pressure Low. 0: no interrupt has been generated; 1: Low differential pressure event has occurred.
 * 0 PH: Differential pressure High. 0: no interrupt has been generated; 1: High differential pressure event has occurred.
 * \endcode
 */
#define LPS25HB_INTERRUPT_SOURCE_REG   (uint8_t)0x25
#define LPS25HB_IA_BIT                 LPS25HB_BIT(2)
#define LPS25HB_PL_BIT                 LPS25HB_BIT(1)
#define LPS25HB_PH_BIT                 LPS25HB_BIT(0)

#define LPS25HB_IA_MASK               (uint8_t)0x04
#define LPS25HB_PL_MASK               (uint8_t)0x02
#define LPS25HB_PH_MASK               (uint8_t)0x01

/**
 * @brief  Status Register
 * \code
 * Read
 * Default value: 0x00
 * 7:6 Reserved: 0
 * 5 P_OR: Pressure data overrun. 0: no overrun has occurred; 1: new data for pressure has overwritten the previous one.
 * 4 T_OR: Temperature data overrun. 0: no overrun has occurred; 1: a new data for temperature has overwritten the previous one.
 * 3:2 Reserved: 0
 * 1 P_DA: Pressure data available. 0: new data for pressure is not yet available; 1: new data for pressure is available.
 * 0 T_DA: Temperature data available. 0: new data for temperature is not yet available; 1: new data for temperature is available.
 * \endcode
 */
#define LPS25HB_STATUS_REG         (uint8_t)0x27
#define LPS25HB_POR_BIT            LPS25HB_BIT(5)
#define LPS25HB_TOR_BIT            LPS25HB_BIT(4)
#define LPS25HB_PDA_BIT            LPS25HB_BIT(1)
#define LPS25HB_TDA_BIT            LPS25HB_BIT(0)

#define LPS25HB_POR_MASK           (uint8_t)0x20
#define LPS25HB_TOR_MASK           (uint8_t)0x10
#define LPS25HB_PDA_MASK           (uint8_t)0x02
#define LPS25HB_TDA_MASK           (uint8_t)0x01

/**
* @brief  Pressure data (LSB) register.
 * \code
 * Read
 * Default value: 0x00.(To be verified)
 * POUT7 - POUT0: Pressure data LSB (2's complement).
 * Pressure output data: Pout(hPA)=(PRESS_OUT_H & PRESS_OUT_L &
 * PRESS_OUT_XL)[dec]/4096.
 * \endcode
 */

#define LPS25HB_PRESS_OUT_XL_REG        (uint8_t)0x28
/**
* @brief  Pressure data (Middle part) register.
 * \code
 * Read
 * Default value: 0x80.
 * POUT15 - POUT8: Pressure data middle part (2's complement).
 * Pressure output data: Pout(hPA)=(PRESS_OUT_H & PRESS_OUT_L &
 * PRESS_OUT_XL)[dec]/4096.
 * \endcode
 */
#define LPS25HB_PRESS_OUT_L_REG        (uint8_t)0x29

/**
* @brief  Pressure data (MSB) register.
 * \code
 * Read
 * Default value: 0x2F.
 * POUT23 - POUT16: Pressure data MSB (2's complement).
 * Pressure output data: Pout(hPA)=(PRESS_OUT_H & PRESS_OUT_L &
 * PRESS_OUT_XL)[dec]/4096.
 * \endcode
 */
#define LPS25HB_PRESS_OUT_H_REG        (uint8_t)0x2A

/**
* @brief  Temperature data (LSB) register.
 * \code
 * Read
 * Default value: 0x00.
 * TOUT7 - TOUT0: temperature data LSB.
 * T(degC) = 42.5 + (Temp_OUTH & TEMP_OUT_L)[dec]/480.
 * \endcode
 */
#define LPS25HB_TEMP_OUT_L_REG         (uint8_t)0x2B

/**
* @brief  Temperature data (MSB) register.
 * \code
 * Read
 * Default value: 0x00.
 * TOUT15 - TOUT8: temperature data MSB.
 * T(degC) = 42.5 + (Temp_OUTH & TEMP_OUT_L)[dec]/480.
 * \endcode
 */
#define LPS25HB_TEMP_OUT_H_REG         (uint8_t)0x2C

/**
* @brief Threshold pressure (LSB) register.
 * \code
 * Read/write
 * Default value: 0x00.
 * 7:0 THS7-THS0: LSB Threshold pressure Low part of threshold value for pressure interrupt
 * generation. The complete threshold value is given by THS_P_H & THS_P_L and is
 * expressed as unsigned number. P_ths(hPA)=(THS_P_H & THS_P_L)[dec]/16.
 * \endcode
 */
#define LPS25HB_THS_P_LOW_REG           (uint8_t)0x30

/**
 * @brief Threshold pressure (MSB)
 * \code
 * Read/write
 * Default value: 0x00.
 * 7:0 THS15-THS8: MSB Threshold pressure. High part of threshold value for pressure interrupt
 * generation. The complete threshold value is given by THS_P_H & THS_P_L and is
 * expressed as unsigned number. P_ths(mbar)=(THS_P_H & THS_P_L)[dec]/16.
 * \endcode
 */
#define LPS25HB_THS_P_HIGH_REG         (uint8_t)0x31

/**
 * @brief FIFO control register
 * \code
 * Read/write
 * Default value: 0x00
 * 7:5 F_MODE2, F_MODE1, F_MODE0: FIFO mode selection.
 *     FM2   | FM1   | FM0   |    FIFO MODE
 *   ---------------------------------------------------
 *      0    |  0    |  0    | BYPASS MODE
 *      0    |  0    |  1    | FIFO MODE. Stops collecting data when full
 *      0    |  1    |  0    | STREAM MODE: Keep the newest measurements in the FIFO
 *      0    |  1    |  1    | STREAM MODE until trigger deasserted, then change to FIFO MODE
 *      1    |  0    |  0    | BYPASS MODE until trigger deasserted, then STREAM MODE
 *      1    |  0    |  1    | Reserved for future use
 *      1    |  1    |  0    | FIFO_MEAN MODE: Fifo is used to generate a running average filtered pressure
 *      1    |  1    |  1    | BYPASS mode until trigger deasserted, then FIFO MODE
 *
 * 4:0 WTM_POINT4-0 : FIFO threshold.Watermark level setting (0-31)
 * 4:0 WTM_POINT4-0 is FIFO Mean Mode Sample size if FIFO_MEAN MODE is used
 *     WTM_POINT4 | WTM_POINT4 | WTM_POINT4 |  WTM_POINT4 | WTM_POINT4 | Sample Size
 *   ----------------------------------------------------------------------------------
 *      0         |    0       |    0       |      0      |     1      |       2
 *      0         |    0       |    0       |      1      |     1      |       4
 *      0         |    0       |    1       |      1      |     1      |       8
 *      0         |    1       |    1       |      1      |     1      |       16
 *      1         |    1       |    1       |      1      |     1      |       32
 * other values operation not guaranteed
 * \endcode
 */
#define LPS25HB_CTRL_FIFO_REG          (uint8_t)0x2E
#define LPS25HB_FMODE_BIT              LPS25HB_BIT(5)
#define LPS25HB_WTM_POINT_BIT          LPS25HB_BIT(0)

#define LPS25HB_FMODE_MASK             (uint8_t)0xE0
#define LPS25HB_WTM_POINT_MASK         (uint8_t)0x1F

/**
 * @brief FIFO Status register
 * \code
 * Read
 * Default value: 0x00
 * 7 WTM_FIFO: Watermark status. 0:FIFO filling is lower than watermark level; 1: FIFO is equal or higher than watermark level.
 * 6 FULL_FIFO: Overrun bit status. 0 - FIFO not full; 1 -FIFO is full.
 * 5 EMPTY_FIFO: Empty FIFO bit. 0 - FIFO not empty; 1 -FIFO is empty.
 * 4:0 DIFF_POINT4-0: FIFOsStored data level.
 * \endcode
 */
#define LPS25HB_STATUS_FIFO_REG        (uint8_t)0x2F
#define LPS25HB_WTM_FIFO_BIT           LPS25HB_BIT(7)
#define LPS25HB_FULL_FIFO_BIT          LPS25HB_BIT(6)
#define LPS25HB_EMPTY_FIFO_BIT         LPS25HB_BIT(5)
#define LPS25HB_DIFF_POINT_BIT         LPS25HB_BIT(0)

#define LPS25HB_WTM_FIFO_MASK          (uint8_t)0x80
#define LPS25HB_FULL_FIFO_MASK         (uint8_t)0x40
#define LPS25HB_EMPTY_FIFO_MASK        (uint8_t)0x20
#define LPS25HB_DIFF_POINT_MASK        (uint8_t)0x1F

/**
 * @brief Pressure offset register  (LSB)
 * \code
 * Read/write
 * Default value: 0x00
 * 7:0 RPDS7-0:Pressure Offset for 1 point calibration after soldering.
 * This register contains the low part of the pressure offset value after soldering,for
 * differential pressure computing. The complete value is given by RPDS_L & RPDS_H
 * and is expressed as signed 2 complement value.
 * \endcode
 */
#define LPS25HB_RPDS_L_REG        (uint8_t)0x39

/**
 * @brief Pressure offset register (MSB)
 * \code
 * Read/write
 * Default value: 0x00
 * 7:0 RPDS15-8:Pressure Offset for 1 point calibration after soldering.
 * This register contains the high part of the pressure offset value after soldering (see description RPDS_L)
 * \endcode
 */
#define LPS25HB_RPDS_H_REG        (uint8_t)0x3A


class LPS25HB: public IPressureSensor {
protected:
	SPIMaster& spi_;
	uint8_t cs_;
	std::vector<GPIOPin> pins_;

public:
	LPS25HB(SPIMaster& spi, uint8_t cs, const std::vector<GPIOPin>& pins = {});
	~LPS25HB();

	uint8_t Get_DeviceID();
	void Get_DriverVersion(LPS25HB_DriverVersion_st *Version);
	void Set_Avg(LPS25HB_Avgp_et avgp, LPS25HB_Avgt_et avgt);
	void Activate(void);
	void DeActivate(void);
	void Set_AvgP(LPS25HB_Avgp_et avgp);
	void Set_AvgT(LPS25HB_Avgt_et avgt);
	LPS25HB_Avgp_et Get_AvgP();
	LPS25HB_Avgt_et Get_AvgT();
	void Set_Odr(LPS25HB_Odr_et odr);
	LPS25HB_Odr_et Get_Odr();
	void Set_PowerDownMode(LPS25HB_BitStatus_et pd);
	void Set_Bdu(LPS25HB_Bdu_et bdu);
	LPS25HB_Bdu_et Get_Bdu();
	void Set_InterruptCircuitEnable(LPS25HB_State_et diff_en);
	LPS25HB_State_et Get_InterruptCircuitEnable();
	void ResetAZ(void);
	void Set_SpiInterface(LPS25HB_SPIMode_et spimode);
	LPS25HB_SPIMode_et Get_SpiInterface();
	void Set_I2C(LPS25HB_State_et statei2c);
	void StartOneShotMeasurement(void);
	void Set_AutoZeroFunction(LPS25HB_BitStatus_et autozero);
	void SwReset(void);
	void MemoryBoot(void);
	void SwResetAndMemoryBoot(void);
	void Set_FifoModeUse(LPS25HB_State_et status);
	void Set_FifoWatermarkLevelUse(LPS25HB_State_et status);
	void Set_FifoMeanDecUse(LPS25HB_State_et status);
	void Set_InterruptActiveLevel(LPS25HB_State_et status);
	void Set_InterruptOutputType(LPS25HB_OutputType_et output);
	void Set_InterruptControlConfig(LPS25HB_OutputSignalConfig_et config);
	void Set_InterruptDataConfig(LPS25HB_DataSignalType_et signal);
	void Set_InterruptDifferentialConfig(LPS25HB_InterruptDiffConfig_et config);
	void LatchInterruptRequest(LPS25HB_State_et status);
	LPS25HB_InterruptDiffStatus_st Get_InterruptDifferentialEventStatus();
	LPS25HB_DataStatus_st Get_DataStatus();
	int32_t Get_RawPressure();
	int32_t Get_Pressure();
	float Get_PressureHpa();
	int16_t Get_RawTemperature();
	uint16_t Get_Temperature();
	float Get_TemperatureCelsius();
	int16_t Get_PressureThreshold();
	void Set_PressureThreshold(int16_t P_ths);
	void Set_FifoMode(LPS25HB_FifoMode_et fifomode);
	LPS25HB_FifoMode_et Get_FifoMode();
	LPS25HB_FifoStatus_st Get_FifoStatus();
	void Set_FifoWatermarkLevel(uint8_t wtmlevel);
	uint8_t Get_FifoWatermarkLevel();
	void Set_FifoSampleSize(LPS25HB_FifoMeanModeSample_et samplesize);
	LPS25HB_FifoMeanModeSample_et Get_FifoSampleSize();
	int16_t Get_PressureOffsetValue();
	void Set_GenericConfig(LPS25HB_ConfigTypeDef_st* pxLPS25HBInit);
	void Get_GenericConfig(LPS25HB_ConfigTypeDef_st* pxLPS25HBInit);
	void Set_InterruptConfig(LPS25HB_InterruptTypeDef_st* pLPS25HBInt);
	void Get_InterruptConfig(LPS25HB_InterruptTypeDef_st* pLPS25HBInt);
	void Set_FifoConfig(LPS25HB_FIFOTypeDef_st* pLPS25HBFIFO);
	void Get_FifoConfig(LPS25HB_FIFOTypeDef_st* pLPS25HBFIFO);
	int32_t Get_ReferencePressure();
	uint8_t IsMeasurementCompleted();
	void Get_Measurement(LPS25HB_MeasureTypeDef_st *Measurement_Value);
	void DeInit(void);


	/*********************Generic*********************/
	void ReadReg(uint8_t reg, uint16_t nbytes, uint8_t* data);
	void WriteReg(uint8_t reg, uint16_t nbytes, uint8_t *data);
	void UpdateReg(uint8_t reg, uint8_t mask, uint8_t data);
	void assert_failed(uint8_t* file, uint32_t line);


	/********************IPressureSensor*************/
	float read_pressure_hpa();
	float read_temperature_celsius();
	PressureSensorType get_pressure_sensor_type();
	bool is_fifo_empty();
};

#endif
