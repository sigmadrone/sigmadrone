#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "headers.hpp"

namespace http {
namespace server {

class connection;

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

	request();
	std::string request_line() const;
	void reset();
};

std::ostream& operator<<(std::ostream& os, const request& req);

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HPP
