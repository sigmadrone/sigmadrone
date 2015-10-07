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
#ifndef _AXESDATA_H_
#define _AXESDATA_H_

#include <linux/types.h>


struct __short3d {
	__s16 x;
	__s16 y;
	__s16 z;
};

struct __int3d {
	__s32 x;
	__s32 y;
	__s32 z;
};

typedef enum {
	AXESDATA_OPMODE_STREAM = 0,
	AXESDATA_OPMODE_BYPASS,
} axesdata_opmode_t;

typedef struct __short3d short3d_t;
typedef struct __int3d int3d_t;

#define AXISDATA_IOC_MAGIC		'A'
#define AXISDATA_IOC_GETRATE		_IO(AXISDATA_IOC_MAGIC, 1)
#define AXISDATA_IOC_SETRATE		_IO(AXISDATA_IOC_MAGIC, 2)
#define AXISDATA_IOC_GETSCALE		_IO(AXISDATA_IOC_MAGIC, 3)
#define AXISDATA_IOC_SETSCALE		_IO(AXISDATA_IOC_MAGIC, 4)
#define AXISDATA_IOC_RESETFIFO		_IO(AXISDATA_IOC_MAGIC, 5)
#define AXISDATA_IOC_GETENABLED		_IO(AXISDATA_IOC_MAGIC, 6)
#define AXISDATA_IOC_SETENABLED		_IO(AXISDATA_IOC_MAGIC, 7)
#define AXISDATA_IOC_SETWATERMARK	_IO(AXISDATA_IOC_MAGIC, 8)
#define AXISDATA_IOC_GETWATERMARK	_IO(AXISDATA_IOC_MAGIC, 9)
#define AXISDATA_IOC_SETMODE		_IO(AXISDATA_IOC_MAGIC, 10)
#define AXISDATA_IOC_GETMODE		_IO(AXISDATA_IOC_MAGIC, 11)
#define AXISDATA_IOC_HPEN		_IO(AXISDATA_IOC_MAGIC, 12)
#define AXISDATA_IOC_HPM		_IO(AXISDATA_IOC_MAGIC, 13)

#endif
