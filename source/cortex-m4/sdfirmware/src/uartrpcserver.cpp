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
#include <memory>
#include "trimstr.h"
#include "uartrpcserver.h"
#include "librexjsonrpc/jsonserialization.h"


UartRpcServer::UartRpcServer(DroneState& dronestate, FlashMemory& configdata)
	: rpc_server<UartRpcServer, UART*>()
	, dronestate_(dronestate)
	, configdata_(configdata)
{
	add("sd_get_altitude", &UartRpcServer::rpc_get_altitude);
	add("sd_get_attitude", &UartRpcServer::rpc_get_attitude);
	add("sd_get_pressure", &UartRpcServer::rpc_get_pressure);
	add("sd_get_temperature", &UartRpcServer::rpc_get_temperature);
	add("sd_get_motors", &UartRpcServer::rpc_get_motors);
	add("sd_get_dronestate", &UartRpcServer::rpc_get_dronestate);
	add("sd_get_dronestate_ex", &UartRpcServer::rpc_get_dronestate_ex);
	add("ki", &UartRpcServer::rpc_ki);
	add("kd", &UartRpcServer::rpc_kd);
	add("kp", &UartRpcServer::rpc_kp);
	add("yaw_ki", &UartRpcServer::rpc_yaw_ki);
	add("yaw_kd", &UartRpcServer::rpc_yaw_kd);
	add("yaw_kp", &UartRpcServer::rpc_yaw_kp);
	add("yaw_bias", &UartRpcServer::rpc_yaw_bias);
	add("pitch_bias", &UartRpcServer::rpc_pitch_bias);
	add("roll_bias", &UartRpcServer::rpc_roll_bias);
	add("sd_set_accelerometer_correction_period", &UartRpcServer::rpc_set_accelerometer_correction_period);
	add("sd_set_gyro_factor", &UartRpcServer::rpc_set_gyro_factor);
	add("sd_set_yaw_throttle_factor", &UartRpcServer::rpc_set_yaw_throttle_factor);
	add("sd_get_configdata", &UartRpcServer::rpc_get_configdata);
	add("sd_set_configdata", &UartRpcServer::rpc_set_configdata);
	add("sd_accelerometer_adjustment", &UartRpcServer::rpc_accelerometer_adjustment);
	add("sd_flight_ceiling", &UartRpcServer::rpc_flight_ceiling);
	add("sd_restore_config", &UartRpcServer::rpc_restore_config);
}

UartRpcServer::~UartRpcServer()
{

}

rexjson::value UartRpcServer::rpc_get_attitude(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_get_attitude\n"
	            "\nGet the current attitude Quaternion."
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	return quaternion_to_json_value(dronestate_.attitude_);
}

rexjson::value UartRpcServer::rpc_get_pressure(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_get_pressure\n"
	            "\nGet the current pressure in hPa."
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	return dronestate_.pressure_hpa_;
}

rexjson::value UartRpcServer::rpc_get_altitude(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_get_altitude\n"
	            "\nGet the current altitude in meters."
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	return dronestate_.altitude_.meters();
}

rexjson::value UartRpcServer::rpc_get_temperature(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_get_temperature\n"
	            "\nGet the current temperature in Celsius."
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	return dronestate_.temperature_;
}

rexjson::value UartRpcServer::rpc_get_motors(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_get_motors\n"
	            "\nGet motors state."
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	rexjson::array ret;
	for (size_t i = 0; i < 4; i++) {
		ret.push_back(dronestate_.motors_.at(i));
	}
	return ret;
}

rexjson::value UartRpcServer::rpc_accelerometer_adjustment(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_array_type|rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_accelerometer_adjustment\n"
	            "\nGet/Set accelerometer adjustment values"
				"\nIf no parameters are specified, the current accelerometer_adjustment will be returned."
				"\n"
				"Arguments:\n"
				"1. accelerometer_adjustment          (array, optional) Array of adjustment values for X,Y,Z\n"
				"   to be added to the current reading of the accelerometer sensor."
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == rexjson::array_type))
		dronestate_.accelerometer_adjustment_ = matrix_from_json_value<float, 3, 1>(params[0]);
	return matrix_to_json_value(dronestate_.accelerometer_adjustment_);
}


