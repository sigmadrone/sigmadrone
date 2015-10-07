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
#include "request.hpp"
#include <string>
#include <boost/lexical_cast.hpp>

namespace http {
namespace server {

request::request()
{
	reset();
}

void request::reset()
{
	method.clear();
	uri.clear();
	http_version_major = http_version_minor = 0;
	headers.clear();
	content.clear();
	headers_size = 0UL;
}

std::string request::request_line() const
{
	std::stringstream oss;
	oss << method << " ";
	oss << uri << " ";
	oss << "HTTP/" << http_version_major << "." << http_version_minor << "\r\n";
	return oss.str();
}


std::ostream& operator<<(std::ostream& os, const request& req)
{
	os << req.request_line();
	for (headers::const_iterator it = req.headers.begin(); it != req.headers.end(); it++) {
		os << it->first << ": " << it->second << "\n";
	}
	os << "\n";
	os << req.content;
	return os;
}


} // namespace server
} // namespace http
