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
#ifndef HTTP_APPLICATION_HPP
#define HTTP_APPLICATION_HPP

#include <string>
#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "libhttp/http_server.hpp"

/// The top-level class of the HTTP server.
class application : private boost::noncopyable
{
public:
	application();
	~application();
	void init(int argc, char *argv[]);
	int run();

protected:
	void signal_handler_terminate();
	void usage();

	void jsonrpc_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep);
	void echo_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep);
	void headers_request_handler(http::server::connection& connection, const http::server::request& req, http::server::reply& rep);

	boost::asio::io_service io_service_rpc_;
	boost::asio::signal_set signals_;
	std::string appname_;
	boost::scoped_ptr<http::server::http_server> rpc_server_;
};



#endif
