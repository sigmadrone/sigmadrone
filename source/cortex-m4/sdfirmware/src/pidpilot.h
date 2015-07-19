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

/*
 * PID coefficients for big props on DJI F450
 * Kp = 1.0 <-> 1.3
 * Ki = 1.2, with leak rate dT
 * Kd = 0.3 <-> 0.45
 *
 * PID coefficients for small props on DJI F450
 * Kp = 1.0 <-> 1.3
 * Ki = 0 - still not a measurable impact of Ki
 * Kd = 0.35
 */

class PidPilot
{
public:
	PidPilot(float kp = 1.0, float ki = 0.0, float kd = 0.35);
	~PidPilot();
	void reset(float kp, float ki, float kd);
	void update_state(DroneState& state, const QuaternionF& target_attitude);

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
