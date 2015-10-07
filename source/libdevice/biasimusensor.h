/*
 *  Sigmadrone
 *  Copyright (c) 2013-2015 The Sigmadrone Developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Martin Stoilov <martin@sigmadrone.org>
 *  Svetoslav Vassilev <svassilev@sigmadrone.org>
 */
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
