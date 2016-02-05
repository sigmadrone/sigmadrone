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

#include "units.h"
#include "d3math.h"
#include "pidcontroller.h"

#ifndef _PIDTORQUE_H_
#define _PIDTORQUE_H_

class PidTorque {
public:
	PidTorque();
	~PidTorque();

	void set_target(const QuaternionF &set_q);
	Vector3f get_torque(const QuaternionF &in_q, const TimeSpan& dt, float yaw_factor);

public:
	QuaternionF set_Q_;
	PidController<float> pid_roll_;
	PidController<float> pid_pitch_;
	PidController<float> pid_yaw_;
};

#endif

