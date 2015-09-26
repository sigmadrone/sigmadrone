/*
 * blackbox.cpp
 *
 *  Created on: Sep 17, 2015
 *      Author: svassilev
 */

#include "blackbox.h"
#include <chrono>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "librexjson/rexjson++.h"
#include "rpcserver.h"
#include "serverapp.h"

black_box::black_box(server_app& server_app) :
    server_app_(server_app),
	logfile_("sd_blackbox.log", black_box::desired_log_size(), logfile::info, true),
	thread_(),
	recording_(false)
{
}

black_box::~black_box()
{
	stop_recording();
}

void black_box::start_recording()
{
	logfile_.set_rotation_extension(".prev");
	logfile_.enable_disable_date_prefix(true);
	logfile_.add_log(logfile::info,"BEGIN LOG");
	//logfile_.enable_disable_date_prefix(false);
	recording_ = true;
	thread_ = std::thread(&black_box::recorder_thread, this);
}

void black_box::stop_recording()
{
	recording_ = false;
	thread_.join();
}

void black_box::recorder_thread()
{
	static const std::string host = "127.0.0.1";//
	static const std::string port = "18222";
	std::string drone_state;
	http::client::http_client http_client(host, port, 30000);
	while (recording_) {
		try {
			get_drone_state(drone_state, http_client);
			record_new_state(drone_state);
		} catch (std::exception& e) {
			// log to the log file
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}

void black_box::get_drone_state(std::string& state,
		http::client::http_client& http_client)
{
	state = "";
	try {
		rexjson::value val;
		http::client::response response;
		http::headers headers;
		http_client.request(response, "POST", "/firmware", create_rpc_request(), headers);
		val.read(response.content);
		if (val.get_obj()["error"].type() != rexjson::obj_type) {
			state = val.get_obj()["result"].write(true);
			state.erase(remove(state.begin(), state.end(), '\n'), state.end());
			state.erase(remove(state.begin(), state.end(), ' '), state.end());
		}
	} catch (std::exception& e) {
		//std::cout << "RPC command failed! " << e.what() << std::endl;
	}
}

std::string black_box::create_rpc_request()
{
	rexjson::object rpc_request;
	rexjson::array parameters;
	rpc_request["jsonrpc"] = "1.0";
	rpc_request["id"] = "clientid";
	rpc_request["method"] = rexjson::value("sd_get_dronestate");
	rpc_request["params"] = parameters;
	return rexjson::write(rpc_request);
}

void black_box::record_new_state(const std::string& state)
{
	logfile_.add_log(logfile::info, "%s", state.c_str());
}

size_t black_box::desired_log_size()
{
	size_t log_size = default_log_size_;
	try {
		boost::filesystem::space_info spi = boost::filesystem::space("/");
		if (spi.available < log_size * 2) {
			log_size = spi.available / 2;
		}
	} catch (...) {
	}
	return log_size;
}
