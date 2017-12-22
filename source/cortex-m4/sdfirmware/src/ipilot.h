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

#ifndef _IPILOT_H_
#define _IPILOT_H_

#include <vector>
#include "dronestate.h"

class IPilot
{
public:
	virtual ~IPilot() = default;
	virtual void set_pid_coefficents(const DroneState&) = 0;
	virtual void update_state(DroneState& state) = 0;

	virtual void set_min_thrust(float minRev) = 0;
	virtual void set_max_thrust(float maxRev) = 0;
	virtual void set_target_thrust(float thrust) = 0;

	virtual const Vector3f& torque_correction() const = 0;
	virtual const std::vector<float>& motors() const  = 0;
	virtual float get_min_thrust() const = 0;
	virtual float get_max_thrust() const = 0;

	virtual PilotType get_pilot_type() const = 0;

	virtual Vector3f get_torque_xy_p() const = 0;
	virtual Vector3f get_torque_xy_d() const = 0;
	virtual Vector3f get_torque_xy_i() const = 0;
	virtual Vector3f get_torque_z_p() const = 0;
	virtual Vector3f get_torque_z_d() const = 0;

};

#endif // _IPILOT_H_
