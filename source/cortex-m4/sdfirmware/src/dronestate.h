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

#ifndef DRONESTATE_H_
#define DRONESTATE_H_

#include "units.h"
#include "d3math.h"
#include "alarm.h"
#include "flightdefs.h"
#include "librexjsonrpc/jsonserialization.h"

static const Altitude DEFAULT_FLIGHT_CEILING(Altitude::from_meters(50));

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
		, kp_(0.150 * 3.0)		/* Removed rpm_coef from the calculation and added it here, i.e. 3.0 */
		, ki_(0.070 * 3.0)		/* Removed rpm_coef from the calculation and added it here, i.e. 3.0 */
		, kd_(0.020 * 3.0)		/* Removed rpm_coef from the calculation and added it here, i.e. 3.0 */
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
        , yaw_throttle_factor_(0.75)
	    , pid_filter_freq_(80)
	    , flight_mode_(FLIGHT_MODE_AUTO_LEVEL)
	    , motors_armed_(false)
		, enforce_flight_ceiling_(false)
		, track_magnetometer_(false)
	    , iteration_(0)
	    , flight_ceiling_(DEFAULT_FLIGHT_CEILING) { }

	rexjson::value to_json()
	{
		rexjson::object ret;
		ret["accel_raw"] = matrix_to_json_value(accel_raw_);
		ret["mag_raw"] = matrix_to_json_value(mag_raw_);
		ret["gyro"] = matrix_to_json_value(gyro_);
		ret["accel"] = matrix_to_json_value(accel_);
		ret["mag"] = matrix_to_json_value(mag_);
		ret["altitude_meters"] = altitude_.meters();
		ret["pressure_hpa"] = pressure_hpa_;
		ret["temperature"] = temperature_;
		ret["battery_voltage"] = battery_voltage_.volts();
		ret["gps_latitude"] = latitude_;
		ret["gps_longitude"] = longitude_;
		ret["gps_speed_kmph"] = speed_over_ground_;
		ret["gps_course_deg"] = course_;
		ret["gps_satellites"] = static_cast<int>(satellite_count_);
		ret["gps_altitude"] = gps_altitude_.meters();
		ret["dt"] = static_cast<float>(dt_.microseconds());
		ret["iteration"] = static_cast<int>(iteration_);
		ret["attitude"] = quaternion_to_json_value(attitude_);
		ret["target"] = quaternion_to_json_value(target_);
		ret["motors"] = matrix_to_json_value(motors_);
		ret["yaw"] = yaw_;
		ret["pitch"] = pitch_;
		ret["roll"] = roll_;
		ret["motors_armed"] = motors_armed_;
		if (!alarm_.is_none()) {
			ret["alarm"] = alarm_.to_string();
			if (alarm_.data().size() > 0) {
				ret["alarm_data"] = alarm_.data();
			}
			ret["alarm_time_ms"] = static_cast<int>(alarm_.when().milliseconds());
		}
		return ret;
	}
	rexjson::value to_json_ex()
	{
		rexjson::object ret = to_json().get_obj();
		ret["accel_adjustment"] = matrix_to_json_value(accelerometer_adjustment_);
		ret["battery_percentage"] = battery_percentage_;
		ret["battery_type"] = battery_type_;
		ret["kp"] = kp_;
		ret["ki"] = ki_;
		ret["kd"] = kd_;
		ret["yaw_kp"] = yaw_kp_;
		ret["yaw_ki"] = yaw_ki_;
		ret["yaw_kd"] = yaw_kd_;
		ret["accelerometer_correction_period"] = accelerometer_correction_period_;
		ret["gyro_factor"] = gyro_factor_;
		ret["yaw_throttle_factor"] = yaw_throttle_factor_;
		ret["yaw_bias"] = yaw_bias_;
		ret["pitch_bias"] = pitch_bias_;
		ret["roll_bias"] = roll_bias_;
		ret["pid_filter_freq"] = pid_filter_freq_;
		if (!most_critical_alarm_.is_none()) {
			ret["crit_alarm"] = most_critical_alarm_.to_string();
			ret["crit_alarm_time_ms"] = static_cast<int>(most_critical_alarm_.when().milliseconds());
		}
		ret["flight_mode"] = flight_mode_;
		return ret;
	}

	rexjson::value boot_config_to_json()
	{
		rexjson::object ret;
		ret["accel_adjustment"] = matrix_to_json_value(accelerometer_adjustment_);
		ret["kp"] = kp_;
		ret["ki"] = ki_;
		ret["kd"] = kd_;
		ret["yaw_kp"] = yaw_kp_;
		ret["yaw_ki"] = yaw_ki_;
		ret["yaw_kd"] = yaw_kd_;
		ret["accelerometer_correction_period"] = accelerometer_correction_period_;
		ret["gyro_factor"] = gyro_factor_;
		ret["yaw_throttle_factor"] = yaw_throttle_factor_;
		ret["yaw_bias"] = yaw_bias_;
		ret["pitch_bias"] = pitch_bias_;
		ret["roll_bias"] = roll_bias_;
		ret["pid_filter_freq"] = pid_filter_freq_;
		return ret;
	}

	void init_from_boot_config(rexjson::value bootconfig)
	{
		try {
			try {
				accelerometer_adjustment_ = matrix_from_json_value<float, 3, 1>(bootconfig["accel_adjustment"]);
			} catch (std::exception& e) {}
			try { kp_ = bootconfig["kp"].get_real(); } catch (std::exception& e) {}
			try { ki_ = bootconfig["ki"].get_real(); } catch (std::exception& e) {}
			try { kd_ = bootconfig["kd"].get_real(); } catch (std::exception& e) {}
			try { yaw_kp_ = bootconfig["yaw_kp"].get_real(); } catch (std::exception& e) {}
			try { yaw_bias_ = bootconfig["yaw_bias"].get_real(); } catch (std::exception& e) {}
			try { pitch_bias_ = bootconfig["pitch_bias"].get_real(); } catch (std::exception& e) {}
			try { roll_bias_ = bootconfig["roll_bias"].get_real(); } catch (std::exception& e) {}
			try { yaw_ki_ = bootconfig["yaw_ki"].get_real(); } catch (std::exception& e) {}
			try { yaw_kd_ = bootconfig["yaw_kd"].get_real(); } catch (std::exception& e) {}
			try { gyro_factor_ = bootconfig["gyro_factor"].get_real(); } catch (std::exception& e) {}
			try { yaw_throttle_factor_ = bootconfig["yaw_throttle_factor"].get_real(); } catch (std::exception& e) {}
			try {
				accelerometer_correction_period_ = bootconfig["accelerometer_correction_period"].get_real();
			} catch (std::exception& e) {}
			try { pid_filter_freq_ = bootconfig["pid_filter_freq"].get_real(); } catch (std::exception& e) {}
		} catch (std::exception& e) {
		}
	}

	/*
	 * Sensors - input attributes
	 */
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
	float altitude_kp_;
	float altitude_ki_;
	float altitude_kd_;
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
	float pid_filter_freq_;
	FlightMode flight_mode_;
	bool motors_armed_;
	bool enforce_flight_ceiling_;
	bool track_magnetometer_;

	/*
	 * Time it took to read sensors
	 */
	TimeSpan dt_;

	/*
	 * Iteration number of the main control loop
	 */
	uint32_t iteration_;

	/*
	 * Calculated state attributes
	 */
	QuaternionF attitude_;
	QuaternionF target_;
	Vector4f motors_;

	/*
	 * Safety...
	 */
	Alarm alarm_;
	Alarm most_critical_alarm_;

	Altitude flight_ceiling_;
	Altitude take_off_altitude_;
};


#endif /* DRONESTATE_H_ */
