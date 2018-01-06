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

#include "l3gd20reader.h"

L3GD20Reader::L3GD20Reader(L3GD20& gyro, PinName gyro_int2_pin, const Matrix3f& axes_align) :
	gyro_(gyro),
	gyro_int2_pin_(gyro_int2_pin, DigitalIn::PullNone, DigitalIn::InterruptRising),
	axes_align_(axes_align)
{
	queue_handle_ = xQueueCreate(10, sizeof(uint32_t));
	gyro_int2_pin_.callback(this, &L3GD20Reader::gyro_isr);
}

const Vector3f& L3GD20Reader::calculate_static_bias_filtered(size_t num_samples)
{
	size_t count = 0;
	LowPassFilter<Vector3f, float> lpf(0.99, 0.01);

	static_bias_.clear();
	L3GD20::AxesDPS_t gyro_axes;
	for (int i = 0; i < 32; i++) {
		gyro_.GetFifoAngRateDPS(&gyro_axes); // Drain the fifo
	}

#if 0
	while (count < 100 && wait_for_data(TimeSpan::from_milliseconds(100))) {
		while (size()) {
			lpf.do_filter(read_sample());
			count++;
		}
	}
	count = 0;
	while (count < num_samples && wait_for_data(TimeSpan::from_milliseconds(100))) {
		while (size()) {
			static_bias_ += lpf.do_filter(read_sample());
			count++;
		}
	}
	static_bias_ /= static_cast<float>(count);
	return static_bias_;
#else
	while (count < num_samples && wait_for_data(TimeSpan::from_milliseconds(100))) {
		while (size()) {
			static_bias_ += lpf.do_filter(read_sample());
			count++;
		}
	}
	static_bias_ = lpf.output();
	return static_bias_;
#endif
}

const Vector3f& L3GD20Reader::bias() const
{
	return static_bias_;
}

size_t L3GD20Reader::size()
{
	return gyro_.GetFifoSourceFSS();
}

Vector3f L3GD20Reader::read_sample()
{
	Vector3f gyro_data;
	uint8_t gyro_samples = gyro_.GetFifoSourceReg() & 0x1F;
	if (gyro_samples) {
		L3GD20::AxesDPS_t axes;
		gyro_.GetAngRateDPS(&axes);
		gyro_data = Vector3f(axes.AXIS_X, axes.AXIS_Y, axes.AXIS_Z);
	}
	return axes_align_ * gyro_data;
}


void L3GD20Reader::init(uint8_t watermark)
{
	gyro_.SetMode(L3GD20::NORMAL);
	gyro_.SetFullScale(L3GD20::FULLSCALE_500);
	gyro_.SetBDU(L3GD20::MEMS_ENABLE);
	gyro_.SetWaterMark(watermark);
	gyro_.FIFOModeEnable(L3GD20::FIFO_STREAM_MODE);
	gyro_.SetInt2Pin(0);
	gyro_.SetInt2Pin(L3GD20_WTM_ON_INT2_ENABLE| L3GD20_OVERRUN_ON_INT2_ENABLE);
	gyro_.SetAxis(L3GD20_X_ENABLE|L3GD20_Y_ENABLE|L3GD20_Z_ENABLE);
	gyro_.HPFEnable(L3GD20::MEMS_ENABLE);
	gyro_.SetHPFMode(L3GD20::HPM_NORMAL_MODE_RES);
	gyro_.SetHPFCutOFF(L3GD20::HPFCF_0);
	gyro_.SetODR(L3GD20::ODR_760Hz_BW_50);
}

bool L3GD20Reader::wait_for_data(const TimeSpan time_to_wait)
{
	uint32_t msg;
	return xQueueReceive(queue_handle_, &msg, ( TickType_t ) portTICK_PERIOD_MS * time_to_wait.milliseconds());
}

void L3GD20Reader::enable_int2(bool enable)
{
	gyro_.SetInt2Pin(enable ? L3GD20_WTM_ON_INT2_ENABLE| L3GD20_OVERRUN_ON_INT2_ENABLE : 0);
}

void L3GD20Reader::gyro_isr()
{
	if (gyro_int2_pin_) {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		uint32_t msg = 1;
		xQueueSendFromISR(queue_handle_, &msg, &xHigherPriorityTaskWoken);
		portEND_SWITCHING_ISR(true);
	}
}
