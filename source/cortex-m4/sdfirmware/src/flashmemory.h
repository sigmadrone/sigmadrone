#ifndef FLASHMEMORY_H_
#define FLASHMEMORY_H_

#include "stm32f4xx_hal.h"

class FlashMemory {
public:
	FlashMemory(void *mem, size_t nbytes, size_t sector, size_t nsecors);
	~FlashMemory();

	void erase();
	void program(void *data, size_t nbytes);
	void* data();
	size_t size();

protected:
	void* mem_;
	size_t nbytes_;
	size_t sector_;
	size_t nsectors_;
};

#endif // FLASHMEMORY_H_
