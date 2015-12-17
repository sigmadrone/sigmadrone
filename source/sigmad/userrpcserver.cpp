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
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "serverapp.h"
#include "userrpcserver.h"
#include "jsonserialization.h"

user_rpcserver::user_rpcserver(server_app& app, boost::asio::io_service& io_service, const std::string& address, const std::string& port)
	: rpc_server<user_rpcserver, http::server::connection_ptr>()
	, http_server(io_service, address, port)
	, app_(app)
{
	add("myaddress", &user_rpcserver::rpc_myaddress);
	add("sd_exit", &user_rpcserver::rpc_exit);
	add("sd_set_accelerometer_correction_period", &user_rpcserver::rpc_set_accelerometer_correction_period);
	add("sd_get_motors", &user_rpcserver::rpc_get_motors);
	add("sd_get_dronestate", &user_rpcserver::rpc_get_dronestate);
	add("sd_get_attitude", &user_rpcserver::rpc_get_attitude);
	add("ki", &user_rpcserver::rpc_ki);
	add("kd", &user_rpcserver::rpc_kd);
	add("kp", &user_rpcserver::rpc_kp);
	add("yaw_ki", &user_rpcserver::rpc_yaw_ki);
	add("yaw_kd", &user_rpcserver::rpc_yaw_kd);
	add("yaw_kp", &user_rpcserver::rpc_yaw_kp);

	add_uri_handler("/firmware", boost::bind(&user_rpcserver::firmware_jsonrpc_request_handler, this, _1, _2, _3));
	add_uri_handler("/jsonrpc", boost::bind(&user_rpcserver::jsonrpc_request_handler, this, _1, _2, _3));
	add_uri_handler("/", boost::bind(&user_rpcserver::jsonrpc_request_handler, this, _1, _2, _3));
	add_uri_handler("/echo", boost::bind(&user_rpcserver::echo_request_handler, this, _1, _2, _3));
	add_uri_handler("/headers", boost::bind(&user_rpcserver::headers_request_handler, this, _1, _2, _3));
	add_uri_handler("/method", boost::bind(&user_rpcserver::method_request_handler, this, _1, _2, _3));
}

user_rpcserver::~user_rpcserver()
{
}

rexjson::value user_rpcserver::rpc_exit(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "exit\n"
	            "\nQuit server.";
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	app_.stop();
	return "Quiting...";
}


rexjson::value user_rpcserver::rpc_myaddress(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = { };
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, sizeof(types) / sizeof(types[0]));
		if (mode == helpspec)
			return create_json_helpspec(types, sizeof(types) / sizeof(types[0]));
		return
				"myaddress \n\n"
				"\nReturn the address of the remote connection making this rpc call\n"
				"\n"
				"Arguments:\n"
				"    none\n"
				;
	}
	verify_parameters(params, types, sizeof(types) / sizeof(types[0]));
	rexjson::object ret;
	ret["address"] = connection->get_remote_address();
	ret["port"] = connection->get_remote_port();
	return ret;
}

rexjson::value user_rpcserver::rpc_get_attitude(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode)
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
	return rpc_client_uart(app_.firmware_uart_, app_.firmware_uart_speed_).call("sd_get_attitude");
}


rexjson::value user_rpcserver::rpc_kp(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_int_type|rpc_null_type};
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
	return rpc_client_uart(app_.firmware_uart_, app_.firmware_uart_speed_).call("kp", params);
}

rexjson::value user_rpcserver::rpc_kd(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_int_type|rpc_null_type};
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
	return rpc_client_uart(app_.firmware_uart_, app_.firmware_uart_speed_).call("kd", params);
}

rexjson::value user_rpcserver::rpc_ki(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_int_type|rpc_null_type};
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
	return rpc_client_uart(app_.firmware_uart_, app_.firmware_uart_speed_).call("ki", params);
}

rexjson::value user_rpcserver::rpc_yaw_kp(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_int_type|rpc_null_type};
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
	return rpc_client_uart(app_.firmware_uart_, app_.firmware_uart_speed_).call("yaw_kp", params);
}

rexjson::value user_rpcserver::rpc_yaw_kd(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_int_type|rpc_null_type};
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
	return rpc_client_uart(app_.firmware_uart_, app_.firmware_uart_speed_).call("yaw_kd", params);
}

rexjson::value user_rpcserver::rpc_yaw_ki(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_int_type|rpc_null_type};
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
	return rpc_client_uart(app_.firmware_uart_, app_.firmware_uart_speed_).call("yaw_ki", params);
}


rexjson::value user_rpcserver::rpc_get_motors(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode)
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
	return rpc_client_uart(app_.firmware_uart_, app_.firmware_uart_speed_).call("sd_get_motors");
}

rexjson::value user_rpcserver::rpc_get_dronestate(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_get_dronestate\n"
	            "\nGet drone state."
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	rpc_client_uart client(app_.firmware_uart_, app_.firmware_uart_speed_);
	client.set_log_file(get_log_file());
	return client.call("sd_get_dronestate");
}

rexjson::value user_rpcserver::rpc_set_accelerometer_correction_period(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_int_type};
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
	return rpc_client_uart(app_.firmware_uart_, app_.firmware_uart_speed_).call("sd_set_accelerometer_correction_period", params);
}

void user_rpcserver::headers_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep)
{
	rep.headers.insert_header("Content-Type", "text/plain");
	rep.headers.insert_header("Access-Control-Allow-Origin","*");

	rep.content.append(req.request_line());
	for (http::headers::const_iterator it = req.headers.begin(); it != req.headers.end(); it++) {
		std::string name = it->first;
		std::string value = it->second;
		rep.content.append(name + ": " + value + "\n");
	}
	rep.status = http::server::reply::ok;
}

void user_rpcserver::echo_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep)
{
	rep.headers.insert_header("Content-Type", "text/plain");
	rep.headers.insert_header("Access-Control-Allow-Origin","*");

	rep.content = req.content;
	rep.status = http::server::reply::ok;
}

void user_rpcserver::method_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep)
{
	rep.headers.insert_header("Content-Type", "text/plain");
	rep.headers.insert_header("Access-Control-Allow-Origin","*");
	rep.content = req.method + "\n";
	rep.status = http::server::reply::ok;
}

void user_rpcserver::jsonrpc_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep)
{
	rexjson::value jsonreq, result;

	result = call(connection.shared_from_this(), req.content);
	rep.content = rexjson::write(result) + "\n";
	rep.headers.insert_header("Access-Control-Allow-Origin","*");
	rep.status = http::server::reply::ok;
}

void user_rpcserver::firmware_jsonrpc_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep)
{
	rexjson::value jsonreq, result;

	try {
		rpc_client_uart uartcli(app_.firmware_uart_, app_.firmware_uart_speed_);
		uartcli.set_log_file(get_log_file());
		rep.content = uartcli.json_rpc_request(req.content) + "\n";

	} catch (std::exception& e) {
		jsonreq.read(req.content);
		result = create_rpc_error(RPC_MISC_ERROR, e.what());
		result.get_obj()["id"] = jsonreq.get_obj()["id"];
		rep.content = rexjson::write(result) + "\n";
	}
	rep.headers.insert_header("Access-Control-Allow-Origin","*");
	rep.status = http::server::reply::ok;
}
