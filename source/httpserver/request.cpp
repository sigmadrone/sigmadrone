#include "request.hpp"
#include <string>
#include <boost/lexical_cast.hpp>

namespace http {
namespace server {

void request::reset()
{
	method.clear();
	uri.clear();
	http_version_major = http_version_minor = 0;
	headers.clear();
	content.clear();
	headers_size = 0;
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
