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
#include "pidcontroller.h"
#include "ipilot.h"
#include "propeller.h"
#include "airflowforce.h"
#include "firfilt.h"
#include "sensorsprefilters.h"

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

	virtual const Vector3d& torque_correction() const { return torque_correction_; }
	virtual const std::vector<float>& motors() const;
	virtual float get_min_thrust() const { return min_thrust_; }
	virtual float get_max_thrust() const { return max_thrust_; }
	virtual PilotType get_pilot_type() const { return PILOT_TYPE_PID_NEW; }
	virtual Vector3d get_torque_xy_p() const { return torq_xy_p_; }
	virtual Vector3d get_torque_xy_d() const { return torq_xy_d_; }
	virtual Vector3d get_torque_xy_i() const { return torq_xy_i_; }
	virtual Vector3d get_torque_z_p() const { return torq_z_p_; }
	virtual Vector3d get_torque_z_d() const { return torq_z_d_; }

private:
	std::vector<float> clip_motors(const std::vector<float>& motors);
	Vector3d get_torque(const DroneState& state);

private:
	AirflowForce<float> pf_;
	std::vector<float> motors_;
	float min_thrust_;
	float max_thrust_;
	float target_thrust_;
	float max_integral_torque_;

	Vector3d target_yawv_;
	PidController3d pid_;
	PidController3d pidz_;
	Vector3d torque_correction_;
	Vector3d swing_;
	std::vector<Propeller> propellers_;
	LowPassFilter<float, float> pitch_avg_;
	LowPassFilter<float, float> roll_avg_;

public:
	Vector3d torq_xy_p_;
	Vector3d torq_xy_d_;
	Vector3d torq_xy_i_;
	Vector3d torq_z_p_;
	Vector3d torq_z_d_;
};


#endif /* PIDPILOT_H_ */
