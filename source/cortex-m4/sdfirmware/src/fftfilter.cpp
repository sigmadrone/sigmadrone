#include "fftfilter.h"

FftFilter::FftFilter(samples datasize, size_t nfilter)
	: datasize_(datasize)
	, nfilter_(nfilter)
{
	assert(nfilter_ < datasize / 2);

	switch(datasize) {
	case samples16:
		pcfft_ = &arm_cfft_sR_f32_len16;
		break;
	case samples32:
		pcfft_ = &arm_cfft_sR_f32_len32;
		break;
	case samples64:
		pcfft_ = &arm_cfft_sR_f32_len64;
		break;
	case samples128:
		pcfft_ = &arm_cfft_sR_f32_len128;
		break;
	case samples256:
		pcfft_ = &arm_cfft_sR_f32_len256;
		break;
	case samples512:
		pcfft_ = &arm_cfft_sR_f32_len512;
		break;
	case samples1024:
		pcfft_ = &arm_cfft_sR_f32_len1024;
		break;
	case samples2048:
		pcfft_ = &arm_cfft_sR_f32_len2048;
		break;
	default:
		throw(std::invalid_argument("Unsupported FFT size"));
	}
	buffer_ = new std::complex<float32_t>[datasize];
}

FftFilter::~FftFilter()
{
	delete [] buffer_;
}

void FftFilter::AddData(const Vector3f& data)
{
	if (data_.size() >= datasize_)
		data_.pop_front();
	data_.push_back(data);
}

/* FFT settings */
#define SAMPLES                    512             /* 256 real party and 256 imaginary parts */
#define FFT_SIZE                SAMPLES / 2        /* FFT size is always the same size as we have samples, so 256 in our case */

#define FFT_BAR_MAX_HEIGHT        120             /* 120 px on the LCD */

/* Global variables */
float32_t Input[SAMPLES];
float32_t Output[FFT_SIZE];

void FftFilter::dsp_fft(float32_t* input, bool inverse)
{
	/* Process the data through the CFFT/CIFFT module */
	arm_cfft_f32(pcfft_, input, inverse, 1);

}

void FftFilter::zero_buffer(std::complex<float32_t>* input, size_t begin, size_t end)
{
	for (size_t i = begin; i <= end; i++) {
		input[i] = 0;
	}
}

Vector3f FftFilter::GetFilteredDataDSP()
{
	Vector3f ret;
	size_t N = data_.size();
	if (N < datasize_)
		return Vector3f();

	for (size_t i = 0; i < N; i++)
		buffer_[i] = data_[i].x();
	dsp_fft((float32_t*)buffer_, false);
//	for (size_t i = 0; i < N; i++) {
//		printf("(%f, %f) ", buffer_[i].real(), buffer_[i].imag());
//	}
//	printf("\n");
	zero_buffer(buffer_, nfilter_ + 1, N - 1 - nfilter_);
//	for (size_t i = 0; i < N; i++) {
//		printf("(%f, %f) ", buffer_[i].real(), buffer_[i].imag());
//	}
//	printf("\n");
	dsp_fft((float32_t*)buffer_, true);
	ret.x() = buffer_[N-1].real();

	for (size_t i = 0; i < N; i++)
		buffer_[i] = data_[i].y();
	dsp_fft((float32_t*)buffer_, false);
	zero_buffer(buffer_, nfilter_ + 1, N - 1 - nfilter_);
	dsp_fft((float32_t*)buffer_, true);
	ret.y() = buffer_[N-1].real();

	for (size_t i = 0; i < N; i++)
		buffer_[i] = data_[i].z();
	dsp_fft((float32_t*)buffer_, false);
	zero_buffer(buffer_, nfilter_ + 1, N - 1 - nfilter_);
	dsp_fft((float32_t*)buffer_, true);
	ret.z() = buffer_[N-1].real();

	return ret;
}

Vector3f FftFilter::GetFilteredData()
{
	size_t N = data_.size();
	if (nfilter_ >= N / 2)
		return Vector3f();
	std::vector<Vector3c> y;
	Vector3c zero(0);
	y.resize(N, zero);

	std::vector<Vector3f> x(data_.begin(), data_.end());
	y[0] = Vector3f::fft(x, 0);
	for (size_t k = 1; k < nfilter_; k++) {
		y[N - k] = y[k] = Vector3f::fft(x, k);
//		y[N - k] = Vector3f::fft(x, N - k);
		y[N - k].x() = std::conj(y[N - k].x());
		y[N - k].y() = std::conj(y[N - k].y());
		y[N - k].z() = std::conj(y[N - k].z());
	}
	return Vector3f::ifft(y, N - 1);
}
