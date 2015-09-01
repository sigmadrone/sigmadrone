#include <sstream>
#include <stdexcept>
#include "flashmemory.h"

/*
 * @param mem Pointer to the memory
 * @param nbytes The size of the memory region in bytes
 * @param sector The first sector of the flash the region occupies
 * @param nsectors The number of sectors the memory region occupies
 *
 * Note: All sectors will be erased together.
 *
 */
FlashMemory::FlashMemory(void *mem, size_t nbytes, size_t sector, size_t nsecors)
	: mem_(mem)
	, nbytes_(nbytes)
	, sector_(sector)
	, nsectors_(nsecors)
{
}

FlashMemory::~FlashMemory()
{
}

void FlashMemory::erase()
{
	uint32_t sectorerror = 0xFFFFFFFF;
	FLASH_EraseInitTypeDef eraseinfo;

	eraseinfo.TypeErase = FLASH_TYPEERASE_SECTORS;
	eraseinfo.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	eraseinfo.Sector = sector_;
	eraseinfo.NbSectors = nsectors_;

	HAL_FLASH_Unlock();
	if(HAL_FLASHEx_Erase(&eraseinfo, &sectorerror) != HAL_OK) {
		HAL_FLASH_Lock();
		std::stringstream oss;
		oss << "Failed to erase sector: " << sectorerror;
		throw std::runtime_error(oss.str());
	}
	HAL_FLASH_Lock();
}

void FlashMemory::program(void *data, size_t nbytes)
{
	uint8_t* memptr = (uint8_t*)mem_;
	uint8_t* dataptr = (uint8_t*)data;

	if (nbytes > nbytes_) {
		std::runtime_error("FlashMemory::program failed, the specified data size is too large.");
	}
	HAL_FLASH_Unlock();
	while (nbytes) {
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, (uint32_t)memptr, *dataptr) != HAL_OK) {
			HAL_FLASH_Lock();
			std::runtime_error("FlashMemory::program failed");
		}
	}
	HAL_FLASH_Lock();
}

void* FlashMemory::data()
{
	return mem_;
}

size_t FlashMemory::size()
{
	return nbytes_;
}
