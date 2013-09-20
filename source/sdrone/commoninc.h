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
#include "dronedefs.h"
#include "matrix.h"
#include "sdroneapi.h"

#ifndef ARRAYSIZE
#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))
#endif

#endif /* COMMONINC_H_ */
