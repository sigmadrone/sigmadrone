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
#include "reply.hpp"
#include <string>
#include <boost/lexical_cast.hpp>

namespace http {
namespace server {

namespace status_strings {

const std::string ok = "HTTP/1.0 200 OK\r\n";
const std::string created = "HTTP/1.0 201 Created\r\n";
const std::string accepted = "HTTP/1.0 202 Accepted\r\n";
const std::string no_content = "HTTP/1.0 204 No Content\r\n";
const std::string multiple_choices = "HTTP/1.0 300 Multiple Choices\r\n";
const std::string moved_permanently = "HTTP/1.0 301 Moved Permanently\r\n";
const std::string moved_temporarily = "HTTP/1.0 302 Moved Temporarily\r\n";
const std::string not_modified = "HTTP/1.0 304 Not Modified\r\n";
const std::string bad_request = "HTTP/1.0 400 Bad Request\r\n";
const std::string unauthorized = "HTTP/1.0 401 Unauthorized\r\n";
const std::string forbidden = "HTTP/1.0 403 Forbidden\r\n";
const std::string not_found = "HTTP/1.0 404 Not Found\r\n";
const std::string internal_server_error = "HTTP/1.0 500 Internal Server Error\r\n";
const std::string not_implemented = "HTTP/1.0 501 Not Implemented\r\n";
const std::string bad_gateway = "HTTP/1.0 502 Bad Gateway\r\n";
const std::string service_unavailable = "HTTP/1.0 503 Service Unavailable\r\n";


const std::string& get_status_line(reply::status_type status)
{
	switch (status) {
	case reply::ok:
		return (ok);
	case reply::created:
		return (created);
	case reply::accepted:
		return (accepted);
	case reply::no_content:
		return (no_content);
	case reply::multiple_choices:
		return (multiple_choices);
	case reply::moved_permanently:
		return (moved_permanently);
	case reply::moved_temporarily:
		return (moved_temporarily);
	case reply::not_modified:
		return (not_modified);
	case reply::bad_request:
		return (bad_request);
	case reply::unauthorized:
		return (unauthorized);
	case reply::forbidden:
		return (forbidden);
	case reply::not_found:
		return (not_found);
	case reply::internal_server_error:
		return (internal_server_error);
	case reply::not_implemented:
		return (not_implemented);
	case reply::bad_gateway:
		return (bad_gateway);
	case reply::service_unavailable:
		return (service_unavailable);
	default:
		return (internal_server_error);
	}
}

boost::asio::const_buffer to_buffer(reply::status_type status)
{
	return boost::asio::buffer(get_status_line(status));
}


} // namespace status_strings

namespace misc_strings {

const char name_value_separator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };

} // namespace misc_strings

std::vector<boost::asio::const_buffer> reply::to_buffers()
{
	std::vector<boost::asio::const_buffer> buffers;
	buffers.push_back(status_strings::to_buffer(status));
	for (headers::const_iterator it = headers.begin(); it != headers.end(); it++) {
		if (!it->second.empty()) {
			buffers.push_back(boost::asio::buffer(it->first));
			buffers.push_back(boost::asio::buffer(misc_strings::name_value_separator));
			buffers.push_back(boost::asio::buffer(it->second));
			buffers.push_back(boost::asio::buffer(misc_strings::crlf));
		}
	}
	buffers.push_back(boost::asio::buffer(misc_strings::crlf));
	buffers.push_back(boost::asio::buffer(content));
	return buffers;
}


namespace stock_replies {

const char ok[] = "";
const char created[] =
	"<html>"
	"<head><title>Created</title></head>"
	"<body><h1>201 Created</h1></body>"
	"</html>";
const char accepted[] =
	"<html>"
	"<head><title>Accepted</title></head>"
	"<body><h1>202 Accepted</h1></body>"
	"</html>";
const char no_content[] =
	"<html>"
	"<head><title>No Content</title></head>"
	"<body><h1>204 Content</h1></body>"
	"</html>";
const char multiple_choices[] =
	"<html>"
	"<head><title>Multiple Choices</title></head>"
	"<body><h1>300 Multiple Choices</h1></body>"
	"</html>";
const char moved_permanently[] =
	"<html>"
	"<head><title>Moved Permanently</title></head>"
	"<body><h1>301 Moved Permanently</h1></body>"
	"</html>";
const char moved_temporarily[] =
	"<html>"
	"<head><title>Moved Temporarily</title></head>"
	"<body><h1>302 Moved Temporarily</h1></body>"
	"</html>";
const char not_modified[] =
	"<html>"
	"<head><title>Not Modified</title></head>"
	"<body><h1>304 Not Modified</h1></body>"
	"</html>";
const char bad_request[] =
	"<html>"
	"<head><title>Bad Request</title></head>"
	"<body><h1>400 Bad Request</h1></body>"
	"</html>";
const char unauthorized[] =
	"<html>"
	"<head><title>Unauthorized</title></head>"
	"<body><h1>401 Unauthorized</h1></body>"
	"</html>";
const char forbidden[] =
	"<html>"
	"<head><title>Forbidden</title></head>"
	"<body><h1>403 Forbidden</h1></body>"
	"</html>";
const char not_found[] =
	"<html>"
	"<head><title>Not Found</title></head>"
	"<body><h1>404 Not Found</h1></body>"
	"</html>";
const char internal_server_error[] =
	"<html>"
	"<head><title>Internal Server Error</title></head>"
	"<body><h1>500 Internal Server Error</h1></body>"
	"</html>";
const char not_implemented[] =
	"<html>"
	"<head><title>Not Implemented</title></head>"
	"<body><h1>501 Not Implemented</h1></body>"
	"</html>";
const char bad_gateway[] =
	"<html>"
	"<head><title>Bad Gateway</title></head>"
	"<body><h1>502 Bad Gateway</h1></body>"
	"</html>";
const char service_unavailable[] =
	"<html>"
	"<head><title>Service Unavailable</title></head>"
	"<body><h1>503 Service Unavailable</h1></body>"
	"</html>";

std::string to_string(reply::status_type status)
{
	switch (status) {
	case reply::ok:
		return ok;
	case reply::created:
		return created;
	case reply::accepted:
		return accepted;
	case reply::no_content:
		return no_content;
	case reply::multiple_choices:
		return multiple_choices;
	case reply::moved_permanently:
		return moved_permanently;
	case reply::moved_temporarily:
		return moved_temporarily;
	case reply::not_modified:
		return not_modified;
	case reply::bad_request:
		return bad_request;
	case reply::unauthorized:
		return unauthorized;
	case reply::forbidden:
		return forbidden;
	case reply::not_found:
		return not_found;
	case reply::internal_server_error:
		return internal_server_error;
	case reply::not_implemented:
		return not_implemented;
	case reply::bad_gateway:
		return bad_gateway;
	case reply::service_unavailable:
		return service_unavailable;
	default:
		return internal_server_error;
	}
}

} // namespace stock_replies


void reply::reset()
{
	status = ok;
	content.clear();
	headers.clear();
}

std::string reply::status_line() const
{
	return std::string(status_strings::get_status_line(status));
}

reply reply::stock_reply(reply::status_type status)
{
	reply rep;
	rep.status = status;
	rep.content = stock_replies::to_string(status);
	rep.headers.insert_header("Content-Length", boost::lexical_cast<std::string>(rep.content.size()));
	rep.headers.insert_header("Content-Type", "text/html");
	return rep;
}

std::ostream& operator<<(std::ostream& os, const reply& rep)
{
	os << rep.status_line();
	for (headers::const_iterator it = rep.headers.begin(); it != rep.headers.end(); it++) {
		os << it->first;
		os << std::string(misc_strings::name_value_separator, sizeof(misc_strings::name_value_separator));
		os << it->second;
		os << std::string(misc_strings::crlf, sizeof(misc_strings::crlf));
	}
	os << std::string(misc_strings::crlf, sizeof(misc_strings::crlf));
	os << rep.content;
	return os;
}

} // namespace server
} // namespace http
