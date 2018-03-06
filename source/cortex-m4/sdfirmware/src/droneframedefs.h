/*
 *  Sigmadrone
 *  Copyright (c) 2013-2018 The Sigmadrone Developers
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

#ifndef DRONE_FRAMEDEFS_H
#define DRONE_FRAMEDEFS_H

//
// Sanity checks none of the macros have been defined outside of droneframedefs.h
//
#if defined(COLIBRI_VERSION_3) || defined(COLIBRI_VERSION_2F)
#error "Colibri version is defined outside of droneframedefs.h"
#endif

#if defined(HEXCOPTER_FRAME) || defined(QUADCOPTER_FRAME)
#error "Frame geometry is defined outside of droneframedefs.h"
#endif

#if defined(COLIBRI_ALIGNMENT_MIPIFRONT) || defined(COLIBRI_ALIGNMENT_MIPILEFT)
#error "Colibri alignment defined outside of droneframedefs.h"
#endif

#if defined(FULL_FRAME) || defined(LITE_FRAME)
#error "Frame size is defined outside of droneframedefs.h"
#endif

//
// One of the COLIBRI_VERSION_X must be defined
//
#define COLIBRI_VERSION_3
//#define COLIBRI_VERSION_2F

#if !defined(COLIBRI_VERSION_3) && !defined(COLIBRI_VERSION_2F) || defined(COLIBRI_VERSION_3) && defined(COLIBRI_VERSION_2F)
#error "One of COLIBRI_VERSION_3 or COLIBRI_VERSION_3 must be defined"
#endif


//
// Only one of the XXXCOPTER_FRAME must be defined
//

//#define HEXCOPTER_FRAME
#define QUADCOPTER_FRAME

#if !defined(HEXCOPTER_FRAME) && !defined(QUADCOPTER_FRAME) || defined(HEXCOPTER_FRAME) && defined(QUADCOPTER_FRAME)
#error "Only one of HEXCOPTER_FRAME or QUADCOPTER_FRAME must be defined"
#endif

//
// One of the alignment macros must be defined
//

//#define COLIBRI_ALIGNMENT_MIPIFRONT
#define COLIBRI_ALIGNMENT_MIPILEFT

#if !defined(COLIBRI_ALIGNMENT_MIPIFRONT) && !defined(COLIBRI_ALIGNMENT_MIPILEFT) || defined(COLIBRI_ALIGNMENT_MIPIFRONT) && defined(COLIBRI_ALIGNMENT_MIPILEFT)
#error "Only one of COLIBRI_VERSION_3 or COLIBRI_VERSION_2F must be defined"
#endif


//
// One of LITE/FULL_FRAME must be defined
//
//#define FULL_FRAME
#define LITE_FRAME

#if !defined(FULL_FRAME) && !defined(LITE_FRAME) || defined(FULL_FRAME) && defined(LITE_FRAME)
#error "Only one of FULL_FRAME or LITE_FRAME must be defined"
#endif

#endif //DRONE_FRAMEDEFS_H
