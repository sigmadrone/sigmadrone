//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "http_server.hpp"
#include <boost/bind.hpp>
#include <signal.h>

namespace http {
namespace server {


http_server::http_server(boost::asio::io_service& io_service, const std::string& address, const std::string& port)
	: address_(address)
	, port_(port)
	, io_service_(io_service)
	, acceptor_(io_service_)
	, timer_(io_service_)
	, connection_manager_(*this)
	, request_handler_(*this)
	, new_connection_()
	, logger_()
	, log_prefix_(std::string("HTTP::SERVER") + "@" + address + ":" + port + "; ")
{
	// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
	boost::asio::ip::tcp::resolver resolver(io_service_);
	boost::asio::ip::tcp::resolver::query query(address, port);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
	acceptor_.open(endpoint.protocol());
	acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor_.bind(endpoint);
	acceptor_.listen();
	start_accept();
}

boost::asio::io_service& http_server::io_service()
{
	return io_service_;
}


void http_server::start_accept()
{
	new_connection_.reset(new connection(*this, connection_manager_, request_handler_));
	acceptor_.async_accept(new_connection_->socket(),
			boost::bind(&http_server::handle_accept, this, boost::asio::placeholders::error));
}

void http_server::handle_accept(const boost::system::error_code& e)
{
	// Check whether the server was stopped by a signal before this completion
	// handler had a chance to run.
	if (!acceptor_.is_open()) {
		return;
	}

	if (!e) {
		connection_manager_.start(new_connection_);
	}

	start_accept();
}

void http_server::handle_stop(const boost::system::error_code& e)
{
	if ( e != boost::asio::error::operation_aborted) {
		// The server is stopped by cancelling all outstanding asynchronous
		// operations. Once all operations have finished the io_service::run() call
		// will exit.
		acceptor_.close();
		connection_manager_.stop_all();
	}
}

/*
 * The actual stopping of the server will not be done
 * from the calling thread, this call will rather schedule
 * the stop operation, which would occur in the context of
 * the io_service thread.
 */
void http_server::stop(unsigned int milliseconds)
{
	timer_.expires_from_now(boost::posix_time::milliseconds(milliseconds));
	timer_.async_wait(boost::bind(&http_server::handle_stop, this, boost::asio::placeholders::error));
}

bool http_server::log_debug_message(const char *fmt, ...)
{
	bool result = false;
	va_list args;

	if (!logger_)
		return result;
	va_start(args, fmt);
	std::string format(log_prefix_ + fmt);
	result = logger_->log_debug_message(format.c_str(), args);
	va_end(args);
	return result;
}

bool http_server::log_warning_message(const char *fmt, ...)
{
	bool result = false;
	va_list args;

	if (!logger_)
		return result;
	va_start(args, fmt);
	std::string format(log_prefix_ + fmt);
	result = logger_->log_warning_message(format.c_str(), args);
	va_end(args);
	return result;
}

bool http_server::log_error_message(const char *fmt, ...)
{
	bool result = false;
	va_list args;

	if (!logger_)
		return result;
	va_start(args, fmt);
	std::string format(log_prefix_ + fmt);
	result = logger_->log_error_message(format.c_str(), args);
	va_end(args);
	return result;
}

void http_server::set_logger(http::logger_ptr ptr)
{
		logger_ = ptr;
}

} // namespace server
} // namespace http
