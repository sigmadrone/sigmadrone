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

#ifndef TRIPILOT_H_
#define TRIPILOT_H_

#include "units.h"
#include "dronestate.h"
#include "pidtorque.h"
#include "pidcontroller.h"
#include "ipilot.h"
#include "propeller.h"
#include "airflowforce.h"
#include "firfilt.h"

class TriPilot : public IPilot
{
public:
	static constexpr float Ro = 1.225; // Air density
	TriPilot();
	virtual ~TriPilot();
	virtual void set_pid_coefficents(const DroneState&);
	virtual void update_state(DroneState& state);

	virtual void set_min_thrust(float minRev);
	virtual void set_max_thrust(float maxRev);
	virtual void set_target_thrust(float thrust);

	virtual const Vector3f& torque_correction() const { return torque_correction_; }
	virtual const Vector4f& motors() const;
	virtual float get_min_thrust() const { return min_thrust_; }
	virtual float get_max_thrust() const { return max_thrust_; }
	virtual PilotType get_pilot_type() const { return PILOT_TYPE_PID_NEW; }

private:
	Vector4f clip_motors(const Vector4f& motors);
	Vector3f get_torque(const DroneState& state);

private:
	AirflowForce<float> pf_;
	Vector4f motors_;
	float min_thrust_;
	float max_thrust_;
	float target_thrust_;
	float max_integral_torque_;

	Vector3f target_yawv_;
	QuaternionF target_swing_;
	QuaternionF target_twist_;
	PidController3f pid_;
	PidController3f pidz_;
	Vector3f torque_correction_;
	Vector3f swing_;
	std::vector<Propeller> propellers_;
	MovingAverageFilter<float, float, 30> pitch_avg_;
	MovingAverageFilter<float, float, 30> roll_avg_;
};


#endif /* PIDPILOT_H_ */
