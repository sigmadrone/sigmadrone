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

#ifndef SIGMAD_BLACKBOX_H_
#define SIGMAD_BLACKBOX_H_

#include <string>
#include <iostream>
#include <thread>
#include "logfile.h"
#include "libhttp/http_client.hpp"

class server_app;

class black_box {
public:
	black_box(server_app& server_app);
	~black_box();
	void start_recording();
	void stop_recording();
private:
	void recorder_thread();
	void get_drone_state(std::string& state, http::client::http_client& client, bool ext_state);
	void record_new_state(const std::string& state);
	static size_t desired_log_size();
	static std::string create_rpc_request(bool ext_state);
private:
	static const size_t default_log_size_ = 512 * 1024 * 1024;
	server_app& server_app_;
	logfile logfile_;
	std::thread thread_;
	bool recording_;
};

#endif /* SIGMAD_BLACKBOX_H_ */
