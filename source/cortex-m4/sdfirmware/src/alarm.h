/*
 * alarm.h
 *
 *  Created on: Jul 24, 2015
 *      Author: svassilev
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
