/*
 * uartrpcserver.cpp
 *
 *  Created on: Jul 21, 2015
 *      Author: mkstoilo
 */

#include "uartrpcserver.h"
#include "librexjsonrpc/jsonserialization.h"

UartRpcServer::UartRpcServer(DroneState& dronestate)
	: rpc_server<UartRpcServer, UART*>()
	, dronestate_(dronestate)
{
	add("sd_get_attitude", &UartRpcServer::rpc_get_attitude);

}

UartRpcServer::~UartRpcServer()
{

}

rexjson::value UartRpcServer::rpc_get_attitude(UART* uart, rexjson::array& params, rpc_exec_mode mode)
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


void UartRpcServer::jsonrpc_request_handler(UART* uart)
{
	rexjson::value response;
	std::string request = uart->readline();

	if (request.empty())
		return;
	response = call(uart, request);
	uart->write(response.write(false, false, 0, 8) + "\n");
}
