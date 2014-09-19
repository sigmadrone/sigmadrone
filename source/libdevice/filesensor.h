#ifndef FILESENSOR_H_
#define FILESENSOR_H_


class file_sensor
{
public:
	file_sensor(const std::string& filename, unsigned int trottle = 0, double scale = 1.0);
	~file_sensor();
	void open();
	void read(double &value);
	double read();

protected:
	std::string filename_;
	unsigned int trottle_;
	unsigned int trottle_counter_;
	double cachedvalue_;
	double scale_;
};

#endif
