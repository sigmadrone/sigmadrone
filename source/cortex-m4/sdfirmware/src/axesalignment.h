/*
 *  Sigmadrone
 *  Copyright (c) 2013-2017 The Sigmadrone Developers
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


#ifndef AXESALIGNMENT_H_
#define AXESALIGNMENT_H_

#include "d3math.h"
#include "droneframedefs.h"

#ifndef DRONE_FRAMEDEFS_H
#error "droneframedefs.h must be included here!"
#endif

#if defined(COLIBRI_VERSION_3)

#if defined(COLIBRI_ALIGNMENT_MIPIFRONT)
static const Matrix3f gyro_align(
		0,-1, 0,
		1, 0, 0,
		0, 0, 1);

static const Matrix3f acc_align(
		0, -1, 0,
		1, 0, 0,
		0, 0, 1);

static const Matrix3f acc2_align(
		0, 1, 0,
		-1, 0, 0,
		0, 0, 1);
#elif defined(COLIBRI_ALIGNMENT_MIPILEFT)
static const Matrix3f gyro_align(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1);

static const Matrix3f acc_align(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1);

static const Matrix3f acc2_align(
		-1, 0, 0,
		0,-1, 0,
		0, 0, 1);
#endif //alignment

#elif defined(COLIBRI_VERSION_2F)

#if defined(COLIBRI_ALIGNMENT_MIPIFRONT)

static const Matrix3f gyro_align(
		0,-1, 0,
		1, 0, 0,
		0, 0, 1);

static const Matrix3f acc_align(
		0, 1, 0,
		-1, 0, 0,
		0, 0, 1);

#elif defined(COLIBRI_ALIGNMENT_MIPILEFT)

static const Matrix3f gyro_align(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1);

static const Matrix3f acc_align(
		-1, 0, 0,
		0, -1, 0,
		0,  0, 1);

#endif //alignment

#else

#error "Colibri board version unrecognized"

#endif // version



#endif /* AXESALIGNMENT_H_ */
