#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "serverapp.h"
#include "userrpcserver.h"
#include "libjsonspirit/jsonserialization.h"


user_rpcserver::user_rpcserver(server_app& app, boost::asio::io_service& io_service, const std::string& address, const std::string& port)
	: http_server(io_service, address, port)
	, app_(app)
{
	add("help", &user_rpcserver::rpc_help);
	add("spec", &user_rpcserver::rpc_spec);
	add("myaddress", &user_rpcserver::rpc_myaddress);
	add("servorate", &user_rpcserver::rpc_servo_rate);
	add("servoenable", &user_rpcserver::rpc_servo_enable);
	add("servosetoffset", &user_rpcserver::rpc_servo_setoffset);
	add("servogetpulse", &user_rpcserver::rpc_servo_getpulse);
	add("servogetpulsems", &user_rpcserver::rpc_servo_getpulsems);
	add("sd_exit", &user_rpcserver::rpc_exit);
	add("sd_run", &user_rpcserver::rpc_run);
	add("sd_get_running", &user_rpcserver::rpc_get_running);
	add("sd_reset", &user_rpcserver::rpc_reset);
	add("sd_get_thrust", &user_rpcserver::rpc_get_thrust);
	add("sd_set_thrust", &user_rpcserver::rpc_set_thrust);
	add("sd_get_motors", &user_rpcserver::rpc_get_motors);
	add("sd_get_earth_g_vector", &user_rpcserver::rpc_get_earth_g);
	add("sd_set_earth_g_vector", &user_rpcserver::rpc_set_earth_g);
	add("sd_get_attitude", &user_rpcserver::rpc_get_attitude);
	add("sd_get_accelerometer", &user_rpcserver::rpc_get_accelerometer);
	add("thrust", &user_rpcserver::rpc_thrust);
	add("ki", &user_rpcserver::rpc_ki);
	add("kd", &user_rpcserver::rpc_kd);
	add("kp", &user_rpcserver::rpc_kp);

	add_uri_handler("/jsonrpc", boost::bind(&user_rpcserver::jsonrpc_request_handler, this, _1, _2, _3));
	add_uri_handler("/", boost::bind(&user_rpcserver::jsonrpc_request_handler, this, _1, _2, _3));
	add_uri_handler("/echo", boost::bind(&user_rpcserver::echo_request_handler, this, _1, _2, _3));
	add_uri_handler("/headers", boost::bind(&user_rpcserver::headers_request_handler, this, _1, _2, _3));
	add_uri_handler("/method", boost::bind(&user_rpcserver::method_request_handler, this, _1, _2, _3));
}

user_rpcserver::~user_rpcserver()
{
}

json::value user_rpcserver::rpc_spec(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = { rpc_str_type };
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
				"spec <\"name\">\n"
				"\nGet the spec for the specified rpc name.\n"
				"\nArguments:\n"
				"1. \"name\"     (string, required) The name of of the rpc method to get the spec on\n"
				"\nResult:\n"
				"\"json\"     (string) The rpc call spec in json\n";
	}

	verify_parameters(params, types, ARRAYSIZE(types));
	json::array ignored;
	return call_method_name(connection, params[0], ignored, spec);
}

json::value user_rpcserver::rpc_help(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = { (rpc_str_type | rpc_null_type) };
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
				"help [\"command\"]\n"
				"\nList all commands, or get help for a specified command.\n"
				"\nArguments:\n"
				"1. \"command\"     (string, optional) The command to get help on\n"
				"\nResult:\n"
				"\"text\"     (string) The help text\n";
	}

	verify_parameters(params, types, ARRAYSIZE(types));
	if (params[0].type() == json::null_type) {
		std::string result;
		for (method_map_type::const_iterator it = map_.begin(); it != map_.end(); it++) {
			json::array ignored;
			std::string ret = call_method_name(connection, json::value(it->first), ignored, help).get_str();
			result += ret.substr(0, ret.find('\n')) + "\n";
		}
		return result;
	}
	json::array ignored;
	return call_method_name(connection, params[0], ignored, help);
}

