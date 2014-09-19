#ifndef SAMPLER_H_
#define SAMPLER_H_

#include <string>
#include "imusensor.h"
#include "filesensor.h"
#include "matrix.h"

class sampler
{
public:
	imu_sensor gyr_;
	imu_sensor acc_;
	imu_sensor mag_;
	file_sensor bar_;

	struct data {
		Vector3d gyr3d_;
		Vector3d acc3d_;
		Vector3d mag3d_;
		double bar1d_;
		double dtime_;
		double time_;
		data() : bar1d_(0), dtime_(0), time_(0) {}
	} data;

public:
	sampler(const std::string& gyrdev, const std::string& accdev, const std::string& magdev, const std::string& bardev);
	~sampler();
	void init();
	void update();

protected:
	void update_time();
};

#endif
