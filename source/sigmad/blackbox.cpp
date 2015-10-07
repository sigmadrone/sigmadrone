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
	get_drone_state(drone_state, http_client, true);
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	bool get_ex_state = false;
	while (recording_) {
		try {
			get_drone_state(drone_state, http_client, get_ex_state);
			record_new_state(drone_state);
		} catch (std::exception& e) {
			// log to the log file
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
		if (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() > 10) {
			get_ex_state = true;
			start = end;
		} else {
			get_ex_state = false;
		}
	}
}

void black_box::get_drone_state(std::string& state,
		http::client::http_client& http_client,
		bool extended_state)
{
	state = "";
	try {
		rexjson::value val;
		http::client::response response;
		http::headers headers;
		http_client.request(response, "POST", "/firmware", create_rpc_request(extended_state), headers);
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

std::string black_box::create_rpc_request(bool ext_state)
{
	rexjson::object rpc_request;
	rexjson::array parameters;
	rpc_request["jsonrpc"] = "1.0";
	rpc_request["id"] = "clientid";
	rpc_request["method"] = rexjson::value(ext_state ? "sd_get_dronestate_ex" : "sd_get_dronestate");
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
