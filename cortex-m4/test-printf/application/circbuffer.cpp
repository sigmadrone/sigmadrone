/*
 * circbuffer.cpp
 *
 *  Created on: Feb 1, 2015
 *      Author: mstoilov
 */

#include "circbuffer.h"

CircBuffer::CircBuffer(size_t sizebits)
	: wp_(0)
	, rp_(0)
	, bufsize_(1 << sizebits)
	, buffer_(new uint8_t(bufsize_))
{
}

CircBuffer::~CircBuffer()
{
	delete [] buffer_;
}

size_t CircBuffer::buffer_size()
{
	return bufsize_;
}

size_t CircBuffer::data_size()
{
	return (rp_ == wp_) ? 0 : ((bufsize_ + wp_ - rp_) % bufsize_);
}

size_t CircBuffer::data_space()
{
	return (rp_ == wp_) ? bufsize_ - 1 : ((bufsize_ + rp_ - wp_) % bufsize_ - 1);
}

size_t CircBuffer::write_size()
{
	unsigned int spacesize = data_space();
	unsigned int spacetoend = bufsize_ - wp_;
	return (spacesize < spacetoend) ? spacesize : spacetoend;
}

size_t CircBuffer::read_size()
{
	size_t datasize = data_size();
	size_t datatoend = bufsize_ - rp_;
	return (datasize < datatoend) ? datasize : datatoend;
}

uint8_t* CircBuffer::get_read_ptr()
{
	return (buffer_ + rp_);
}

uint8_t* CircBuffer::get_write_ptr()
{
	return (buffer_ + wp_);
}

uint8_t* CircBuffer::get_buffer_ptr()
{
	return buffer_;
}

void CircBuffer::read_update(size_t size)
{
	rp_ = (rp_ + bufsize_ + size) % bufsize_;
}

void CircBuffer::write_update(size_t size)
{
	wp_ = (wp_ + bufsize_ + size) % bufsize_;
}

bool CircBuffer::empty()
{
	return wp_ == rp_ ? true : false;
}

void CircBuffer::reset()
{
	wp_ = rp_ = ((size_t)0);
}

void CircBuffer::reset_wp(size_t wp)
{
	wp_ = wp % bufsize_;
}

void CircBuffer::reset_rp(size_t rp)
{
	rp_ = rp % bufsize_;
}
