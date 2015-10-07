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
#ifndef _USERRPCSERVER_H_
#define _USERRPCSERVER_H_

#include "rpcserver.h"
#include "libhttp/http_server.hpp"


class server_app;

class user_rpcserver
	: public rpc_server<user_rpcserver, http::server::connection_ptr>
	, public http::server::http_server
{
public:
	user_rpcserver(server_app& app, boost::asio::io_service& io_service, const std::string& address, const std::string& port);
	~user_rpcserver();

	rexjson::value rpc_exit(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_myaddress(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_servo_enable(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_servo_rate(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_servo_setoffset(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_servo_getpulse(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_servo_getpulsems(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_attitude(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_accelerometer(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_magnetometer(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_earth_g(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_set_earth_g(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_correction_thrust(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_set_correction_thrust(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_run(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_running(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_reset(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_ki(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_kd(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_kp(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_yaw_ki(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_yaw_kd(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_yaw_kp(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_thrust(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_thrust(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_set_thrust(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_set_accelerometer_correction_period(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_motors(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_dronestate(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);


protected:
	void firmware_jsonrpc_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep);
	void jsonrpc_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep);
	void echo_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep);
	void headers_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep);
	void method_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep);

protected:
	server_app& app_;
};


#endif // _USERRPCSERVER_H_
