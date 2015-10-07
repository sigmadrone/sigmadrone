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
#include "http_server.hpp"
#include <boost/bind.hpp>
#include <signal.h>

namespace http {
namespace server {


http_server::http_server(boost::asio::io_service& io_service, const std::string& address, const std::string& port)
	: logger("HTTP Server: ")
	, address_(address)
	, port_(port)
	, io_service_(io_service)
	, acceptor_(io_service_)
	, timer_(io_service_)
	, blocktimer_(io_service_)
	, connection_manager_(*this)
	, request_handler_(*this)
	, new_connection_()
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
	schedule_blocked_requests(5000);
	set_log_prefix(std::string("HTTP::SERVER") + "@" + address + ":" + port + "; ");
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
		blocktimer_.cancel();
		connection_manager_.stop_all();
	}
}

void http_server::schedule_blocked_requests(unsigned int millisec)
{

	if (acceptor_.is_open()) {
		blocktimer_.expires_from_now(boost::posix_time::milliseconds(millisec));
		blocktimer_.async_wait(boost::bind(&http_server::handle_blocked_requests, this, boost::asio::placeholders::error));
	}
}

void http_server::handle_blocked_requests(const boost::system::error_code& ec)
{
	if (!ec) {
		connection_manager_.handle_pending_requests();
		schedule_blocked_requests(5000);
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

size_t http_server::get_remote_connections(std::vector<std::string>& connections)
{
	return connection_manager_.get_remote_connections(connections);
}

} // namespace server
} // namespace http
