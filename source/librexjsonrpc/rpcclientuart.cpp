/*
 * rpcclientuart.cpp
 *
 *  Created on: Jul 24, 2015
 *      Author: Martin Stoilov
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string>
#include <stdexcept>
#include "rpcclientuart.h"

rpc_client_uart::rpc_client_uart(const std::string& filename, size_t usec, const std::string& jsonrpc_version)
	: fd_(-1)
	, timeout_(usec)
	, jsonrpc_version_(jsonrpc_version)
	, serial_(0)
{
	struct termios tio;

	fd_ = ::open(filename.c_str(), O_RDWR);
	if (fd_ < 0)
		throw std::runtime_error(std::string("Failed to open file: " + filename));
	if (!::isatty(fd_)) {
		::close(fd_);
		throw std::runtime_error(std::string(filename + " : Not a tty device"));
	}

    /* Set the funny terminal modes. */
    tcgetattr(fd_, &tio);
    tio.c_lflag &= ~(ECHO); /* Clear ICANON and ECHO. */
    tio.c_lflag |= ICANON;
    tio.c_cc[VMIN] = 1;
    tio.c_cc[VTIME] = 0;
    cfsetospeed(&tio, B115200);
    cfsetispeed(&tio, B115200);
    tcsetattr(fd_, TCSANOW, &tio);
    tcsetattr(fd_, TCSAFLUSH, &tio);
}

rpc_client_uart::~rpc_client_uart()
{
	::close(fd_);
}

int rpc_client_uart::readtimeout(char *buf, size_t size)
{
	fd_set rfds;
	struct timeval tv;
	int retval;

	/* Watch fd to see when it has input. */
	FD_ZERO(&rfds);
	FD_SET(fd_, &rfds);

	if (timeout_) {
		/* Wait up to five seconds. */
		tv.tv_sec = timeout_ / 1000000;
		tv.tv_usec = timeout_ % 1000000;
		retval = select(fd_ + 1, &rfds, NULL, NULL, &tv);
	} else {
		retval = select(fd_ + 1, &rfds, NULL, NULL, NULL);
	}

	if (retval == -1) {
		return -1;
	} else if (retval) {
		/* FD_ISSET(0, &rfds) will be true. */
		return ::read(fd_, buf, size);
	}

	/* No data within timeout value */
	return 0;
}

void rpc_client_uart::request(const std::string& str)
{
	const char *bufptr = str.c_str();
	size_t size = str.size();
	ssize_t ret = 0;

	while (size) {
		ret = write(fd_, (void*)bufptr, size);
		if (ret < 0)
			throw std::runtime_error("rpc_client_uart::write failed");
		size -= ret;
		bufptr += ret;
	}
}

std::string rpc_client_uart::response()
{
	std::string resp;
	char buf[256];
	int count = 0;

	while ((count = readtimeout(buf, sizeof(buf))) > 0) {
		std::string ret(buf, count);
		resp += ret;
		if (buf[count - 1] == '\n')
			break;
	}
	if (count < 0)
		throw std::runtime_error("rpc_client_uart::response timed out");
	return resp;
}

rexjson::value rpc_client_uart::call(const std::string& method, const rexjson::array& params) throw(std::exception)
{
	rexjson::value rpc_response;
	rexjson::object rpc_request;
	rexjson::object rpc_error;

	rpc_request["jsonrpc"] = jsonrpc_version_;
	rpc_request["id"] = (int)++serial_;
	rpc_request["method"] = rexjson::value(method);
	rpc_request["params"] = params;
	request(rexjson::write(rpc_request));
	rpc_response.read(response(), 20);
	if (rpc_response.get_obj()["error"].type() == rexjson::obj_type)
		throw std::runtime_error(rpc_response.get_obj()["error"].get_obj()["message"].get_str());
	return rpc_response.get_obj()["result"];
}


