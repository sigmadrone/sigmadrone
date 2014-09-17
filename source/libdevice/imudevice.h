#ifndef IMUDEVICE_H_
#define IMUDEVICE_H_

class imu_device
{
public:
	struct double3d_t {
		double x;
		double y;
		double z;
	};
	/**
	 * @filename The name of the device file. Example: /dev/gyro0
	 * @mode The mode passed to the open(2) function. Example: O_RDONLY|O_NONBLOCK
	 */
	imu_device(const char *filename, unsigned int mode);

	~imu_device();

	void open();

	/**
	 * buffer pointer to array of double3d_t elements
	 * size the number of elements of type double3d_t
	 *
	 * Returns the number of elements of type double3d_t put into the buffer
	 */
	ssize_t read(double3d_t* buffer, size_t size);

	/**
	 * Calculate the average values over the number of samples read
	 * by using the read_raw function.
	 *
	 * Return: true if x, y, z are set, false otherwise. Normally false
	 * is returned if the device is opened with O_NONBLOCK and no new data
	 * is available yet.
	 */
	bool read_average(double& x, double& y, double &z);

	void close();

	void enable();

	void disable();

	/**
	 * Get the sampling rate of the device.
	 */
	int get_rate();

	/**
	 * Set the sampling rate of the device.
	 */
	void set_rate(unsigned int rate);

	/**
	 * Set the number of samples to read in the FIFO before
	 * returning the data, for devices that have FIFO buffer
	 */
	int get_fifo_threshold();

	void set_fifo_threshold(unsigned int threshold);

	int get_full_scale();

	void set_full_scale(int scale);

	/**
	 * Reset the internal structures of the device driver and enable the device.
	 */
	void reset();

protected:
	int fd_;
	std::string filename_;
	unsigned int mode_;
};


#endif