rexjson::value UartRpcServer::rpc_kp(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "kp\n"
	            "\nGet/Set Kp."
				"\nIf the new coefficient is not specified, the current Kp will be returned."
				"\n"
				"Arguments:\n"
				"1. Kp          (real, optional) The Kp of the PID controller.\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == rexjson::real_type))
		dronestate_.kp_ = params[0].get_real();
	return dronestate_.kp_;
}

rexjson::value UartRpcServer::rpc_kd(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "kd\n"
	            "\nGet/Set Kd."
				"\nIf the new coefficient is not specified, the current Kd will be returned."
				"\n"
				"Arguments:\n"
				"1. Kd          (real, optional) The Kd of the PID controller.\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == rexjson::real_type))
		dronestate_.kd_ = params[0].get_real();
	return dronestate_.kd_;
}

rexjson::value UartRpcServer::rpc_ki(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "ki\n"
	            "\nGet/Set Ki."
				"\nIf the new coefficient is not specified, the current Ki will be returned."
				"\n"
				"Arguments:\n"
				"1. Ki          (real, optional) The Ki of the PID controller.\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == rexjson::real_type))
		dronestate_.ki_ = params[0].get_real();
	return dronestate_.ki_;
}


rexjson::value UartRpcServer::rpc_yaw_kp(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "yaw_kp\n"
	            "\nGet/Set yaw_kp."
				"\nIf the new coefficient is not specified, the current yaw_kp will be returned."
				"\n"
				"Arguments:\n"
				"1. yaw_kp          (real, optional) The yaw_kp of the PID controller.\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == rexjson::real_type))
		dronestate_.yaw_kp_ = params[0].get_real();
	return dronestate_.yaw_kp_;
}

rexjson::value UartRpcServer::rpc_yaw_kd(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "yaw_kd\n"
	            "\nGet/Set yaw_kd."
				"\nIf the new coefficient is not specified, the current yaw_kd will be returned."
				"\n"
				"Arguments:\n"
				"1. yaw_kd          (real, optional) The yaw_kd of the PID controller.\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == rexjson::real_type))
		dronestate_.yaw_kd_ = params[0].get_real();
	return dronestate_.yaw_kd_;
}

rexjson::value UartRpcServer::rpc_yaw_ki(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "yaw_ki\n"
	            "\nGet/Set yaw_ki."
				"\nIf the new coefficient is not specified, the current yaw_ki will be returned."
				"\n"
				"Arguments:\n"
				"1. yaw_ki          (real, optional) The yaw_ki of the PID controller.\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == rexjson::real_type))
		dronestate_.yaw_ki_ = params[0].get_real();
	return dronestate_.yaw_ki_;
}

rexjson::value UartRpcServer::rpc_yaw_bias(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "yaw_bias\n"
	            "\nGet/Set yaw_bias."
				"\nIf the new coefficient is not specified, the current yaw_bias will be returned."
				"\n"
				"Arguments:\n"
				"1. yaw_bias          (real, optional) The new yaw_bias.\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == rexjson::real_type)) {
		float value = params[0].get_real();
		if (value > 0.1 || value < -0.1)
			throw std::range_error("Invalid yaw_bias value");
		dronestate_.yaw_bias_ = value;
	}
	return dronestate_.yaw_bias_;
}

rexjson::value UartRpcServer::rpc_pitch_bias(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "pitch_bias\n"
	            "\nGet/Set pitch_bias."
				"\nIf the new coefficient is not specified, the current pitch_bias will be returned."
				"\n"
				"Arguments:\n"
				"1. pitch_bias          (real, optional) The new pitch_bias.\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == rexjson::real_type)) {
		float value = params[0].get_real();
		if (value > 0.1 || value < -0.1)
			throw std::range_error("Invalid pitch_bias value");
		dronestate_.pitch_bias_ = value;
	}
	return dronestate_.pitch_bias_;
}

rexjson::value UartRpcServer::rpc_roll_bias(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "roll_bias\n"
	            "\nGet/Set roll_bias."
				"\nIf the new coefficient is not specified, the current roll_bias will be returned."
				"\n"
				"Arguments:\n"
				"1. roll_bias          (real, optional) The new roll_bias.\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == rexjson::real_type)) {
		float value = params[0].get_real();
		if (value > 0.1 || value < -0.1)
			throw std::range_error("Invalid roll_bias value");
		dronestate_.roll_bias_ = value;
	}
	return dronestate_.roll_bias_;
}

