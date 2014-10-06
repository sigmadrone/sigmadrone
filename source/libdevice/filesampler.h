#ifndef FILE_SAMPLER_H_
#define FILE_SAMPLER_H_

#include <string>
#include <fstream>
#include "matrix.h"

class file_sampler
{
public:
	struct _data {
		Vector3d gyr3d_;
		Vector3d acc3d_;
		Vector3d mag3d_;
		double bar1d_;
		double dtime_;
		double time_;
		_data() : bar1d_(0), dtime_(0), time_(0) {}
	} data;

public:
	file_sampler(const std::string& file_name, bool simulate_dt=false);
	~file_sampler();
	bool update();
private:
	void reset_time();

private:
	std::ifstream file_;
	bool simulate_realtime_;
};

#endif
