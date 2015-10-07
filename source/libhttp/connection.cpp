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
#include <vector>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string.hpp>
#include "request_handler.hpp"
#include "connection_manager.hpp"
#include "http_server.hpp"
#include "parameters.hpp"

namespace http {
namespace server {


connection::connection(http_server& server, connection_manager& manager, request_handler& handler)
	: server_(server)
	, socket_(server_.io_service())
	, timer_(server_.io_service())
	, connection_manager_(manager)
	, request_handler_(handler)
	, serial_(0)
	, eagain_(false)
{

}

boost::asio::ip::tcp::socket& connection::socket()
{
	return socket_;
}

std::string connection::get_remote_address() const
{
	return remote_address_;
}

std::string connection::get_remote_port() const
{
	return remote_port_;
}

void connection::set_eagain()
{
	eagain_ = true;
}

void connection::reset_eagain()
{
	eagain_ = false;
}


void connection::start()
{
	remote_address_ = socket_.remote_endpoint().address().to_string();
	remote_port_ = boost::lexical_cast<std::string>(socket_.remote_endpoint().port());
	remote_ = remote_address_ + ":" + remote_port_;
	schedule_headers_read();
	server_.log_debug_message("Starting new connection to remote: %s", remote_.c_str());

}

void connection::stop()
{
	// Initiate graceful connection closure.
	boost::system::error_code ignored_ec;
	socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
	socket_.close();
}

void connection::schedule_timeout()
{
	timer_.cancel();
	timer_.expires_from_now(boost::posix_time::seconds(http::server::max_inactive_timeout));
	timer_.async_wait(boost::bind(&connection::handle_connection_timeout, this, boost::asio::placeholders::error));
}

void connection::schedule_headers_read()
{
	schedule_timeout();
	socket_.async_read_some(boost::asio::buffer(buffer_),
		boost::bind(&connection::handle_headers_read,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}

void connection::schedule_content_read()
{
	schedule_timeout();
	boost::asio::async_read(socket_, boost::asio::buffer(buffer_),
							boost::asio::transfer_at_least(1),
							boost::bind(&connection::handle_content_read,
									this,
									boost::asio::placeholders::error,
									boost::asio::placeholders::bytes_transferred));

}

void connection::schedule_reply_write()
{
	schedule_timeout();
	boost::asio::async_write(
		socket_,
		reply_.to_buffers(),
		boost::bind(&connection::handle_reply_write, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
}

void connection::reply_with_error(reply::status_type status)
{
	reply_ = reply::stock_reply(status);
	reply_.headers.insert_header("Connection", "close");
	schedule_reply_write();
}

void connection::handle_connection_timeout(const boost::system::error_code& e)
{
	if ( e != boost::asio::error::operation_aborted) {
		connection_manager_.stop(shared_from_this());
	}
}


void connection::handle_pending_request()
{
	if (eagain_) {
		reset_eagain();
		reply_.reset();
		request_handler_.handle_request(*this, request_, reply_, serial_);
		if (!eagain_)
			schedule_reply_write();
	}
}

void connection::handle_content_read(const boost::system::error_code& e, std::size_t bytes_transferred)
{
	server_.log_debug_message("  --->connection::handle_content_read, error:  %s, bytes_transferred: %lu", e.message().c_str(), bytes_transferred);
	++serial_;
	if (!e) {
		request_.content.append(buffer_.data(), buffer_.data() + bytes_transferred);
		if (request_.content.size() > max_content_size) {
			connection_manager_.stop(shared_from_this());
		}

		if (request_.content.size() < request_.headers.content_length()) {
			schedule_content_read();
		} else {
			reset_eagain();
			request_handler_.handle_request(*this, request_, reply_, serial_);
			if (!eagain_)
				schedule_reply_write();
		}
	} else if (e != boost::asio::error::operation_aborted) {
		connection_manager_.stop(shared_from_this());
	}
}

void connection::handle_headers_read(const boost::system::error_code& e, std::size_t bytes_transferred)
{
	++serial_;
	server_.log_debug_message("  --->connection::handle_headers_read, error:  %s, bytes_transferred: %lu", e.message().c_str(), bytes_transferred);

	if (!e) {
		boost::tribool result;
		size_t parsed_size = 0;
		boost::tie(result, parsed_size) = request_parser_.parse_headers(request_, buffer_.data(), buffer_.data() + bytes_transferred);
		request_.headers_size += parsed_size;
		if (request_.headers_size > max_headers_size || request_.headers.content_length() > max_content_size) {
			server_.log_error_message("Bad Request from remote %s, headers size: %lu, content size: %lu",
					remote_.c_str(),
					(unsigned long)request_.headers_size,
					(unsigned long)request_.content.size());
			reply_with_error(reply::bad_request);
			return;
		}

		if (result) {
			request_.content.append(buffer_.data() + parsed_size, buffer_.data() + bytes_transferred);
			if (request_.content.size() < request_.headers.content_length()) {
				schedule_content_read();
			} else {
				reset_eagain();
				request_handler_.handle_request(*this, request_, reply_, serial_);
				if (!eagain_)
					schedule_reply_write();
			}
		} else if (!result) {
			reply_ = reply::stock_reply(reply::bad_request);
			schedule_reply_write();
		} else {
			schedule_headers_read();
		}
	} else if (e != boost::asio::error::operation_aborted) {
		server_.log_debug_message("Closing connection to remote %s", remote_.c_str());
		connection_manager_.stop(shared_from_this());
	}
}

void connection::handle_reply_write(const boost::system::error_code& e, std::size_t bytes_transferred)
{
	++serial_;
	server_.log_debug_message("  --->connection::handle_reply_write, error:  %s, bytes_transferred: %lu", e.message().c_str(), bytes_transferred);

	if (!e) {
		if (boost::iequals(reply_.headers.find_header("Connection"), "keep-alive")) {
			request_parser_.reset();
			request_.reset();
			reply_.reset();
			schedule_headers_read();
		} else {
			server_.log_debug_message("Closing connection to remote %s, Connection header is set to: %s",
					remote_.c_str(),
					reply_.headers.find_header("Connection").c_str());
			connection_manager_.stop(shared_from_this());
		}
	} else if (e != boost::asio::error::operation_aborted) {
		server_.log_debug_message("Closing connection to remote %s", remote_.c_str());
		connection_manager_.stop(shared_from_this());
	}
}

std::string connection::get_local_address()
{
	std::string address;
	try {
		address = socket_.local_endpoint().address().to_string();
	} catch (boost::system::system_error& e) {
		address.clear();
	}
	return address;
}

std::string connection::get_local_port()
{
	std::string port;
	try {
		unsigned int local_port = socket_.local_endpoint().port();
		port = boost::lexical_cast<std::string>(local_port);
	} catch (boost::system::system_error& e) {
		port.clear();
	}
	return port;
}


std::string connection::get_remote_address()
{
	std::string address;
	try {
		address = socket_.remote_endpoint().address().to_string();
	} catch (boost::system::system_error& e) {
		address.clear();
	}
	return address;
}

std::string connection::get_remote_port()
{
	std::string port;
	try {
		unsigned int remote_port = socket_.remote_endpoint().port();
		port = boost::lexical_cast<std::string>(remote_port);
	} catch (boost::system::system_error& e) {
		port.clear();
	}
	return port;
}


} // namespace server
} // namespace http
