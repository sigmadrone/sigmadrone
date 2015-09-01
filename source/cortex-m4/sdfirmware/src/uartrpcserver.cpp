/*
 * uartrpcserver.cpp
 *
 *  Created on: Jul 21, 2015
 *      Author: mkstoilo
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
	add("ki", &UartRpcServer::rpc_ki);
	add("kd", &UartRpcServer::rpc_kd);
	add("kp", &UartRpcServer::rpc_kp);
	add("yaw_ki", &UartRpcServer::rpc_yaw_ki);
	add("yaw_kd", &UartRpcServer::rpc_yaw_kd);
	add("yaw_kp", &UartRpcServer::rpc_yaw_kp);
	add("sd_set_accelerometer_correction_period", &UartRpcServer::rpc_set_accelerometer_correction_period);
	add("sd_set_gyro_factor", &UartRpcServer::rpc_set_gyro_factor);
	add("sd_set_yaw_throttle_factor", &UartRpcServer::rpc_set_yaw_throttle_factor);
	add("sd_get_configdata", &UartRpcServer::rpc_get_configdata);
	add("sd_set_configdata", &UartRpcServer::rpc_set_configdata);
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

rexjson::value UartRpcServer::rpc_get_configdata(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "rpc_get_config\n"
	            "\nGet the current firmware configuration data."
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	return std::string((char*)configdata_.mem_);
}

rexjson::value UartRpcServer::rpc_set_configdata(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_str_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "rpc_get_config\n"
	            "\nSaves the firmware configuration data in the flash."
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	std::string configstr(params[0].get_str());
	configdata_.program((void*)configstr.c_str(), configstr.size());
	return std::string((char*)configdata_.mem_);
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
//	request = uart->read();
	trim(request);
	if (request.empty())
		return;
	response = call(uart, request);
	uart->write(response.write(false, false, 0, 8) + "\n");
}
