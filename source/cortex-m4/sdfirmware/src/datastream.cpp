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
#include "datastream.h"

DataStream::DataStream(size_t queuesiz)
	: serial_(0)
	, queuesiz_(queuesiz)
{

}

DataStream::~DataStream()
{

}

void DataStream::set_gyroscope(const Vector3f& value)
{
	entry_.gyro_ = value;
}

void DataStream::set_accelerometer(const Vector3f& value)
{
	entry_.accel_ = value;
}

void DataStream::set_correction_torque(const Vector3f& value)
{
	entry_.correction_torque_ = value;
}

void DataStream::set_attitude(const QuaternionF& value)
{
	entry_.attitude_ = value;
}

void DataStream::set_target_attitude(const QuaternionF& value)
{
	entry_.target_attitude_ = value;
}


/*
 * Serialize the current DataStreamEntry and reset its value to default ones.
 * The set functions for the entry values can be called multiple times
 * between commit, but only the last value will be serialized in the queue
 * during the commit call.
 */
void DataStream::commit()
{
	entry_.serial_ = serial_++;
	queue_.push_back(entry_);
	if (queue_.size() > queuesiz_)
		queue_.pop_front();
	entry_.reset();
}

bool DataStream::retrieve(DataStreamEntry& entry)
{
	if (!queue_.size())
		return false;
	entry = *queue_.begin();
	queue_.pop_front();
	return true;
}
