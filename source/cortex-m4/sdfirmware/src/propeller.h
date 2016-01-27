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

#ifndef _PROPELLER_H_
#define _PROPELLER_H_

#include "units.h"
#include "d3math.h"

class Propeller {
public:
	Vector3f position_;		// Position of the propeller in the local coordinate system.
	Vector3f thrust_dir_;	// Direction of the thrust
	Vector3f torque_dir_;	// Direction of the torque vector. It depends on the position of the propeller and the direction of rotation.

	enum RotationDir {
		CCW = 1,
		CW = -1
	};

public:
	/*
	 * Assuming the thrust, created by the propeller is in Z direction:
	 * The propeller will create two types of torque:
	 * 1. XY-plane torque which is the (radius * thrust)
	 * 2. Z torque, which is opposite to the direction of rotation
	 */
	Propeller(Vector3f position, Vector3f thrust_dir, RotationDir rot)
		: position_(position)
		, thrust_dir_(thrust_dir)
	{
		/*
		 * Calculates the direction of the torque_vector
		 */
		torque_dir_ = (position.cross(thrust_dir) + thrust_dir * -rot).normalize();
	}

	const Vector3f& thrust_dir() const { return thrust_dir_; }
	const Vector3f& torque_dir() const { return torque_dir_; }
};

#endif // _PROPELLER_H_
