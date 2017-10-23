#include "lsm303reader.h"

LSM303Reader::LSM303Reader(LSM303D& acc, PinName int2_pin, const Matrix3f& axes_align)
	: acc_(acc)
	, int2_pin_(int2_pin, DigitalIn::PullNone, DigitalIn::InterruptRising)
	, axes_align_(axes_align)
{
}

Vector3f LSM303Reader::read_sample_acc()
{
	LSM303D::AxesAcc_t axes = {0,0,0};
	acc_.GetAcc(&axes);
	return axes_align_ * Vector3d(axes.AXIS_X, axes.AXIS_Y, axes.AXIS_Z);
}

Vector3f LSM303Reader::read_sample_mag()
{
	LSM303D::AxesAcc_t axes = {0,0,0};
	acc_.GetMag(&axes);
	Vector3f mag_vector = mag_filter_.do_filter(axes_align_ * Vector3f(axes.AXIS_X, axes.AXIS_Y, axes.AXIS_Z));
	mag_calibrator_.add_reading(mag_vector);
	return mag_calibrator_.calibrate_reading(mag_vector);
}


size_t LSM303Reader::size()
{
	return acc_.GetFifoSourceFSS();
}

void LSM303Reader::init(uint8_t watermark)
{
	acc_.SetHPFMode(LSM303D::HPM_NORMAL_MODE_RES);
	acc_.SetFilterDataSel(LSM303D::MEMS_DISABLE);
	acc_.SetODR(LSM303D::ODR_1600Hz);
	acc_.SetFullScale(LSM303D::FULLSCALE_4);
	acc_.SetAntiAliasingBandwidth(LSM303D::ABW_194_HZ);
	acc_.SetAxis(LSM303D::X_ENABLE | LSM303D::Y_ENABLE | LSM303D::Z_ENABLE);
	acc_.FIFOModeSet(LSM303D::FIFO_STREAM_MODE);
	acc_.SetThreshold(watermark);
	acc_.SetODR_M(LSM303D::ODR_100Hz_M);
	acc_.SetFullScaleMag(LSM303D::FULLSCALE_2_GA);
	acc_.SetModeMag(LSM303D::CONTINUOUS_MODE);
}

void LSM303Reader::enable_int2(bool enable)
{
	if (enable) {
		acc_.SetInt2Pin(LSM303D_INT2_OVERRUN_ENABLE|LSM303D_INT2_FTH_ENABLE);
		int2_pin_.callback(this, &LSM303Reader::isr);
	} else {
		acc_.SetInt2Pin(0);
		int2_pin_.callback(NULL);
	}
}

void LSM303Reader::isr()
{

}
