#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>
#include <vector>
#include <map>
#include <boost/asio.hpp>
#include "headers.hpp"

namespace http {
namespace client {

/// A reply to be sent to a client.
struct response
{
	/// The status of the reply.
	int http_version_major;
	int http_version_minor;
	int status;
	std::string status_text;

	/// The headers, included in the response.
	http::headers headers;

	/// The content to be sent in the reply.
	std::string content;
};


} // namespace server
} // namespace http

#endif // HTTP_RESPONSE_HPP
