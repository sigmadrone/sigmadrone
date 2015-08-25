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
struct DroneState {
	DroneState()
		: altitude_(INVALID_ALTITUDE)
		, pressure_hpa_(0.0f)
		, temperature_(0.0f)
		, kp_(0.2f)
		, ki_(0.0f)
		, kd_(0.04f)
		, kp_yaw_(0.01333f)
		, ki_yaw_(0.0f)
		, kd_yaw_(0.0f)
		, accelerometer_correction_period_(4.0)
		, yaw_(0.0)
		, pitch_(0.0)
		, roll_(0.0)
		, base_throttle_(0.0) { }
	rexjson::value to_json() {
		rexjson::object ret;
		ret["gyro_raw"] = matrix_to_json_value(gyro_raw_);
		ret["accel_raw"] = matrix_to_json_value(accel_raw_);
		ret["mag_raw"] = matrix_to_json_value(mag_raw_);
		ret["gyro"] = matrix_to_json_value(gyro_);
		ret["accel"] = matrix_to_json_value(accel_);
		ret["mag"] = matrix_to_json_value(mag_);
		ret["altitude_meters"] = altitude_.meters();
		ret["pressure_hpa"] = pressure_hpa_;
		ret["temperature"] = temperature_;
		ret["dt"] = static_cast<float>(dt_.microseconds());
		ret["attitude"] = quaternion_to_json_value(attitude_);
		ret["target"] = quaternion_to_json_value(target_);
		ret["motors"] = matrix_to_json_value(motors_);
		ret["pid_torque"] = matrix_to_json_value(pid_torque_);
		ret["last_twist"] = quaternion_to_json_value(last_twist_);
		ret["kp"] = kp_;
		ret["ki"] = ki_;
		ret["kd"] = kd_;
		ret["kp_yaw"] = kp_yaw_;
		ret["ki_yaw"] = ki_yaw_;
		ret["kd_yaw"] = kd_yaw_;
		ret["accelerometer_correction_period"] = accelerometer_correction_period_;
		ret["yaw"] = yaw_;
		ret["pitch"] = pitch_;
		ret["roll"] = roll_;
		ret["base_throttle"] = base_throttle_;
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
	Altitude altitude_;
	float pressure_hpa_;
	float temperature_;
	/*more to add here*/

	/*
	 * Control
	 */
	float kp_;
	float ki_;
	float kd_;
	float kp_yaw_;
	float ki_yaw_;
	float kd_yaw_;
	float accelerometer_correction_period_;
	float yaw_;
	float pitch_;
	float roll_;
	float base_throttle_;

	/*
	 * Time it took to read sensors
	 */
	TimeSpan dt_;

	/*
	 * Calculated state attributes
	 */
	QuaternionF attitude_;
	QuaternionF target_;
	Vector4f motors_;
	Vector3f pid_torque_;
	QuaternionF last_twist_;

	/*
	 * Safety...
	 */
	Alarm alarm_;
	Alarm most_critical_alarm_;
};


#endif /* DRONESTATE_H_ */
