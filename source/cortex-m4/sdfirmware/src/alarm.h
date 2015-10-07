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

#ifndef ALARM_H_
#define ALARM_H_

#include "timestamp.h"

struct Alarm {
	enum AlarmType {
		ALARM_NONE = 0,
		ALARM_SEV_WARNING = 1,
		ALARM_CEILING_HIT = ALARM_SEV_WARNING,
		ALARM_BATTERY_LEVEL_LOW = 2,
		ALARM_SEV_CRITICAL = 256,
		ALARM_RC_CHANNEL_DEAD = ALARM_SEV_CRITICAL,
		ALARM_BATTERY_LEVEL_CRITICAL = 257,
		ALARM_LAST
	};
	Alarm(AlarmType type = ALARM_NONE) : alarm_type_(type), data_(0), time_(TimeStamp::since_boot()) {}
	Alarm(AlarmType type, int64_t data) : alarm_type_(type), data_(data), time_(TimeStamp::since_boot()) {}
	int64_t data() const { return data_; }
	AlarmType type() const { return alarm_type_; }
	bool is_warning() const { return alarm_type_ >= ALARM_SEV_WARNING && alarm_type_ < ALARM_SEV_CRITICAL; }
	bool is_critical() const { return alarm_type_ >= ALARM_SEV_CRITICAL; }
	bool is_none() const { return alarm_type_ == ALARM_NONE; }
	TimeSpan when() const { return time_; }

	inline const char* to_string() const;
	inline const char* severity_to_string() const {
		if (is_none()) return "SEVERITY_NONE";
		if (is_warning()) return "SEVERITY_WARNING";
		return "SEVERITY_CRITICAL";
	}
private:
	AlarmType alarm_type_;
	int64_t data_;
	TimeSpan time_;
};

const char* Alarm::to_string() const {
	switch (alarm_type_) {
	case ALARM_CEILING_HIT: return "ALARM_CEILING_HIT";
	case ALARM_BATTERY_LEVEL_LOW: return "ALARM_BATTERY_LEVEL_LOW";
	case ALARM_RC_CHANNEL_DEAD: return "ALARM_RC_CHANNEL_DEAD";
	case ALARM_BATTERY_LEVEL_CRITICAL: return "ALARM_BATTERY_LEVEL_CRITICAL";
	case ALARM_NONE:
	default: return "ALARM_NONE";
	}
}

#endif /* ALARM_H_ */
