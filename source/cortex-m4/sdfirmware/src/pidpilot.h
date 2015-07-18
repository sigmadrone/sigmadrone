/*
 * pidpilot.h
 *
 *  Created on: May 18, 2013
 *      Author: svassilev
 */

#ifndef PIDPILOT_H_
#define PIDPILOT_H_

#include "units.h"
#include "pidtorque.h"
#include "pidcontroller.h"

struct DroneState {
	/*
	 * Sensors - input attributes
	 */
	Vector3f gyro_raw_;
	Vector3f accel_raw_;
	Vector3f mag_raw_;
	Vector3f gyro_;
	Vector3f accel_;
	Vector3f mag_;
	/*more to add here*/

	/*
	 * Time it took to read sensors
	 */
	TimeSpan dt_;

	/*
	 * Calculated state attributes
	 */
	QuaternionF attitude_;
	Vector4f motors_;
};

class PidPilot
{
public:
	PidPilot(float kp = 0.0, float ki = 0.0, float kd = 0.0);
	~PidPilot();
	void reset(float kp, float ki, float kd);
	void update_state(DroneState& state, const QuaternionF& target_attitude);

	void set_min_thrust(float minRev);
	void set_max_thrust(float maxRev);

	const Vector4f& get_motors() const;
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
