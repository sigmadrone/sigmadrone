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

#ifndef GEOLOCATION_H_
#define GEOLOCATION_H_

#include "units.h"
#
struct GeoLocation
{
	GeoLocation(Latitude lat, Longitude lon, Altitude alt = INVALID_ALTITUDE ) : lat_(lat), lon_(lon), altitude_(alt) {}
	GeoLocation() : lat_(INVALID_ANGLE), lon_(INVALID_ANGLE), altitude_(INVALID_ALTITUDE) {}
	Latitude latitude() const { return lat_; }
	Longitude longitude() const { return lon_; }
	Altitude altitude() const { return altitude_; }
	bool is_valid() const {
		return lon_.is_valid() && lat_.is_valid() && altitude() != INVALID_ALTITUDE;
	}
	static Distance distance_between(const GeoLocation& loc1, const GeoLocation& loc2);
private:
	Latitude lat_;
	Longitude lon_;
	Altitude altitude_;
};


#endif /* GEOLOCATION_H_ */
