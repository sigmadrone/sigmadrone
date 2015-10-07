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
#include "rpcclienthttp.h"


rpc_client_http::rpc_client_http(const std::string& address, std::string port, size_t timeout, const std::string& jsonrpc_version)
	: http::client::http_client(address, port, timeout)
	, jsonrpc_version_(jsonrpc_version)
	, serial_(0)
	, failures_(0)
{
}

rpc_client_http::~rpc_client_http()
{
}

size_t rpc_client_http::get_failure_count()
{
	return failures_;
}

rexjson::value rpc_client_http::call(const std::string& url, const std::string& method, const rexjson::array& params, http::headers headers) throw(std::exception)
{
	rexjson::value rpc_response;
	rexjson::object rpc_request;
	rexjson::object rpc_error;
	http::client::response response;

	if (headers.find_header("Connection").empty())
		headers.insert_header("Connection", "keep-alive");
	if (headers.find_header("content-type").empty())
		headers.insert_header("content-type", "application/json-rpc");
	rpc_request["jsonrpc"] = jsonrpc_version_;
	rpc_request["id"] = (int)++serial_;
	rpc_request["method"] = rexjson::value(method);
	rpc_request["params"] = params;
	++failures_;
	request(response, "POST", url, rexjson::write(rpc_request), headers);
	failures_ = 0;
	rpc_response.read(response.content);
	if (rpc_response.get_obj()["error"].type() == rexjson::obj_type)
		throw std::runtime_error(rpc_response.get_obj()["error"].get_obj()["message"].get_str());
	return rpc_response.get_obj()["result"];
}

rexjson::value rpc_client_http::call(const std::string& url, const std::string& method, http::headers headers) throw(std::exception)
{
	rexjson::array params;
	return call(url, method, params, headers);
}

rexjson::value rpc_client_http::call(const std::string& url, const std::string& method, const rexjson::value& val1, http::headers headers) throw(std::exception)
{
	rexjson::array params;
	params.push_back(val1);
	return call(url, method, params);
}

rexjson::value rpc_client_http::call(const std::string& url, const std::string& method, const rexjson::value& val1, const rexjson::value& val2, http::headers headers) throw(std::exception)
{
	rexjson::array params;
	params.push_back(val1);
	params.push_back(val2);
	return call(url, method, params);
}

rexjson::value rpc_client_http::call(
		const std::string& url,
		const std::string& method,
		const rexjson::value& val1,
		const rexjson::value& val2,
		const rexjson::value& val3,
		http::headers headers) throw(std::exception)
{
	rexjson::array params;
	params.push_back(val1);
	params.push_back(val2);
	params.push_back(val3);
	return call(url, method, params);
}


