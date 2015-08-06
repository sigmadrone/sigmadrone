/*
 * rpcclientuart.h
 *
 *  Created on: Jul 24, 2015
 *      Author: Martin Stoilov
 */

#ifndef _RPCCLIENTUART_H_
#define _RPCCLIENTUART_H_

#include <termios.h>
#include "librexjson/rexjson++.h"

class rpc_client_uart {
public:
	rpc_client_uart(const std::string& filename, speed_t speed = B115200, size_t usec = 100000, const std::string& jsonrpc_version = "1.0");
	~rpc_client_uart();
	std::string json_rpc_request(const std::string& json);
	rexjson::value call(const std::string& method, const rexjson::array& params) throw(std::exception);

	rexjson::value call(
		const std::string& method
	) throw(std::exception);

	rexjson::value call(
		const std::string& method,
		const rexjson::value& val1
	) throw(std::exception);

	rexjson::value call(
		const std::string& method,
		const rexjson::value& val1,
		const rexjson::value& val2
	) throw(std::exception);

	rexjson::value call(
		const std::string& method,
		const rexjson::value& val1,
		const rexjson::value& val2,
		const rexjson::value& val3
	) throw(std::exception);

	rexjson::value call(
		const std::string& method,
		const rexjson::value& val1,
		const rexjson::value& val2,
		const rexjson::value& val3,
		const rexjson::value& val4
	) throw(std::exception);

	int get_fd() { return fd_; }

public:
	std::string response_;

protected:
	int readtimeout(char *buf, size_t size);
	int readcache(char *buf, size_t size);
	void request(const std::string& str);
	std::string response();

protected:
	int fd_;
	size_t timeout_;
	std::string jsonrpc_version_;
	size_t serial_;
	char cache_[1024];
	char *cache_head_, *cache_tail_;
};

#endif /* _RPCCLIENTUART_H_ */
