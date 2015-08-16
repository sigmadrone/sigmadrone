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
