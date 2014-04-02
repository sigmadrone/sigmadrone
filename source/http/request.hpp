//
// request.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>
#include <vector>
#include "headers.hpp"

namespace http {
namespace server {

/// A request received from a client.
struct request
{
	std::string method;
	std::string uri;
	int http_version_major;
	int http_version_minor;
	http::headers headers;
	std::string content;
	size_t headers_size;

	std::string request_line() const;
	void reset();
};

std::ostream& operator<<(std::ostream& os, const request& req);

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HPP
