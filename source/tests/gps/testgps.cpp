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
#include <string>
#include <iostream>
#include "libgps/TinyGPS.h"


bool decode_nmea_sentence(TinyGPS& gps, const char* nmea_sent)
{
	const char* cursor = nmea_sent;
	while (*cursor != '\0') {
		if (gps.encode(*cursor++)) {
			return true;
		}
	}
	if (cursor != nmea_sent && *(cursor-1) != '\n') {
		if (gps.encode('\r') || gps.encode('\n')) {
			return true;
		}
	}
	return false;
}

void display_gps_data(TinyGPS& gps)
{
	float lat, longit;
	gps.f_get_position(&lat, &longit);
	std::cout << "LAT: " << lat << " LON: " << longit << " ALT: " << gps.f_altitude() << std::endl;
}

unsigned long millis()
{
	timespec now = {0, 0};
	clock_gettime(CLOCK_REALTIME, &now);
	return now.tv_sec*1000 + now.tv_nsec/1000000;
}


int main(int argc, char *argv[])
{
	if (argc > 2) {
		for (int i = 1; i < argc; i++) {
			TinyGPS gps;
			if (!decode_nmea_sentence(gps, argv[i])) {
				return -1;
			}
			display_gps_data(gps);
		}
	} else {
		const char* nmea_sent[] = {
			"$GPRMC,201547.000,A,3014.5527,N,09749.5808,W,0.24,163.05,040109,,*1A",
			"$GPGGA,201548.000,3014.5529,N,09749.5808,W,1,07,1.5,225.6,M,-22.5,M,18.8,0000*78",
			"$GPRMC,225446,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E*68",
			"$GPRMC,081836,A,3751.65,S,14507.36,E,000.0,360.0,130998,011.3,E*62"
		};
		for (size_t i = 0; i < sizeof(nmea_sent)/sizeof(nmea_sent[0]); i++) {
			TinyGPS gps;
			if (!decode_nmea_sentence(gps, nmea_sent[i])) {
				return -1;
			}
			display_gps_data(gps);
		}
	}
	return 0;
}
