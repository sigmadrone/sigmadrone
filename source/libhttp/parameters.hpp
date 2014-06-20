#ifndef HTTP_PARAMETERS_HPP
#define HTTP_PARAMETERS_HPP


namespace http {
namespace server {

const unsigned int max_inactive_timeout = 90;			// No activity on a cannection timeout
const unsigned int max_headers_size = 8 * 1024;			// Maximum allowed headers size
const unsigned int max_content_size = 20 * 1024 * 1024;	// Maximum allowed content size


} // namespace server
} // namespace http

#endif // HTTP_REPLY_HPP
