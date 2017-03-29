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

#ifndef TARGETATTITUDE_H_
#define TARGETATTITUDE_H_

#include "dronestate.h"
#include "lowpassfilter.h"

class TargetAttitude
{
public:
	TargetAttitude();
	void update_state(DroneState& state);
	QuaternionF get() const;
	const QuaternionF& target_twist() const;
	const QuaternionF& target_swing() const;

private:
	QuaternionF twist_;
	QuaternionF swing_;
	LowPassFilter<float, float> pitch_avg_;
	LowPassFilter<float, float> roll_avg_;
};




#endif /* TARGETATTITUDE_H_ */
