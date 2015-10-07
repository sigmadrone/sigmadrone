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
#ifndef _RPCCLIENT_H_
#define _RPCCLIENT_H_

#include <string>
#include <stdexcept>
#include "librexjson/rexjson++.h"
#include "libhttp/http_client.hpp"

class rpc_client_http : public http::client::http_client
{
public:
	rpc_client_http(const std::string& address, std::string port, size_t timeout = 0, const std::string& jsonrpc_version = "1.0");
	~rpc_client_http();
	rexjson::value call(const std::string& url,
			const std::string& method,
			const rexjson::array& params,
			http::headers headers = http::headers()) throw(std::exception);

	rexjson::value call(const std::string& url,
			const std::string& method,
			http::headers headers = http::headers()) throw(std::exception);

	rexjson::value call(const std::string& url,
			const std::string& method,
			const rexjson::value& val1,
			http::headers headers = http::headers()) throw(std::exception);

	rexjson::value call(const std::string& url,
			const std::string& method,
			const rexjson::value& val1,
			const rexjson::value& val2,
			http::headers headers = http::headers()) throw(std::exception);

	rexjson::value call(const std::string& url,
			const std::string& method,
			const rexjson::value& val1,
			const rexjson::value& val2,
			const rexjson::value& val3,
			http::headers headers = http::headers()) throw(std::exception);

	size_t get_failure_count();

protected:
	std::string jsonrpc_version_;
	size_t serial_;
	size_t failures_;
};


#endif // _RPCCLIENT_H_
