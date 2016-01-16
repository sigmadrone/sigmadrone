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

#ifndef DATASTREAM_H_
#define DATASTREAM_H_

#include <vector>
#include <deque>
#include "d3math.h"


struct DataStreamEntry {
	size_t serial_;
	Vector3f gyro_;
	Vector3f accel_;
	Vector3f correction_torque_;
	QuaternionF attitude_;
	QuaternionF target_attitude_;

	QuaternionF attitude_swing_;
	QuaternionF attitude_twist_;
	QuaternionF target_swing_;
	QuaternionF target_twist_;

	DataStreamEntry() : serial_(0) {}
	void reset() {
		serial_ = 0;
		gyro_ = Vector3f();
		accel_ = Vector3f();
		correction_torque_ = Vector3f();
		attitude_ = QuaternionF();
		target_attitude_ = QuaternionF();
	}

};

class DataStream {
public:
	DataStream(size_t queuesiz);
	~DataStream();

	void set_gyroscope(const Vector3f& value);
	void set_accelerometer(const Vector3f& value);
	void set_correction_torque(const Vector3f& value);
	void set_attitude(const QuaternionF& value);
	void set_target_attitude(const QuaternionF& value);
	void set_attitude_swing(const QuaternionF& value);
	void set_attitude_twist(const QuaternionF& value);
	void set_target_swing(const QuaternionF& value);
	void set_target_twist(const QuaternionF& value);

	void commit();
	bool retrieve(DataStreamEntry& entry);

protected:
	size_t serial_;
	size_t queuesiz_;
	DataStreamEntry entry_;
	std::deque<DataStreamEntry> queue_;
};

#endif /* DATASTREAM_H_ */
