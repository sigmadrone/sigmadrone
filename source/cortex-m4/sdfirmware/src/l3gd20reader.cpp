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

const Vector3f& L3GD20Reader::calculate_static_bias(uint32_t num_samples)
{
	static_bias_.clear();
	L3GD20::AxesDPS_t gyro_axes;
	gyro_.GetFifoAngRateDPS(&gyro_axes); // Drain the fifo
	for (uint32_t i = 0; i < num_samples; i++) {
		if (!wait_for_data(TimeSpan::from_milliseconds(100))) {
			static_bias_.clear();
			break;
		}
		static_bias_ += read_data(1);
	}
	static_bias_ /= static_cast<float>(num_samples);
	return static_bias_;
}


const Vector3f& L3GD20Reader::bias() const
{
	return static_bias_;
}

Vector3f L3GD20Reader::read_data(uint8_t watermark)
{
	Vector3f gyro_data;
	uint8_t gyro_samples = gyro_.GetFifoSourceReg() & 0x1F;
	if (gyro_samples >= watermark) {
		L3GD20::AxesDPS_t axes;
		for (size_t i = 0; i < gyro_samples; ++i) {
			gyro_.GetAngRateDPS(&axes);
			gyro_data += Vector3f(axes.AXIS_X, axes.AXIS_Y, axes.AXIS_Z);
		}
		gyro_data /= gyro_samples;
	}
	return axes_align_ * gyro_data;
}

void L3GD20Reader::init_gyro(uint8_t watermark)
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
	gyro_.SetODR(L3GD20::ODR_760Hz_BW_30);
}

bool L3GD20Reader::wait_for_data(const TimeSpan time_to_wait)
{
	uint32_t msg;
	return xQueueReceive(queue_handle_, &msg, ( TickType_t ) portTICK_PERIOD_MS * time_to_wait.milliseconds());
}

void L3GD20Reader::enable_disable_int2(bool enable)
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
