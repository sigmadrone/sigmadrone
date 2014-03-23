#ifndef HTTP_PARAMETERS_HPP
#define HTTP_PARAMETERS_HPP


namespace http {
namespace server {

const int max_inactive_timeout = 60;			// No activity on a cannection timeout
const int max_headers_size = 8 * 1024;			// Maximum allowed headers size
const int max_content_size = 2 * 1024 * 1024;	// Maximum allowed content size


} // namespace server
} // namespace http

#endif // HTTP_REPLY_HPP
