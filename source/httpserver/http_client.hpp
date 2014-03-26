#ifndef HTTPCLIENT_HPP
#define HTTPCLIENT_HPP

#include <string>
#include <cstdarg>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "headers.hpp"
#include "response.hpp"
#include "response_parser.hpp"
#include "logger.hpp"

namespace http {
namespace client {

/// The top-level class of the HTTP server.
class http_client: public boost::enable_shared_from_this<http_client>, private boost::noncopyable
{
public:
	typedef boost::array<char, 8192> buffer_type;

	/// Construct the server to listen on the specified TCP address and port, and
	/// serve up files from the given directory.
	explicit http_client();
	~http_client();
	boost::system::error_code connect(const std::string& server, const std::string& port);
	boost::system::error_code request(http::client::response& response, const std::string& method, const std::string& url, const std::string& content, http::headers headers = http::headers());
	void set_logger(http::logger_ptr ptr);

private:
	void async_request(const std::string& method, const std::string& url, const std::string& content);
	void handle_resolve(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
	void handle_connect(const boost::system::error_code& err);
	void handle_write_request(const boost::system::error_code& err);
	void handle_content_read(const boost::system::error_code& e, std::size_t bytes_transferred);
	void schedule_content_read();
	void schedule_connect();
	boost::system::error_code read_headers(http::client::response& response);
	boost::system::error_code read_content(http::client::response& response);
	boost::system::error_code send_request(http::client::response& response, const std::vector<boost::asio::const_buffer>& request_buffers);
	boost::system::error_code write_request_buffers(const std::vector<boost::asio::const_buffer>& request_buffers);
	void add_default_headers(http::headers& headers, size_t content_size);
	std::vector<boost::asio::const_buffer> prepare_request_buffers(
			const std::string& method,
			const std::string& url,
			const std::string& content,
			const http::headers& headers);
	void disconnect();
	boost::system::error_code connect_endpoint();
	bool log_debug_message(const char *fmt, ...);
	bool log_warning_message(const char *fmt, ...);
	bool log_error_message(const char *fmt, ...);

private:
	std::string server_;
	std::string port_;
	std::string log_prefix_;
	logger_ptr logger_;
	boost::asio::io_service io_service_;
	boost::asio::ip::tcp::resolver resolver_;
	boost::asio::ip::tcp::socket socket_;
	buffer_type buffer_;
	http::client::response_parser response_parser_;
	boost::asio::ip::tcp::resolver::iterator endpoint_;
};

} // namespace server
} // namespace client

#endif // HTTP_CLIENTSERVICE_HPP
