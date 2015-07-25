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
	rpc_client_uart(const std::string& filename, speed_t speed = B115200, size_t usec = 0, const std::string& jsonrpc_version = "1.0");
	virtual ~rpc_client_uart();
	void request(const std::string& str);
	std::string response();
	rexjson::value call(const std::string& method, const rexjson::array& params = rexjson::array()) throw(std::exception);

protected:
	int readtimeout(char *buf, size_t size);

protected:
	int fd_;
	size_t timeout_;
	std::string jsonrpc_version_;
	size_t serial_;
};

#endif /* _RPCCLIENTUART_H_ */
