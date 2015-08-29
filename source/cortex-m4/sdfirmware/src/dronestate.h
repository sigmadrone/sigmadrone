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
		, kp_(0.2)
		, ki_(0.0)
		, kd_(0.03)
		, yaw_kp_(0.0)
		, yaw_ki_(0.0)
		, yaw_kd_(0.03)
		, accelerometer_correction_period_(4.5)
		, gyro_factor_(1.25)
		, yaw_throttle_factor_(0.75)
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
		ret["twist"] = quaternion_to_json_value(twist_);
		ret["kp"] = kp_;
		ret["ki"] = ki_;
		ret["kd"] = kd_;
		ret["yaw_kp"] = yaw_kp_;
		ret["yaw_ki"] = yaw_ki_;
		ret["yaw_kd"] = yaw_kd_;
		ret["accelerometer_correction_period"] = accelerometer_correction_period_;
		ret["gyro_factor"] = gyro_factor_;
		ret["yaw_throttle_factor"] = yaw_throttle_factor_;
		ret["yaw"] = yaw_;
		ret["pitch"] = pitch_;
		ret["roll"] = roll_;
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
	float yaw_kp_;
	float yaw_ki_;
	float yaw_kd_;
	float accelerometer_correction_period_;
	float gyro_factor_;
	float yaw_;
	float pitch_;
	float roll_;
	float base_throttle_;
	float yaw_throttle_factor_;

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
	QuaternionF twist_;

	/*
	 * Safety...
	 */
	Alarm alarm_;
	Alarm most_critical_alarm_;
};


#endif /* DRONESTATE_H_ */
