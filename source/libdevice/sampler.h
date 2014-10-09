#ifndef SAMPLER_H_
#define SAMPLER_H_

#include <string>
#include "biasimusensor.h"
#include "imusensor.h"
#include "filesensor.h"
#include "matrix.h"

class sampler
{
public:
	biasimu_sensor gyr_;
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
		bool gyr3d_upd_;
		bool acc3d_upd_;
		bool mag3d_upd_;
		bool bar1d_upd_;
		data() : bar1d_(0), dtime_(0), time_(0), gyr3d_upd_(false), acc3d_upd_(false), mag3d_upd_(false), bar1d_upd_(false) {}
	} data;

public:
	sampler(const std::string& gyrdev = "", const std::string& accdev = "", const std::string& magdev = "", const std::string& bardev = "");
	~sampler();
	void init();
	void update();

protected:
	void update_sensors();
	void update_time();
};

#endif
