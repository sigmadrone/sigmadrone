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
	FLIGHT_MODE_AUTO_ALTITUDE = 1,

	/*
	 * The firmware controls the altitude and keeps the drone inside a circle
	 * whose center lays at pre-defined latitude and longitude
	 */
	FLIGHT_MODE_LOITER = 2,

	/*
	 * The drone flies through pre-defined waypoints
	 */
	FLIGHT_MODE_MISSION = 3
};

enum PilotType {
	PILOT_TYPE_PID_LEGACY = 0,
	PILOT_TYPE_PID_NEW = 1
};


#endif /* FLIGHTDEFS_H_ */
