#ifndef HTTP_CONNECTION_HPP
#define HTTP_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "reply.hpp"
#include "request.hpp"
#include "request_handler.hpp"
#include "request_parser.hpp"

namespace http {
namespace server {

class request_handler;
class connection_manager;
class http_server;

/// Represents a single connection from a client.
class connection: public boost::enable_shared_from_this<connection>, private boost::noncopyable
{
public:
	typedef boost::array<char, 96> buffer_type;

	/// Construct a connection with the given io_service.
	explicit connection(http_server& server, connection_manager& manager, request_handler& handler);

	/// Get the socket associated with the connection.
	boost::asio::ip::tcp::socket& socket();

	/// Start the first asynchronous operation for the connection.
	void start();

	/// Stop all asynchronous operations associated with the connection.
	void stop();

	void handle_pending_request();

	std::string get_remote_address() const;

	std::string get_remote_port() const;

	void set_eagain();

	void reset_eagain();

	std::string get_local_address();

	std::string get_local_port();

	std::string get_remote_address();

	std::string get_remote_port();


protected:
	/// Handle completion of a read operation.
	void handle_headers_read(const boost::system::error_code& e, std::size_t bytes_transferred);

	/// Handle completion of a read operation.
	void handle_content_read(const boost::system::error_code& e, std::size_t bytes_transferred);

	/// Handle completion of a write operation.
	void handle_reply_write(const boost::system::error_code& e, std::size_t bytes_transferred);

	/// Handle completion of a write operation.
	void handle_connection_timeout(const boost::system::error_code& e);

	/// Schedule async read HTTP headers operation
	void schedule_headers_read();

	/// Schedule async read HTTP payload operation
	void schedule_content_read();

	/// Schedule async write reply operation
	void schedule_reply_write();

	void schedule_timeout();

	void reply_with_error(reply::status_type status);

	/// HTTP Server
	http_server& server_;

	/// Socket for the connection.
	boost::asio::ip::tcp::socket socket_;

	/// Connection timeout
	boost::asio::deadline_timer timer_;

	/// The manager for this connection.
	connection_manager& connection_manager_;

	/// The handler used to process the incoming request.
	request_handler& request_handler_;

	/// Buffer for incoming data.
	buffer_type buffer_;

	/// The parser for the incoming request.
	request_parser request_parser_;

	/// The incoming request.
	request request_;

	/// The reply to be sent back to the client.
	reply reply_;

	size_t serial_;

	bool eagain_;

	std::string remote_address_;

	std::string remote_port_;

	std::string remote_;
};

typedef boost::shared_ptr<connection> connection_ptr;

} // namespace server
} // namespace http

#endif // HTTP_CONNECTION_HPP
