/*
 * ringbuffer.cpp
 *
 *  Created on: Feb 1, 2015
 *      Author: mstoilov
 */

#include "ringbuffer.h"

RingBuffer::RingBuffer(size_t sizebits)
	: wp_(0)
	, rp_(0)
	, bufsize_(1 << sizebits)
	, buffer_(new uint8_t[bufsize_])
{
}

RingBuffer::~RingBuffer()
{
	delete [] buffer_;
}

size_t RingBuffer::buffer_size()
{
	return bufsize_;
}

size_t RingBuffer::data_size()
{
	return (rp_ == wp_) ? 0 : ((bufsize_ + wp_ - rp_) % bufsize_);
}

size_t RingBuffer::data_space()
{
	return (rp_ == wp_) ? bufsize_ - 1 : ((bufsize_ + rp_ - wp_) % bufsize_ - 1);
}

size_t RingBuffer::write_size()
{
	unsigned int spacesize = data_space();
	unsigned int spacetoend = bufsize_ - wp_;
	return (spacesize < spacetoend) ? spacesize : spacetoend;
}

size_t RingBuffer::read_size()
{
	size_t datasize = data_size();
	size_t datatoend = bufsize_ - rp_;
	return (datasize < datatoend) ? datasize : datatoend;
}

size_t RingBuffer::line_size()
{
	size_t datasize = data_size();
	size_t rp = rp_;

	for (size_t i = 0; i < datasize; i++) {
		if (buffer_[(rp + i) % bufsize_] == '\n' )
			return i + 1;
	}
	return 0;
}


uint8_t* RingBuffer::get_read_ptr()
{
	return (buffer_ + rp_);
}

uint8_t* RingBuffer::get_write_ptr()
{
	return (buffer_ + wp_);
}

uint8_t* RingBuffer::get_buffer_ptr()
{
	return buffer_;
}

void RingBuffer::read_update(size_t size)
{
	rp_ = (rp_ + bufsize_ + size) % bufsize_;
}

void RingBuffer::write_update(size_t size)
{
	wp_ = (wp_ + bufsize_ + size) % bufsize_;
}

bool RingBuffer::empty()
{
	return wp_ == rp_ ? true : false;
}

void RingBuffer::reset()
{
	wp_ = rp_ = ((size_t)0);
}

void RingBuffer::reset_wp(size_t wp)
{
	wp_ = wp;
}

void RingBuffer::reset_rp(size_t rp)
{
	rp_ = rp;
}
