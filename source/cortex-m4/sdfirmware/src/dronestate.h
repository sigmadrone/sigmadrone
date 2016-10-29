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

#define SMALL_FRAME

#include "units.h"
#include "battery.h"
#include "d3math.h"
#include "alarm.h"
#include "flightdefs.h"
#include "librexjsonrpc/jsonserialization.h"

static const Altitude DEFAULT_FLIGHT_CEILING(Altitude::from_meters(50));

struct DroneState {
	DroneState()
		: altitude_(INVALID_ALTITUDE)
		, pressure_hpa_(0.0f)
		, temperature_(0.0f)
	    , battery_voltage_()
        , battery_percentage_(0.0f)
	    , battery_type_(Battery::TYPE_UNKNOWN)
	    , latitude_(0.0f)
	    , longitude_(0.0f)
		, speed_over_ground_(0.0f)
		, course_(-360.0f)
	    , satellite_count_(0.0f)
	    , gps_altitude_(Altitude::from_meters(-100))
		, vertical_speed_()
		, kp_(0.14)
		, ki_(0.3)
		, kd_(0.035)
		, yaw_kp_(0.72)
		, yaw_ki_(0.0)
		, yaw_kd_(0.30)
	    , altitude_kp_(0.35)
		, altitude_ki_(0.5)
		, altitude_kd_(0.035)
	    , gyro_drift_kp_(0.0)
	    , gyro_drift_ki_(0.01)
	    , gyro_drift_kd_(0.0)
	    , gyro_drift_leak_rate_(0.00001)
	    , accelerometer_correction_speed_(1.0)
	    , pilot_type_(PILOT_TYPE_PID_LEGACY)
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
		, track_magnetometer_(true)
		, track_accelerometer_(true)
	    , external_gyro_enabled_(false)
	    , external_gyro_align_(0, 1, 0,
	    		              -1, 0, 0,
				               0, 0, 1)
	    , altitude_tracker_kp_(0.025)
		, altitude_tracker_ki_(0.005)
		, altitude_tracker_kd_(0.0)
		, altitude_tracker_kp2_(0.04)
	    , altitude_correction_period_(TimeSpan::from_seconds(10))
	    , iteration_(0)
	    , flight_ceiling_(DEFAULT_FLIGHT_CEILING)
	    , altitude_lpf_(0.965)
	{
		set_pilot_type(PILOT_TYPE_PID_NEW);
	}

	rexjson::value to_json()
	{
		rexjson::object ret;
		ret["gyro"] = matrix_to_json_value(gyro_);
		ret["gyro_raw"] = matrix_to_json_value(gyro_raw_);
		ret["gyro_drift"] = matrix_to_json_value(gyro_drift_error_);
		ret["accel"] = matrix_to_json_value(accel_);
		ret["mag"] = matrix_to_json_value(mag_);
		ret["altitude_meters"] = altitude_.meters();
		ret["pressure_hpa"] = pressure_hpa_;
		ret["temperature"] = temperature_;
		ret["battery_voltage"] = battery_voltage_.volts();
		ret["battery_percentage"] = battery_percentage_;
		ret["gps_latitude"] = latitude_;
		ret["gps_longitude"] = longitude_;
		ret["gps_speed_kmph"] = speed_over_ground_;
		ret["gps_course_deg"] = course_;
		ret["gps_satellites"] = static_cast<int>(satellite_count_);
		ret["gps_altitude"] = gps_altitude_.meters();
		ret["vertical_speed"] = vertical_speed_.meters_per_second();
		ret["dt"] = static_cast<float>(dt_.microseconds());
		ret["iteration"] = static_cast<int>(iteration_);
		ret["attitude"] = quaternion_to_json_value(attitude_);
		ret["altitude_from_baro"] = altitude_from_baro_.meters();
		ret["target"] = quaternion_to_json_value(target_);
		ret["motors"] = matrix_to_json_value(motors_);
		ret["yaw"] = yaw_;
		ret["pitch"] = pitch_;
		ret["roll"] = roll_;
		ret["base_throttle"] = base_throttle_;
		ret["motors_armed"] = motors_armed_;
		ret["pid_torque"] = matrix_to_json_value(pid_torque_);
		if (!alarm_.is_none()) {
			ret["alarm"] = alarm_.to_string();
			if (alarm_.data().size() > 0) {
				ret["alarm_data"] = alarm_.data();
			}
			ret["alarm_time_ms"] = static_cast<int>(alarm_.when().milliseconds());
		}
		if (!most_critical_alarm_.is_none()) {
			ret["crit_alarm"] = most_critical_alarm_.to_string();
			ret["crit_alarm_time_ms"] = static_cast<int>(most_critical_alarm_.when().milliseconds());
		}
		return ret;
	}
	rexjson::value to_json_ex()
	{
		rexjson::object ret = to_json().get_obj();
		ret["accel_adjustment"] = matrix_to_json_value(accelerometer_adjustment_);
		ret["battery_type"] = Battery::type_as_string(battery_type_);
		ret["kp"] = kp_;
		ret["ki"] = ki_;
		ret["kd"] = kd_;
		ret["yaw_kp"] = yaw_kp_;
		ret["yaw_ki"] = yaw_ki_;
		ret["yaw_kd"] = yaw_kd_;
		ret["gyro_drift_kp"] = gyro_drift_kp_;
		ret["gyro_drift_ki"] = gyro_drift_ki_;
		ret["gyro_drift_kd"] = gyro_drift_kd_;
		ret["altitude_kp"] = altitude_kp_;
		ret["altitude_ki"] = altitude_ki_;
		ret["altitude_kd"] = altitude_kd_;
		ret["accelerometer_correction_period"] = accelerometer_correction_speed_;
		ret["altitude_correction_period"] = altitude_correction_period_.seconds_float();
		ret["gyro_factor"] = gyro_factor_;
		ret["yaw_throttle_factor"] = yaw_throttle_factor_;
		ret["yaw_bias"] = yaw_bias_;
		ret["pitch_bias"] = pitch_bias_;
		ret["roll_bias"] = roll_bias_;
		ret["pid_filter_freq"] = pid_filter_freq_;
		ret["track_magnetometer"] = track_magnetometer_;
		ret["flight_mode"] = flight_mode_;
		ret["pilot_type"] = std::string(PilotTypeAsStr(pilot_type_));
		ret["ext_gyro_enabled"] = external_gyro_enabled_;
		ret["ext_gyro_align"] = external_gyro_align_;
		ret["flight_posture"] = flight_posture_;
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
		ret["altitude_kp"] = altitude_kp_;
		ret["altitude_ki"] = altitude_ki_;
		ret["altitude_kd"] = altitude_kd_;
		ret["accelerometer_correction_period"] = accelerometer_correction_speed_;
		ret["altitude_correction_period"] = altitude_correction_period_.seconds_float();
		ret["gyro_factor"] = gyro_factor_;
		ret["yaw_throttle_factor"] = yaw_throttle_factor_;
		ret["yaw_bias"] = yaw_bias_;
		ret["pitch_bias"] = pitch_bias_;
		ret["roll_bias"] = roll_bias_;
		ret["pid_filter_freq"] = pid_filter_freq_;
		ret["use_ext_gyro"] = external_gyro_enabled_;
		ret["ext_gyro_align"] = matrix_to_json_value(external_gyro_align_);
		ret["flight_mode"] = flight_mode_;
		return ret;
	}

