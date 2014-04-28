#ifndef HTTPCLIENT_HPP
#define HTTPCLIENT_HPP

#include <string>
#include <cstdarg>
#include <stdexcept>
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

	struct connect_condition
	{
		connect_condition(http_client& client) : client_(client) {};
		template<typename Iterator>
		Iterator operator()(const boost::system::error_code& ec, Iterator next)
		{
			if (ec)
				client_.log_error_message("Connect error: %s", ec.message().c_str());
			client_.log_debug_message("Trying to connect: %s", next->endpoint().address().to_string().c_str());
			return next;
		}
		http_client& client_;
	};

	/// Construct the server to listen on the specified TCP address and port, and
	/// serve up files from the given directory.
	explicit http_client(const std::string& server, const std::string& port, size_t timeout = 0);
	~http_client();
	void timeout(size_t millisec);
	size_t timeout();
	void request(http::client::response& response,
			const std::string& method,
			const std::string& url,
			const std::string& content = std::string(),
			const http::headers& userheaders = headers()) throw(std::exception);
	void request(http::client::response& response,
			const std::string& method,
			const std::string& url,
			const std::string& content,
			const http::headers& userheaders,
			boost::system::error_code& ec);
	void request(http::client::response& response,
			const std::string& method,
			const std::string& url,
			const std::string& content,
			boost::system::error_code& ec);
	void request(http::client::response& response,
			const std::string& method,
			const std::string& url,
			boost::system::error_code& ec);
	void set_logger(http::logger_ptr ptr);

private:
	void handle_async_bytestransferred(const boost::system::error_code& e, std::size_t bytes_transferred);
	size_t async_read_data(boost::system::error_code& ec);
	void handle_async_bytestransferred_timeout(const boost::system::error_code& e);

	void async_connect_endpoint();
	void handle_async_connect_timeout(const boost::system::error_code& e);
	void handle_async_connect(const boost::system::error_code& e);

	void handle_async_resolve_timeout(const boost::system::error_code& e);
	void handle_async_resolve(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
	void async_resolve();

	boost::system::error_code read_headers(http::client::response& response);
	boost::system::error_code read_content(http::client::response& response);
	boost::system::error_code send_request(http::client::response& response, const std::vector<boost::asio::const_buffer>& request_buffers);
	boost::system::error_code async_write_request_buffers(const std::vector<boost::asio::const_buffer>& request_buffers);
	void add_default_headers(http::headers& headers, size_t content_size);
	std::vector<boost::asio::const_buffer> prepare_request_buffers(
			const std::string& method,
			const std::string& url,
			const std::string& content,
			const http::headers& headers);
	void disconnect();

protected:
	bool log_debug_message(const char *fmt, ...);
	bool log_warning_message(const char *fmt, ...);
	bool log_error_message(const char *fmt, ...);

private:
	size_t io_timeout_;
	size_t bytes_transferred_;
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
	boost::asio::deadline_timer timer_;
	boost::system::error_code ec_;
};

} // namespace server
} // namespace client

#endif // HTTP_CLIENTSERVICE_HPP
