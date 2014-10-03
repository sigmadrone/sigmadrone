#ifndef IMUSENSOR_H_
#define IMUSENSOR_H_

class imu_sensor
{
public:
	struct double3d_t {
		double x;
		double y;
		double z;
		double3d_t() : x(0), y(0), z(0) {}
	};
	enum open_mode_t {
		o_default = 0,
		o_nonblock = 1,
	};
	/**
	 * @filename The name of the device file. Example: /dev/gyro0
	 */
	imu_sensor(const std::string& filename, unsigned int trottle = 0, double scale = 1.0);

	virtual ~imu_sensor();

	/**
	 *
	 * @mode The mode used to open the device o_default corresponds to O_RDONLY, o_nonblock: O_RDONLY|O_NONBLOCK
	 */
	virtual void open(open_mode_t mode = o_default);

	/**
	 * Get device file name.
	 */
	virtual std::string filename() const;

	/**
	 * Check if device is open.
	 */
	virtual bool isopen() const;

	/**
	 * buffer pointer to array of double3d_t elements
	 * size the number of elements of type double3d_t
	 *
	 * Returns the number of elements of type double3d_t put into the buffer
	 */
	virtual size_t read(double3d_t* buffer, size_t size) const;

	/**
	 * Calculate the average values over the number of samples read
	 * by using the read_raw function.
	 *
	 * Return: true if x, y, z are set, false otherwise. Normally false
	 * is returned if the device is opened with O_NONBLOCK and no new data
	 * is available yet.
	 */
	virtual bool read_average(double& x, double& y, double &z) const;

	/**
	 * Calculate the average values over the number of samples read
	 * by using the read_raw function and then apply the current scale
	 * for the device.
	 *
	 * Return: true if x, y, z are set, false otherwise. Normally false
	 * is returned if the device is opened with O_NONBLOCK and no new data
	 * is available yet.
	 */
	virtual bool read_scaled_average(double& x, double& y, double &z) const;

	virtual void close();

	virtual void enable();

	virtual void disable();

	/**
	 * Get the sampling rate of the device.
	 */
	virtual int get_rate() const;

	/**
	 * Set the sampling rate of the device.
	 */
	virtual void set_rate(unsigned int rate);

	/**
	 * Set the number of samples to read in the FIFO before
	 * returning the data, for devices that have FIFO buffer
	 */
	virtual int get_fifo_threshold() const;

	virtual void set_fifo_threshold(unsigned int threshold);

	virtual int get_full_scale() const;

	virtual void set_full_scale(int scale);

	/**
	 * Reset the internal FIFO of the device.
	 */
	virtual void reset_fifo();

protected:
	int fd_;
	std::string filename_;
	unsigned int trottle_;
	mutable unsigned int trottle_counter_;
	double scale_;
};


#endif
