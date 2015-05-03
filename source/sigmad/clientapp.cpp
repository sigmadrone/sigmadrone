#include <iostream>
#include "boost/bind.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "boost/thread.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include "clientapp.h"
#include "libhttp/base64.hpp"
#include "rpcserver.h"


client_app::client_app(const cmd_args& args)
	: args_(args)
	, logfile_(new logfile("sigmadcli.log", 1024*1024*10, logfile::none))
{
}

client_app::~client_app()
{
}

bool client_app::is_client(int argc, const char *argv[])
{
	bool client = false;
	for (int i = 1; i < argc; i++) {
		std::string arg(argv[i]);
		if (arg[0] != '-') {
			client = true;
			break;
		}
	}
	return client;
}

int client_app::run(int argc, const char *argv[])
{
	std::string host = args_.get_value("rpc-server", "127.0.0.1");
	std::string port = args_.get_value("rpc-port", "18222");
	std::string url = args_.get_value("rpc-uri", "/jsonrpc");
	std::string rpcuser = args_.get_value("rpcuser", "m");
	std::string rpcpassword = args_.get_value("rpcpassword", "x");

	try {
		http::client::response response;
		http::client::http_client client(host, port, 30000);
		logfile_->log_level(args_.get_value("loglevel", "info"));
		client.set_log_file(logfile_);
		http::headers headers;
		headers["Authorization"] = std::string("Basic ") + http::base64::encode(rpcuser + ":" + rpcpassword);
		std::vector<std::string> parsed_cmd_line = parse_command_line(argc, argv);
		if (!parsed_cmd_line.size())
			throw std::runtime_error("no RPC method");
		std::string method = parsed_cmd_line[0];
		std::vector<std::string> params(parsed_cmd_line.size() - 1);
		std::copy(parsed_cmd_line.begin() + 1, parsed_cmd_line.end(), params.begin());
		client.request(response, "POST", url, create_rpc_specrequest(method), headers);
		rexjson::value spec_result;
		spec_result.read(response.content);
		std::vector<unsigned int> rpc_types = rpc_server<>::rpc_types(spec_result.get_obj()["result"].get_array());
		client.request(response, "POST", url, create_rpc_request(method, params, rpc_types), headers);
//		std::cout << "Raw RPC response(DEBUG): " << response.content << std::endl;
		rexjson::value val;
		val.read(response.content);
		if (val.get_obj()["error"].type() == rexjson::obj_type ) {
			std::cout << "Error: " << val.get_obj()["error"].get_obj()["message"].get_str() << std::endl;
		} else if (val.get_obj()["result"].type() == rexjson::str_type) {
			std::cout << val.get_obj()["result"].get_str() << std::endl;
		} else {
			std::cout << val.get_obj()["result"].write(true) << std::endl;
		}	} catch (std::exception& e) {
		std::cout << "RPC command failed! " << e.what() << std::endl;
	}
	return 0;
}

std::vector<std::string> client_app::parse_command_line(int argc, const char *argv[])
{
	std::vector<std::string> ret;

	for (int i = 1; i < argc; i++) {
		std::string arg(argv[i]);
		if (arg[0] != '-') {
			ret.push_back(arg);
		}
	}
	return ret;
}

std::string client_app::create_rpc_request(const std::string& method,
		const std::vector<std::string>& params, const std::vector<unsigned int>& params_types)
{
	rexjson::object rpc_request;
	rexjson::array parameters;

	rpc_request["jsonrpc"] = "1.0";
	rpc_request["id"] = "clientid";
	rpc_request["method"] = rexjson::value(method);
	for (size_t i = 0; i < params_types.size(); i++) {
		if (i < params.size()) {
			rexjson::value val(params[i]);
			if (!(rpc_server<>::get_rpc_type(val.type()) & params_types[i])) {
				/*
				 * The default (string) interpretation is not valid type
				 * try to reinterpret it by parsing.
				 */
				if (!rexjson::read_no_throw(val, params[i]))
					throw std::runtime_error("Error parsing: " + params[i]);
				if (!(rpc_server<>::get_rpc_type(val.type()) & params_types[i]))
					throw std::runtime_error("Parameter: " + params[i] + " is of invalid type");
			}
			parameters.push_back(val);
		} else {

		}
	}
	rpc_request["params"] = parameters;
	return rexjson::write(rpc_request);
}

std::string client_app::create_rpc_specrequest(const std::string& method)
{
	rexjson::object rpc_request;
	rexjson::array parameters;

	rpc_request["jsonrpc"] = "1.0";
	rpc_request["id"] = "clientid";
	rpc_request["method"] = "spec";
	parameters.push_back(method);
	rpc_request["params"] = parameters;
	return rexjson::write(rpc_request);
}
