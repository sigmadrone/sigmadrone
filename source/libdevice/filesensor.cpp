#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include "filesensor.h"

file_sensor::file_sensor(const std::string& filename, unsigned int trottle, double scale)
	: filename_(filename)
	, trottle_(trottle)
	, trottle_counter_(0)
	, cachedvalue_(0)
	, scale_(scale)
	, updating_(false)
{
	if (pthread_cond_init(&cond_, NULL) != 0)
		throw std::runtime_error("pthread_cond_init failed.");
	if (pthread_mutex_init(&mutex_, NULL) != 0)
		throw std::runtime_error("pthread_mutex_init failed.");
	if (pthread_attr_init(&detached_) != 0)
		throw std::runtime_error("pthread_attr_init failed.");
	if (pthread_attr_setdetachstate(&detached_, PTHREAD_CREATE_DETACHED) != 0)
		throw std::runtime_error("pthread_attr_setdetachstate failed.");

}

file_sensor::~file_sensor()
{
	pthread_mutex_lock(&mutex_);
	if (updating_)
		pthread_cond_wait(&cond_, &mutex_);
	pthread_mutex_unlock(&mutex_);
	pthread_cond_destroy(&cond_);
	pthread_mutex_destroy(&mutex_);
	pthread_attr_destroy(&detached_);
}

void file_sensor::open()
{
	std::ifstream file;
	file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
	file.open(filename_.c_str());
	file.close();
}

double file_sensor::read()
{
	if (updating_ || (trottle_ && (trottle_counter_++ % trottle_))) {
		return cachedvalue_;
	}
	pthread_t thread;
	updating_ = true;
	if (pthread_create(&thread, &detached_, file_sensor_thread, this) != 0) {
		updating_ = false;
		throw std::runtime_error("pthread_create failed.");
	}
	return cachedvalue_;
}

void file_sensor::read(double &value)
{
	value = read();
}

void file_sensor::update_value()
{
	std::string strval;
	std::ifstream file;
	file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
	file.open(filename_.c_str());
	file >> strval;
	file.close();
	cachedvalue_ = strtod(strval.c_str(), NULL) * scale_;
	pthread_mutex_lock(&mutex_);
	updating_ = false;
	pthread_cond_signal(&cond_);
	pthread_mutex_unlock(&mutex_);
}

void* file_sensor::file_sensor_thread(void *pthis)
{
	static_cast<file_sensor*>(pthis)->update_value();
	return NULL;
}
