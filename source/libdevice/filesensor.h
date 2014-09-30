#ifndef FILESENSOR_H_
#define FILESENSOR_H_

#include <pthread.h>

class file_sensor
{
public:
	file_sensor(const std::string& filename, unsigned int trottle = 0, double scale = 1.0);
	~file_sensor();
	void open();
	void read(double &value) const;
	double read() const;
	std::string filename() const;

protected:
	static void *file_sensor_thread(void*);
	void update_value();

protected:
	std::string filename_;
	unsigned int trottle_;
	double scale_;
	mutable unsigned int trottle_counter_;
	mutable double cachedvalue_;
	mutable bool updating_;
	mutable pthread_attr_t detached_;
	pthread_cond_t cond_;
	pthread_mutex_t mutex_;
};

#endif
