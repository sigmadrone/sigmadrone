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

rpc_client_uart::rpc_client_uart(const std::string& filename, speed_t speed, size_t usec, const std::string& jsonrpc_version)
	: fd_(-1)
	, timeout_(usec)
	, jsonrpc_version_(jsonrpc_version)
	, serial_(0)
	, cache_head_(cache_)
	, cache_tail_(cache_)
{
	struct termios tio;

	fd_ = ::open(filename.c_str(), O_RDWR);
	if (fd_ < 0)
		throw std::runtime_error(
				std::string("Failed to open file: " + filename));
	if (!::isatty(fd_)) {
		::close(fd_);
		throw std::runtime_error(std::string(filename + " : Not a tty device"));
	}

	/* Set the funny terminal modes. */
	tcgetattr(fd_, &tio);
	tio.c_lflag &= ~(ECHO | ICANON); /* Clear ICANON and ECHO. */
	tio.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR
			| ICRNL | IXON);
	tio.c_oflag &= ~OPOST;
	tio.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	tio.c_cflag &= ~(CSIZE | PARENB);
	tio.c_cflag &= ~CRTSCTS;
	tio.c_cflag |= (CS8);
	tio.c_cc[VMIN] = 1;
	tio.c_cc[VTIME] = 0;
	cfsetospeed(&tio, speed);
	cfsetispeed(&tio, speed);
	tcsetattr(fd_, TCSANOW, &tio);
	tcflush(fd_, TCIOFLUSH);
}

rpc_client_uart::~rpc_client_uart()
{
	tcflush(fd_, TCIOFLUSH);
	::close(fd_);
}

int rpc_client_uart::readcache(char *buf, size_t size)
{
	int ret = 0;

	for (size_t i = 0; i < size && cache_tail_ < cache_head_; i++) {
		buf[i] = *cache_tail_++;
		ret++;
		if (buf[i] == '\n')
			break;
	}
	return ret;
}

int rpc_client_uart::readtimeout(char *buf, size_t size)
{
	fd_set rfds;
	struct timeval tv;
	int retval;

	if (cache_head_ != cache_tail_)
		return readcache(buf, size);
	cache_head_ = cache_tail_ = cache_;

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
		ssize_t ret = ::read(fd_, cache_, sizeof(cache_));
		if (ret < 0)
			return ret;
		cache_head_ += ret;
		return readcache(buf, size);
	}

	/* No data within timeout value */
	return 0;
}

std::string rpc_client_uart::response()
{
	char buf[256];
	int count = 0;

	response_.clear();
	while ((count = readtimeout(buf, sizeof(buf))) > 0) {
		std::string ret(buf, count);
		response_ += ret;
		if (buf[count - 1] == '\n')
			break;
	}
	if (count < 0)
		throw std::runtime_error("rpc_client_uart::response timed out");
	return response_;
}

void rpc_client_uart::request(const std::string& req)
{
	std::string str = req + "\n";
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

std::string rpc_client_uart::json_rpc_request(const std::string& json)
{
	request(json);
	return response();
}

rexjson::value rpc_client_uart::call(const std::string& method, const rexjson::array& params) throw(std::exception)
{
	rexjson::value rpc_response;
	rexjson::object rpc_request;
	rexjson::object rpc_error;
	std::string content;

	rpc_request["jsonrpc"] = jsonrpc_version_;
	rpc_request["id"] = (int)++serial_;
	rpc_request["method"] = rexjson::value(method);
	rpc_request["params"] = params;
	content = json_rpc_request(rexjson::write(rpc_request));
	try {
		rpc_response.read(content);
	} catch (std::exception& e) {
		log_error_message("rpc_client_uart::call exception: %s, CONTENT: \n%s", e.what(), content.c_str());
		throw;
	}
	if (rpc_response.get_obj()["error"].type() == rexjson::obj_type)
		throw std::runtime_error(rpc_response.get_obj()["error"].get_obj()["message"].get_str());
	return rpc_response.get_obj()["result"];
}

rexjson::value rpc_client_uart::call(const std::string& method) throw(std::exception)
{
	rexjson::array params;
	return call(method, params);
}


rexjson::value rpc_client_uart::call(
		const std::string& method,
		const rexjson::value& val1
		) throw(std::exception)
{
	rexjson::array params;
	params.push_back(val1);
	return call(method, params);
}

rexjson::value rpc_client_uart::call(
		const std::string& method,
		const rexjson::value& val1,
		const rexjson::value& val2
		) throw(std::exception)
{
	rexjson::array params;
	params.push_back(val1);
	params.push_back(val2);
	return call(method, params);
}

rexjson::value rpc_client_uart::call(
		const std::string& method,
		const rexjson::value& val1,
		const rexjson::value& val2,
		const rexjson::value& val3
		) throw(std::exception)
{
	rexjson::array params;
	params.push_back(val1);
	params.push_back(val2);
	params.push_back(val3);
	return call(method, params);
}

rexjson::value rpc_client_uart::call(
		const std::string& method,
		const rexjson::value& val1,
		const rexjson::value& val2,
		const rexjson::value& val3,
		const rexjson::value& val4
		) throw(std::exception)
{
	rexjson::array params;
	params.push_back(val1);
	params.push_back(val2);
	params.push_back(val3);
	params.push_back(val4);
	return call(method, params);
}
