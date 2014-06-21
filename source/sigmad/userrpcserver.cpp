#include "boost/bind.hpp"
#include "serverapp.h"
#include "userrpcserver.h"


user_rpcserver::user_rpcserver(server_app& app, boost::asio::io_service& io_service, const std::string& address, const std::string& port)
	: http_server(io_service, address, port)
	, app_(app)
{
	add("help", &user_rpcserver::rpc_help);
	add("spec", &user_rpcserver::rpc_spec);
	add("stop", &user_rpcserver::rpc_stop);
	add("myaddress", &user_rpcserver::rpc_myaddress);

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
			return create_json_spec(types, sizeof(types)/sizeof(types[0]));
		if (mode == helpspec)
			return create_json_helpspec(types, sizeof(types)/sizeof(types[0]));
		return
				"spec <\"name\">\n"
				"\nGet the spec for the specified rpc name.\n"
				"\nArguments:\n"
				"1. \"name\"     (string, required) The name of of the rpc method to get the spec on\n"
				"\nResult:\n"
				"\"json\"     (string) The rpc call spec in json\n";
	}

	verify_parameters(params, types, sizeof(types)/sizeof(types[0]));
	json::array ignored;
	return call_method_name(connection, params[0], ignored, spec);
}

json::value user_rpcserver::rpc_help(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = { (rpc_str_type | rpc_null_type) };
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, sizeof(types)/sizeof(types[0]));
		if (mode == helpspec)
			return create_json_helpspec(types, sizeof(types)/sizeof(types[0]));
		return
				"help [\"command\"]\n"
				"\nList all commands, or get help for a specified command.\n"
				"\nArguments:\n"
				"1. \"command\"     (string, optional) The command to get help on\n"
				"\nResult:\n"
				"\"text\"     (string) The help text\n";
	}

	verify_parameters(params, types, sizeof(types)/sizeof(types[0]));
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

json::value user_rpcserver::rpc_stop(http::server::connection_ptr connection, json::array& params, rpc_exec_mode mode)
{
	static unsigned int types[] = {rpc_null_type};
	if (mode != execute) {
		if (mode == spec)
			return create_json_spec(types, sizeof(types)/sizeof(types[0]));
		if (mode == helpspec)
			return create_json_helpspec(types, sizeof(types)/sizeof(types[0]));
		return
	            "stop\n"
	            "\nStop server.";
	}
	verify_parameters(params, types, sizeof(types)/sizeof(types[0]));
	app_.stop();
	return "Server stopping";
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

void user_rpcserver::headers_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep)
{
	rep.headers.header("Content-Type", "text/plain");

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
	rep.headers.header("Content-Type", "text/plain");

	rep.content = req.content;
	rep.status = http::server::reply::ok;
}

void user_rpcserver::method_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep)
{
	rep.headers.header("Content-Type", "text/plain");
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
