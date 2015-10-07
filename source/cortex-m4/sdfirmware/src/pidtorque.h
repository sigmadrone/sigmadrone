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
#include "matrix.h"
#include "pidcontroller.h"

#ifndef _PIDTORQUE_H_
#define _PIDTORQUE_H_

#define PILOT_Z_COMPENSATE

class PidTorque
{
public:
	PidTorque();
	~PidTorque();

	void set_target(const QuaternionF &set_q);
	Vector3f get_torque(const QuaternionF &in_q, const TimeSpan& dt, float yaw_factor);

public:
	QuaternionF twist_;
	QuaternionF set_Q_;
	PidController3f pid_controller_;
	PidController3f pid_controller_yaw_;
};

#endif

