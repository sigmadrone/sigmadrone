#ifndef FILESENSOR_H_
#define FILESENSOR_H_

#include <pthread.h>

class file_sensor
{
public:
	file_sensor(const std::string& filename, unsigned int rate = 50, double scale = 1.0);
	~file_sensor();
	void open();
	void close();
	bool read(double &value) const;
	std::string filename() const;

protected:
	static void *file_sensor_thread(void*);
	void update_value();
	void update_thread();

protected:
	std::string filename_;
	unsigned int rate_;
	double scale_;
	mutable unsigned int trottle_counter_;
	mutable double cachedvalue_;
	mutable bool updating_;
	bool exit_;
	pthread_t thread_;
	pthread_cond_t cond_;
	pthread_mutex_t mutex_;
};

#endif