rexjson::value UartRpcServer::rpc_flight_ceiling(UART*, rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
				"sd_flight_ceiling\n"
				"\nGet/Set flight ceiling, which is relative to the lift-off altitude."
				"\nIf the new ceiling is not specified, then the current one is returned."
				"\n"
				"Arguments:\n"
				"1. flight_ceiling   (real, optional) The new flight ceiling in meters.\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == rexjson::real_type)) {
		dronestate_.flight_ceiling_ = Altitude::from_meters(params[0].get_real());
	}
	return dronestate_.flight_ceiling_.meters();
}

rexjson::value UartRpcServer::rpc_restore_config(UART*, rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
				"sd_restore_config\n"
				"\nRestores the factory default configuration data."
				"\nPreviously stored configuration will be erased"
				"\nArguments:\n";
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	dronestate_ = DroneState();
	std::string default_config = dronestate_.boot_config_to_json().write(false, true, 0, 8);
	configdata_.program((void*)default_config.c_str(), default_config.size());
	return default_config;
}

rexjson::value UartRpcServer::rpc_set_accelerometer_correction_period(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_set_accelerometer_correction_period\n"
				"\nSet the correction period for the accelerometer."
				"\nThe attitude tracker will try to correct the attitude error for the"
				"\nspecified period. Shorter period means the error will be corrected"
				"\nfaster, but this will introduce noise from the accelerometer sensor."
				"\nLonger periods will suppress the noise, but it will take longer time"
				"\nto correct the error."
				"\n"
				"Arguments:\n"
				"1. n          (real) correction period\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	float period = params[0].get_real();
	if (period < 0.25)
		throw std::range_error("Invalid value");
	dronestate_.accelerometer_correction_period_ = period;
	return dronestate_.accelerometer_correction_period_;
}

rexjson::value UartRpcServer::rpc_set_gyro_factor(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_set_gyro_factor\n"
				"\nSet the correction factor for the gyro sensor."
				"\n"
				"Arguments:\n"
				"1. n          (real) gyro factor\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	float factor = params[0].get_real();
	if (factor < 0.25 || factor > 2.0)
		throw std::range_error("Invalid value");
	dronestate_.gyro_factor_ = factor;
	return dronestate_.gyro_factor_;
}

rexjson::value UartRpcServer::rpc_set_yaw_throttle_factor(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_set_yaw_throttle_factor\n"
				"\nSet the maximum percentage modification that can be"
				"applied to the current throttle to generate yaw torque."
				"\n"
				"Arguments:\n"
				"1. n          (real) maximum percentage modification of the throttle\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	float factor = params[0].get_real();
	if (factor > 0.90 || factor < 0)
		throw std::range_error("Invalid value");
	dronestate_.yaw_throttle_factor_ = factor;
	return dronestate_.yaw_throttle_factor_;
}


rexjson::value UartRpcServer::rpc_get_dronestate(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_get_dronestate\n"
	            "\nGet the current drone state."
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	return dronestate_.to_json();
}

rexjson::value UartRpcServer::rpc_get_dronestate_ex(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_get_dronestate_ex\n"
	            "\nGet the extended drone state."
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	return dronestate_.to_json_ex();
}

rexjson::value UartRpcServer::rpc_get_configdata(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_get_configdata\n"
	            "\nGet the current boot time firmware configuration data."
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	rexjson::value v;
	v.read((char*)configdata_.mem_, configdata_.size());
	std::string ret = v.write(false, true, 0, 8);
	return ret;
}

rexjson::value UartRpcServer::rpc_set_configdata(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_obj_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_set_configdata\n"
	            "\nSaves the boot time firmware configuration data in flash memory."
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	std::string configstr = params[0].write(false, true, 0, 8);
	configdata_.program((void*)configstr.c_str(), configstr.size());
	return configstr;
}

void UartRpcServer::jsonrpc_request_handler(UART* uart)
{
	rexjson::value response;
	std::string request;

	cached_request_ += uart->read();
	if (*cached_request_.rbegin() != '\n')
		return;
	request = cached_request_;
	cached_request_.clear();
	trim(request);
	if (request.empty())
		return;
	response = call(uart, request);
	uart->write(response.write(false, false, 0, 4) + "\n");
}
