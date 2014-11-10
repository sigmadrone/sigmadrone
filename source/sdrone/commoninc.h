/*
 * commoninc.h
 *
 *  Created on: May 25, 2013
 *      Author: svassilev
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
