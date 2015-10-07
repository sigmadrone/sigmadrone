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
//	size_t line_size();
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