	void init_from_boot_config(rexjson::value bootconfig)
	{
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
			accelerometer_correction_speed_ = bootconfig["accelerometer_correction_period"].get_real();
		} catch (std::exception& e) {}
		try { pid_filter_freq_ = bootconfig["pid_filter_freq"].get_real(); } catch (std::exception& e) {}
		try { external_gyro_enabled_ = bootconfig["use_ext_gyro"].get_real(); } catch (std::exception& e) {}
		try {
			external_gyro_align_ = matrix_from_json_value<float, 3, 3>(bootconfig["ext_gyro_align"]);
		} catch (std::exception& e) {}
		try { altitude_kp_ = bootconfig["altitude_kp"].get_real(); } catch (std::exception& e) {}
		try { altitude_ki_ = bootconfig["altitude_ki"].get_real(); } catch (std::exception& e) {}
		try { altitude_kd_ = bootconfig["altitude_kd"].get_real(); } catch (std::exception& e) {}
		try {
			altitude_correction_period_ = TimeSpan::from_seconds_float(bootconfig["altitude_correction_period"].get_real());
		} catch (std::exception& e) {}

		try { flight_mode_ = (FlightMode)bootconfig["flight_mode"].get_int(); } catch (std::exception& e) {}
		if (flight_mode_ > FLIGHT_MODE_LAST) {
			flight_mode_ = FLIGHT_MODE_AUTO_LEVEL;
		}
	}

	void set_pilot_type(PilotType pilot_type)
	{
		pilot_type_ = pilot_type;
		if (PILOT_TYPE_PID_NEW == pilot_type) {
#ifdef SMALL_FRAME
			kp_ = 0.2;
			kd_= 0.035;
			ki_ = 0.09;
#else
			kp_ = 0.4;
			kd_= 0.035;
			ki_ = 0.09;
#endif
			yaw_kp_ = 0.20;
			yaw_ki_= 0.0;
			yaw_kd_ = 0.07;
		} else if (PILOT_TYPE_PID_LEGACY == pilot_type) {
			kp_ = 0.14;
			ki_ = 0.3;
			kd_ = 0.035;
			yaw_kp_ = 1.00;
			yaw_ki_ = 0.0;
			yaw_kd_ = 0.10;
		}
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
	Vector3f gyro_drift_error_;
	Altitude altitude_;
	Altitude altitude_from_baro_;
	float pressure_hpa_;
	float temperature_;
	Voltage battery_voltage_;
	float battery_percentage_;
	Battery::Type battery_type_;
	float latitude_;
	float longitude_;
	float speed_over_ground_; //km/h
	float course_; // degrees
	uint32_t satellite_count_;
	Altitude gps_altitude_;
	Speed vertical_speed_;
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
	float gyro_drift_kp_;
	float gyro_drift_ki_;
	float gyro_drift_kd_;
	float gyro_drift_leak_rate_;
	Vector3f accelerometer_adjustment_;
	float accelerometer_correction_speed_;
	PilotType pilot_type_;
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
	bool track_accelerometer_;
	bool external_gyro_enabled_;
	Matrix3f external_gyro_align_;
	float altitude_tracker_kp_;
	float altitude_tracker_ki_;
	float altitude_tracker_kd_;
	float altitude_tracker_kp2_;
	TimeSpan altitude_correction_period_;

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
	Vector3f pid_torque_;
	float throttle_;
	std::string flight_posture_;

	/*
	 * Safety...
	 */
	Alarm alarm_;
	Alarm most_critical_alarm_;

	Altitude flight_ceiling_;
	Altitude take_off_altitude_;

	float altitude_lpf_; // temp
};


#endif /* DRONESTATE_H_ */