json::value user_rpcserver::rpc_exit(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
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


json::value user_rpcserver::rpc_myaddress(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
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
	json::object ret;
	ret["address"] = connection->get_remote_address();
	ret["port"] = connection->get_remote_port();
	return ret;
}

json::value user_rpcserver::rpc_servo_enable(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_bool_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "servoenable\n"
	            "\nEnable/Disable servo controller."
				"\n"
				"Arguments:\n"
				"1. enable          (bool, required) true|false\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if (params[0].get_bool()) {
		app_.servoctrl_->enable();
		return "Servo is enabled";
	}
	app_.servoctrl_->disable();
	return "Servo is disabled";
}

json::value user_rpcserver::rpc_servo_setoffset(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_int_type, rpc_real_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "servosetoffset\n"
	            "\nSet the servo motor position. The offset can be any value between -1.0 and +1.0"
				"\n 0.0 -> neutral position."
				"\n-1.0 -> the maximum negative position"
				"\n 1.0 -> the maximum positive position"
				"\n"
				"Arguments:\n"
				"1. motor          (int, required) The channel of the motor\n"
				"2. offset         (double, required) The offset of the motor position. Must be between -1.0 and +1.0\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	app_.servoctrl_->motor(params[0].get_int()).offset(params[1].get_real());
	app_.servoctrl_->update();
	return params[1];
}

json::value user_rpcserver::rpc_servo_getpulse(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_int_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "servogetpulse\n"
	            "\nGet the servo current PWM pulse"
				"\n"
				"Arguments:\n"
				"1. motor          (int, required) The channel of the motor\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	return (int)app_.servoctrl_->getpulse(params[0].get_int());
}

json::value user_rpcserver::rpc_servo_getpulsems(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_int_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "servogetpulsems\n"
	            "\nGet the servo current PWM pulse in mSec"
				"\n"
				"Arguments:\n"
				"1. motor          (int, required) The channel of the motor\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	return app_.servoctrl_->getpulse_ms(params[0].get_int());
}

json::value user_rpcserver::rpc_get_attitude(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
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
	return quaternion_to_json_value(app_.ctrl_thread_.get_attitude());
}

json::value user_rpcserver::rpc_get_accelerometer(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_get_accelerometer\n"
	            "\nGet the current accelerometer reading"
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	return matrix_to_json_value(app_.ssampler_->data.acc3d_.normalize());
}


json::value user_rpcserver::rpc_get_earth_g(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_get_earth_g_vector\n"
	            "\nGet the the earth G vector when the body is completely leveled."
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	return  matrix_to_json_value(app_.attitude_tracker_->get_earth_g());
}

json::value user_rpcserver::rpc_set_earth_g(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_array_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_set_earth_g_vector\n"
	            "\nSet the the earth G vector when the body is completely leveled."
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	Vector3d earth_g = matrix_from_json_value<double, 3, 1>(params[0]);
	app_.attitude_tracker_->set_earth_g(earth_g);
	app_.attitude_tracker_->reset_attitude();
	return  matrix_to_json_value(app_.attitude_tracker_->get_earth_g());
}


json::value user_rpcserver::rpc_run(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_run\n"
	            "\nRun the controller."
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	app_.ctrl_thread_.start();
	return "Controller started.";
}

json::value user_rpcserver::rpc_get_running(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_get_running\n"
	            "\nCheck if the controller is running."
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	return app_.ctrl_thread_.is_running();
}


json::value user_rpcserver::rpc_reset(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_reset\n"
	            "\nStop the controller."
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	app_.ctrl_thread_.stop();
	return "Controller stopped.";
}


