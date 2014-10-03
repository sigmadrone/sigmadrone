#ifndef BIASIMUSENSOR_H_
#define BIASIMUSENSOR_H_

#include "imusensor.h"

class biasimu_sensor : public imu_sensor
{
public:
	biasimu_sensor(const std::string& filename, unsigned int trottle = 0, double scale = 1.0);
	~biasimu_sensor();

	virtual size_t read(double3d_t* buffer, size_t size) const;
	void bias_update(size_t nsamples = 32);

protected:
	double3d_t bias_;
};


#endif
