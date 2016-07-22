/**
  ******************************************************************************
  * @file    USB_Device/MSC_Standalone/Src/usbd_storage.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    26-June-2014
  * @brief   Memory management layer
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_storage.h"
#include "assert.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define STORAGE_LUN_NBR                  1  
#define STORAGE_BLK_NBR                  0x10000  
#define STORAGE_BLOCK_SIZE                  0x200

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* USB Mass storage Standard Inquiry Data */
int8_t STORAGE_Inquirydata[] = { /* 36 */
  /* LUN 0 */
  0x00,		
  0x00, //0x80,	/*RMB - removable media*/
  0x03, //Version: 0x02 - obsolete, 0x03 - SPC, 0x04 - SPC-2
  0x02, //response data format - 0x2 - conforms to the scsi standard
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,	
  0x00,
  'S', 'i', 'g', 'm', 'a', ' ', ' ', ' ', /* Manufacturer: 8 bytes  */
  'D', 'r', 'o', 'n', 'e', ' ', 'F', 'l',      /* Product     : 16 Bytes */
  'a', 's', 'h', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '2','0',                           /* Version     : 4 Bytes  */
}; 

/* Private function prototypes -----------------------------------------------*/
int8_t STORAGE_Init(uint8_t lun);
int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
int8_t STORAGE_IsReady(uint8_t lun);
int8_t STORAGE_IsWriteProtected(uint8_t lun);
int8_t STORAGE_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
int8_t STORAGE_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
int8_t STORAGE_GetMaxLun(void);

USBD_StorageTypeDef USBD_DISK_fops = {
  STORAGE_Init,
  STORAGE_GetCapacity,
  STORAGE_IsReady,
  STORAGE_IsWriteProtected,
  STORAGE_Read,
  STORAGE_Write,
  STORAGE_GetMaxLun,
  STORAGE_Inquirydata, 
};
/* Private functions ---------------------------------------------------------*/

static char bit_bucket[STORAGE_BLOCK_SIZE * 16]; // TODO: should be backed up by actual flash

/**
  * @brief  Initailizes the storage unit (medium)       
  * @param  lun: Logical unit number
  * @retval Status (0 : Ok / -1 : Error)
  */
int8_t STORAGE_Init(uint8_t lun)
{
	assert(0 == lun);
	memset(bit_bucket,0,sizeof(bit_bucket));
	bit_bucket[510] = 0x55;
	bit_bucket[511] = 0xaa;
	return 0;
}

/**
  * @brief  Returns the medium capacity.      
  * @param  lun: Logical unit number
  * @param  block_num: Number of total block number
  * @param  block_size: Block size
  * @retval Status (0: Ok / -1: Error)
  */
int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
	assert(0 == lun);
	*block_num =  128*1024*1024/STORAGE_BLOCK_SIZE;// sizeof(bit_bucket)/STORAGE_BLOCK_SIZE; // ?? why - 1 in the original code
	*block_size = STORAGE_BLOCK_SIZE;
	return 0;
}

/**
  * @brief  Checks whether the medium is ready.  
  * @param  lun: Logical unit number
  * @retval Status (0: Ok / -1: Error)
  */
int8_t STORAGE_IsReady(uint8_t lun)
{
	assert(0 == lun);
	return 0;
}

/**
  * @brief  Checks whether the medium is write protected.
  * @param  lun: Logical unit number
  * @retval Status (0: write enabled / -1: otherwise)
  */
int8_t STORAGE_IsWriteProtected(uint8_t lun)
{
	assert(0 == lun);
	return 0;
}

/**
  * @brief  Reads data from the medium.
  * @param  lun: Logical unit number
  * @param  blk_addr: Logical block address
  * @param  blk_len: Blocks number
  * @retval Status (0: Ok / -1: Error)
  */
int8_t STORAGE_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  int8_t ret = 0;
  assert(0 == lun);
  if (blk_addr + blk_len  <= sizeof(bit_bucket) / STORAGE_BLOCK_SIZE)
  {
	  memcpy(buf, bit_bucket + blk_addr * STORAGE_BLOCK_SIZE, blk_len * STORAGE_BLOCK_SIZE);
	  ret = 0;
  }
  else
  {
	  ret = 0;
  }
  return ret;
}

/**
  * @brief  Writes data into the medium.
  * @param  lun: Logical unit number
  * @param  blk_addr: Logical block address
  * @param  blk_len: Blocks number
  * @retval Status (0 : Ok / -1 : Error)
  */
int8_t STORAGE_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
	  int8_t ret = 0;//-1;
	  assert(0 == lun);
	  if (blk_addr + blk_len  <= sizeof(bit_bucket) / STORAGE_BLOCK_SIZE)
	  {
		  memcpy(bit_bucket + blk_addr * STORAGE_BLOCK_SIZE, buf, blk_len * STORAGE_BLOCK_SIZE);
		  ret = 0;
	  }
	  else
	  {
		  ret = 0;
	  }
	  return ret;
}

/**
  * @brief  Returns the Max Supported LUNs.   
  * @param  None
  * @retval Lun(s) number
  */
int8_t STORAGE_GetMaxLun(void)
{
	return(STORAGE_LUN_NBR - 1);
}
 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

