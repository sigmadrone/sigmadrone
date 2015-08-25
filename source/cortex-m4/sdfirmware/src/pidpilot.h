/*
 * pidpilot.h
 *
 *  Created on: May 18, 2013
 *      Author: svassilev
 */

#ifndef PIDPILOT_H_
#define PIDPILOT_H_

#include "units.h"
#include "dronestate.h"
#include "pidtorque.h"
#include "pidcontroller.h"


class PidPilot
{
public:
	PidPilot();
	~PidPilot();
	void set_pid_coefficents(const DroneState&);
	void update_state(DroneState& state);

	void set_min_thrust(float minRev);
	void set_max_thrust(float maxRev);
	void set_target_thrust(float thrust);

	const Vector4f& motors() const;
	float get_min_thrust() const { return min_thrust_; }
	float get_max_thrust() const { return max_thrust_; }

private:
	void set_and_scale_motors(
			float w0,
			float w1,
			float w2,
			float w3
			);

private:
	Vector4f motors_;
	float min_thrust_;
	float max_thrust_;
	float target_thrust_;

	PidTorque pid_;
	Vector3f torque_correction_;
	Vector3f m0_, m1_, m2_, m3_;
};


#endif /* PIDPILOT_H_ */
