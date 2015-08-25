/*
 * uartrpcserver.cpp
 *
 *  Created on: Jul 21, 2015
 *      Author: mkstoilo
 */
#include "trimstr.h"
#include "uartrpcserver.h"
#include "librexjsonrpc/jsonserialization.h"


UartRpcServer::UartRpcServer(DroneState& dronestate)
	: rpc_server<UartRpcServer, UART*>()
	, dronestate_(dronestate)
{
	add("sd_get_altitude", &UartRpcServer::rpc_get_altitude);
	add("sd_get_attitude", &UartRpcServer::rpc_get_attitude);
	add("sd_get_pressure", &UartRpcServer::rpc_get_pressure);
	add("sd_get_temperature", &UartRpcServer::rpc_get_temperature);
	add("sd_get_motors", &UartRpcServer::rpc_get_motors);
	add("sd_get_dronestate", &UartRpcServer::rpc_get_dronestate);
	add("kp", &UartRpcServer::rpc_kp);
	add("ki", &UartRpcServer::rpc_ki);
	add("kd", &UartRpcServer::rpc_kd);
	add("kp_yaw", &UartRpcServer::rpc_kp_yaw);
	add("ki_yaw", &UartRpcServer::rpc_ki_yaw);
	add("kd_yaw", &UartRpcServer::rpc_kd_yaw);
	add("sd_set_accelerometer_correction_period", &UartRpcServer::rpc_set_accelerometer_correction_period);
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

rexjson::value UartRpcServer::rpc_kp_yaw(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "kp_yaw\n"
	            "\nGet/Set Kp for the yaw axis."
				"\nIf the new coefficient is not specified, the current yaw axis Kp will be returned."
				"\n"
				"Arguments:\n"
				"1. Kp          (real, optional) The Kp of the yaw axis PID controller.\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == rexjson::real_type))
		dronestate_.kp_yaw_ = params[0].get_real();
	return dronestate_.kp_yaw_;
}

rexjson::value UartRpcServer::rpc_kd_yaw(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "kd_yaw\n"
	            "\nGet/Set Kd for the yaw axis."
				"\nIf the new coefficient is not specified, the current yaw axis Kd will be returned."
				"\n"
				"Arguments:\n"
				"1. Kd          (real, optional) The Kd of the yaw axis PID controller.\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == rexjson::real_type))
		dronestate_.kd_yaw_ = params[0].get_real();
	return dronestate_.kd_yaw_;
}

rexjson::value UartRpcServer::rpc_ki_yaw(UART* , rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "ki_yaw\n"
	            "\nGet/Set Ki for the yaw axis."
				"\nIf the new coefficient is not specified, the current yaw axis Ki will be returned."
				"\n"
				"Arguments:\n"
				"1. Ki          (real, optional) The Ki of the yaw axis PID controller.\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == rexjson::real_type))
		dronestate_.ki_yaw_ = params[0].get_real();
	return dronestate_.ki_yaw_;
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
	dronestate_.accelerometer_correction_period_ = params[0].get_real();
	return params[0].get_real();
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
