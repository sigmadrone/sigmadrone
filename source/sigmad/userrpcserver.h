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
	rexjson::value rpc_thrust(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_thrust(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_set_thrust(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_set_accelerometer_correction_period(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_motors(http::server::connection_ptr connection, rexjson::array& params, rpc_exec_mode mode = execute);

protected:
	void jsonrpc_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep);
	void echo_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep);
	void headers_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep);
	void method_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep);

protected:
	server_app& app_;
};


#endif // _USERRPCSERVER_H_
