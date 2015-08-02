/*
 * dronestate.h
 *
 *  Created on: Jul 18, 2015
 *      Author: svassilev
 */

#ifndef DRONESTATE_H_
#define DRONESTATE_H_

#include "units.h"
#include "matrix.h"
#include "alarm.h"

struct DroneState {
	DroneState() : altitude_meters_(0.0f), pressure_hpa_(0.0f), temperature_(0.0f) {}

	/*
	 * Sensors - input attributes
	 */
	Vector3f gyro_raw_;
	Vector3f accel_raw_;
	Vector3f mag_raw_;
	Vector3f gyro_;
	Vector3f accel_;
	Vector3f mag_;
	float altitude_meters_;
	float pressure_hpa_;
	float temperature_;
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
	Vector3f pid_torque_;

	/*
	 * Safety...
	 */
	Alarm alarm_;
	Alarm most_critical_alarm_;
};


#endif /* DRONESTATE_H_ */
