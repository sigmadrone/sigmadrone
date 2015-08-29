/*
 * altitudetracker.h
 *
 *  Created on: Aug 15, 2015
 *      Author: svassilev
 */

#ifndef ALTITUDETRACKER_H_
#define ALTITUDETRACKER_H_

#include <stdint.h>
#include "dronestate.h"

static const Altitude DEFAULT_FLIGHT_CEILING(Altitude::from_meters(5));

class AltitudeTracker
{
public:
	AltitudeTracker(
			const Altitude& flight_ceiling = DEFAULT_FLIGHT_CEILING,
			float safe_threshold = 0.7f);
	~AltitudeTracker();
	Altitude flight_ceiling() const;
	void flight_ceiling(const Altitude& altitude);
	Altitude highest_recorded_altitude() const;
	void update_state(DroneState& state);
	bool is_flight_ceiling_hit() const;

private:
	void clear_alarm();

	static const uint32_t ALARM_COUNT_THRESHOLD = 16;

	Altitude flight_ceiling_;
	Altitude starting_altitude_;
	Altitude current_altitude_;
	Altitude highest_altitude_;
	uint32_t alarm_count_;
	float safe_threshold_;
	bool flight_ceiling_hit_;
};


#endif /* ALTITUDETRACKER_H_ */
