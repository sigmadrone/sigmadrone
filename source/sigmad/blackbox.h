/*
 * blackbox.h
 *
 *  Created on: Sep 17, 2015
 *      Author: svassilev
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
