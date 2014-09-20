#ifndef FILESENSOR_H_
#define FILESENSOR_H_

#include <pthread.h>

class file_sensor
{
public:
	file_sensor(const std::string& filename, unsigned int trottle = 0, double scale = 1.0);
	~file_sensor();
	void open();
	void read(double &value);
	double read();

protected:
	static void *file_sensor_thread(void*);
	void update_value();

protected:
	std::string filename_;
	unsigned int trottle_;
	unsigned int trottle_counter_;
	double cachedvalue_;
	double scale_;
	bool updating_;
	pthread_cond_t cond_;
	pthread_mutex_t mutex_;
	pthread_attr_t detached_;
};

#endif
