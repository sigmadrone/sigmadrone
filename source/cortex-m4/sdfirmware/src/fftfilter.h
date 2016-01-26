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

#ifndef FFTFILTER_H_
#define FFTFILTER_H_


#include <stdint.h>
#include <stdio.h>
#include <stdexcept>

#include "stm32f4xx_hal.h"
#include <stddef.h>
#include <stdint.h>
#include <deque>
#include <vector>
#include <deque>
#include <complex>
#include "d3math.h"
#include "arm_math.h"
#include "arm_const_structs.h"

class FftFilter {
public:
	enum samples {
		samples16 = 16,
		samples32 = 32,
		samples64 = 64,
		samples128 = 128,
		samples256 = 256,
		samples512 = 512,
		samples1024 = 1024,
		samples2048 = 2048,
	};
	FftFilter(samples datasize, size_t nfilter);
	~FftFilter();
	void AddData(const Vector3f& data);
	Vector3f GetFilteredData();
	Vector3f GetFilteredDataDSP();

protected:
	void dsp_fft(float32_t* input, bool inverse = false);
	void zero_buffer(std::complex<float32_t>* input, size_t begin, size_t end);

protected:
	samples datasize_;
	size_t nfilter_;
	std::complex<float32_t>* buffer_;
	std::deque<Vector3f> data_;
	const arm_cfft_instance_f32* pcfft_;
};

#endif
