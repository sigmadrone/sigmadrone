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

#ifndef COMMONINC_H_
#define COMMONINC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <assert.h>
#include <sys/select.h>
#include <errno.h>
#include "axesdata.h"
#include "matrix.h"
#include "sdroneapi.h"

struct deltatime {
	deltatime() { clock_gettime(CLOCK_REALTIME, &last_time_); }
	double get() {
		timespec now;
		double dt;
		clock_gettime(CLOCK_REALTIME, &now);
		dt = (now.tv_sec - last_time_.tv_sec) +
				(now.tv_nsec - last_time_.tv_nsec)/1000000000.0;
		last_time_ = now;
		return dt;
	}
private:
	timespec last_time_;
};

#endif /* COMMONINC_H_ */
