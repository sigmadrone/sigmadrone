/*
 * ringbuffer.h
 *
 *  Created on: Feb 1, 2015
 *      Author: mstoilov
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <cstdint>
#include <cstddef>

class RingBuffer
{
public:
	RingBuffer(size_t sizebits);
	~RingBuffer();
	size_t buffer_size();
	size_t data_size();
	size_t data_space();
	size_t write_size();
	size_t read_size();
	size_t line_size();
	uint8_t *get_read_ptr();
	uint8_t *get_write_ptr();
	uint8_t *get_buffer_ptr();
	void read_update(size_t size);
	void write_update(size_t size);
	void reset();
	bool empty();
	void reset_wp(size_t wp);
	void reset_rp(size_t rp);

protected:
	volatile size_t wp_;
	volatile size_t rp_;
	size_t bufsize_;
	uint8_t *buffer_;
};

#endif /* CORTEX_M4_TEST_PRINTF_APPLICATION_RINGBUFFER_H_ */
