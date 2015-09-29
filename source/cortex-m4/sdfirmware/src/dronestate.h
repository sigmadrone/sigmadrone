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
 *
 * PID coefficients for big 9" props on DJI F450
 * Kp = 0.125
 * Ki = 0
 * Kd = 0.2 -- 0.3
 *
 * Kp_Yaw = 0.11
 * Ki_Yaw = 0
 * Kd_Yaw = 0.45
 *
 */
struct DroneState {
	DroneState()
		: altitude_(INVALID_ALTITUDE)
		, pressure_hpa_(0.0f)
		, temperature_(0.0f)
	    , battery_voltage_()
        , battery_percentage_(0.0f)
	    , latitude_(0.0f)
	    , longitude_(0.0f)
		, speed_over_ground_(0.0f)
		, course_(-360.0f)
	    , satellite_count_(0.0f)
	    , gps_altitude_(Altitude::from_meters(-100))
		, kp_(0.125)
		, ki_(0.0)
		, kd_(0.03)
		, yaw_kp_(0.24)
		, yaw_ki_(0.0)
		, yaw_kd_(0.10)
		, accelerometer_correction_period_(5.0)
		, gyro_factor_(1.25)
		, yaw_(0.0)
		, pitch_(0.0)
		, roll_(0.0)
		, yaw_bias_(0.0)
		, pitch_bias_(0.0)
		, roll_bias_(0.0)
		, base_throttle_(0.0)
        , yaw_throttle_factor_(0.75) { }
	rexjson::value to_json() {
		rexjson::object ret;
		ret["gyro_raw"] = matrix_to_json_value(gyro_raw_);
		ret["accel_raw"] = matrix_to_json_value(accel_raw_);
		ret["mag_raw"] = matrix_to_json_value(mag_raw_);
		ret["gyro"] = matrix_to_json_value(gyro_);
		ret["accel"] = matrix_to_json_value(accel_);
		ret["accel_adjustment"] = matrix_to_json_value(accelerometer_adjustment_);
		ret["mag"] = matrix_to_json_value(mag_);
		ret["altitude_meters"] = altitude_.meters();
		ret["pressure_hpa"] = pressure_hpa_;
		ret["temperature"] = temperature_;
		ret["battery_voltage"] = battery_voltage_.volts();
		ret["battery_percentage"] = battery_percentage_;
		ret["battery_type"] = battery_type_;
		ret["gps_latitude"] = latitude_;
		ret["gps_longitude"] = longitude_;
		ret["gps_speed_kmph"] = speed_over_ground_;
		ret["gps_course_deg"] = course_;
		ret["gps_satellites"] = static_cast<int>(satellite_count_);
		ret["gps_altitude"] = gps_altitude_.meters();
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
		ret["yaw_bias"] = yaw_bias_;
		ret["pitch_bias"] = pitch_bias_;
		ret["roll_bias"] = roll_bias_;
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
	Voltage battery_voltage_;
	float battery_percentage_;
	std::string battery_type_;
	float latitude_;
	float longitude_;
	float speed_over_ground_; //km/h
	float course_; // degrees
	uint32_t satellite_count_;
	Altitude gps_altitude_;
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
	Vector3f accelerometer_adjustment_;
	float accelerometer_correction_period_;
	float gyro_factor_;
	float yaw_;
	float pitch_;
	float roll_;
	float yaw_bias_;
	float pitch_bias_;
	float roll_bias_;
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
