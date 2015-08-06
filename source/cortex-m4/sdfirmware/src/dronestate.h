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
#include "librexjsonrpc/jsonserialization.h"

struct DroneState {
	DroneState() : altitude_meters_(0.0f), pressure_hpa_(0.0f), temperature_(0.0f) {}
	rexjson::value to_json() {
		rexjson::object ret;
		ret["gyro_raw"] = matrix_to_json_value(gyro_raw_);
		ret["accel_raw"] = matrix_to_json_value(accel_raw_);
		ret["mag_raw"] = matrix_to_json_value(mag_raw_);
		ret["gyro"] = matrix_to_json_value(gyro_);
		ret["accel"] = matrix_to_json_value(accel_);
		ret["mag"] = matrix_to_json_value(mag_);
		ret["altitude_meters"] = altitude_meters_;
		ret["pressure_hpa"] = pressure_hpa_;
		ret["temperature"] = temperature_;
		ret["dt"] = static_cast<float>(dt_.microseconds());
		ret["attitude"] = quaternion_to_json_value(attitude_);
		ret["motors"] = matrix_to_json_value(motors_);
		ret["pid_torque"] = matrix_to_json_value(pid_torque_);
		return ret;
	}

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
