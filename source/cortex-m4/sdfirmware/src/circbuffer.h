/*
 * circbuffer.h
 *
 *  Created on: Feb 1, 2015
 *      Author: mstoilov
 */

#ifndef CIRCBUFFER_H_
#define CIRCBUFFER_H_

#include <cstdint>
#include <cstddef>

class CircBuffer
{
public:
	CircBuffer(size_t sizebits);
	~CircBuffer();
	size_t buffer_size();
	size_t data_size();
	size_t data_space();
	size_t write_size();
	size_t read_size();
	uint8_t *get_read_ptr();
	uint8_t *get_write_ptr();
	uint8_t *get_buffer_ptr();
	void read_update(size_t size);
	void write_update(size_t size);
	void reset();
	bool empty();
	void reset_wp(size_t wp);
	void reset_rp(size_t rp);

public:
	volatile size_t wp_;
	volatile size_t rp_;
	size_t bufsize_;
	uint8_t *buffer_;
};

#endif /* CORTEX_M4_TEST_PRINTF_APPLICATION_CIRCBUFFER_H_ */