json::value user_rpcserver::rpc_servo_rate(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_int_type|rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "servorate\n"
	            "\nGet/Set servo controller PWM rate."
				"\nIf the rate is not specified, the current rate will be returned."
				"\n"
				"Arguments:\n"
				"1. rate          (int, optional) The rate of the PWM pulse.\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == json::int_type))
		app_.servoctrl_->setrate(params[0].get_int());
	return (int)app_.servoctrl_->getrate();
}

json::value user_rpcserver::rpc_kp(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
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
				"\nIf the new coeficient is not specified, the current Kp will be returned."
				"\n"
				"Arguments:\n"
				"1. Kp          (real, optional) The Kp of the PID controller.\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == json::real_type))
		app_.ctrl_thread_.pid_.Kp_ = params[0].get_real();
	return app_.ctrl_thread_.pid_.Kp_;
}

json::value user_rpcserver::rpc_kd(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
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
				"\nIf the new coeficient is not specified, the current Kd will be returned."
				"\n"
				"Arguments:\n"
				"1. Kd          (real, optional) The Kd of the PID controller.\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == json::real_type))
		app_.ctrl_thread_.pid_.Kd_ = params[0].get_real();
	return app_.ctrl_thread_.pid_.Kd_;
}

json::value user_rpcserver::rpc_ki(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
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
				"\nIf the new coeficient is not specified, the current Ki will be returned."
				"\n"
				"Arguments:\n"
				"1. Ki          (real, optional) The Ki of the PID controller.\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == json::real_type))
		app_.ctrl_thread_.pid_.Ki_ = params[0].get_real();
	return app_.ctrl_thread_.pid_.Ki_;
}

json::value user_rpcserver::rpc_get_motors(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
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
	json::array ret;
	for (size_t i = 0; i < app_.servoctrl_->channelcount(); i++) {
		if (app_.servoctrl_->motor(0).valid()) {
			ret.push_back(app_.servoctrl_->motor(i).offset());
		}
	}
	return ret;
}


json::value user_rpcserver::rpc_thrust(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type|rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "thrust\n"
	            "\nGet/Set thrust."
				"\nIf the new thrust is not specified, the current thrust will be returned."
				"\n"
				"Arguments:\n"
				"1. n          (real, optional) New thrust from 0.0 to 1.0\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	if ((params[0].type() == json::real_type))
		app_.ctrl_thread_.thrust_ = params[0].get_real();
	return app_.ctrl_thread_.thrust_;
}

json::value user_rpcserver::rpc_get_thrust(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_get_thrust\n"
	            "\nGet thrust."
				"\nReturn the current thrust"
				"\n"
				"Arguments:\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	return app_.ctrl_thread_.thrust_;
}

json::value user_rpcserver::rpc_set_thrust(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_real_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, ARRAYSIZE(types));
		if (mode == helpspec)
			return create_json_helpspec(types, ARRAYSIZE(types));
		return
	            "sd_set_thrust\n"
	            "\nSet thrust."
				"\nSet the current thrust will be returned."
				"\n"
				"Arguments:\n"
				"1. n          (real) New thrust from 0.0 to 1.0\n"
				;
	}
	verify_parameters(params, types, ARRAYSIZE(types));
	app_.ctrl_thread_.thrust_ = params[0].get_real();
	return app_.ctrl_thread_.thrust_;
}

void user_rpcserver::headers_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep)
{
	rep.headers.insert_header("Content-Type", "text/plain");

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

	rep.content = req.content;
	rep.status = http::server::reply::ok;
}

void user_rpcserver::method_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep)
{
	rep.headers.insert_header("Content-Type", "text/plain");
	rep.content = req.method + "\n";
	rep.status = http::server::reply::ok;
}

void user_rpcserver::jsonrpc_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep)
{
	json::value jsonreq, result;

	json::read(req.content, jsonreq);
	result = call(connection.shared_from_this(), jsonreq);
	rep.content = json::write(result) + "\n";
	rep.status = http::server::reply::ok;
}
