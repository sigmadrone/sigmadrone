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

#ifndef FLIGHTDEFS_H_
#define FLIGHTDEFS_H_

enum FlightMode {
	/*
	 * Firmware keeps the drone leveled when there are no other commands
	 * from the pilot.
	 */
	FLIGHT_MODE_AUTO_LEVEL = 0,

	/*
	 * Just like FLIGHT_MODE_AUTO_LEVEL, but the firmware also maintains the
	 * altitude, where the throttle position controls the desired altitude
	 * relative to the take off point
	 */
	FLIGHT_MODE_ALTITUDE_HOLD = 1,

	/*
	 * The firmware controls the altitude and keeps the drone inside a circle
	 * whose center lays at pre-defined latitude and longitude
	 */
	FLIGHT_MODE_POSITION_HOLD = 2,

	/*
	 * The drone flies through pre-defined waypoints
	 */
	FLIGHT_MODE_MISSION = 3,

	FLIGHT_MODE_LAST = FLIGHT_MODE_MISSION
};

enum PilotType {
	PILOT_TYPE_PID_NEW = 0,
	PILOT_TYPE_UNKNOWN = 1,
};

inline const char* PilotTypeAsStr(PilotType pilot)
{
	if (PILOT_TYPE_PID_NEW == pilot) return "pid_pilot_new";
	return "pilot_unkown";
}

inline PilotType PilotTypeFromStr(const std::string& pilot)
{
	if (pilot == "pid_pilot_new")
		return PILOT_TYPE_PID_NEW;
	return PILOT_TYPE_UNKNOWN;
}

#endif /* FLIGHTDEFS_H_ */
