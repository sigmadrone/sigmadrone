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
#ifndef CLIENTAPP_H
#define CLIENTAPP_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "libhttp/http_client.hpp"
#include "librexjson/rexjson++.h"
#include "cmdargs.h"

class client_app : private boost::noncopyable
{
public:
	client_app(const cmd_args& args);
	~client_app();
	int run(int argc, const char *argv[]);
	std::vector<std::string> parse_command_line(int argc, const char *argv[]);
	std::string create_rpc_request(const std::string& method, const std::vector<std::string>& params, const std::vector<unsigned int>& params_types);
	std::string create_rpc_specrequest(const std::string& method);
	static bool is_client(int argc, const char *argv[]);

protected:
	const cmd_args& args_;
	boost::shared_ptr<logfile> logfile_;
	boost::scoped_ptr<http::client::http_client> rpc_client_;
};


#endif // APPLICATION_CLIENT_H
