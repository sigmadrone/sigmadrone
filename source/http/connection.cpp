#include <vector>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "connection_manager.hpp"
#include "request_handler.hpp"
#include "connection.hpp"
#include "http_server.hpp"
#include "parameters.hpp"

namespace http {
namespace server {


connection::connection(http_server& server, connection_manager& manager, request_handler& handler)
	: server_(server)
	, socket_(server_.io_service())
	, timer_(server_.io_service())
	, connection_manager_(manager)
	, request_handler_(handler)
	, serial_(0)
{

}

boost::asio::ip::tcp::socket& connection::socket()
{
	return socket_;
}

void connection::start()
{
	remote_ = socket_.remote_endpoint().address().to_string() + ":" + boost::lexical_cast<std::string>(socket_.remote_endpoint().port());
	schedule_headers_read();
	server_.log_debug_message("Starting new connection to remote: %s", remote_.c_str());

}

void connection::stop()
{
	// Initiate graceful connection closure.
	boost::system::error_code ignored_ec;
	socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
	socket_.close();
}

void connection::schedule_timeout()
{
	timer_.cancel();
	timer_.expires_from_now(boost::posix_time::seconds(http::server::max_inactive_timeout));
	timer_.async_wait(boost::bind(&connection::handle_connection_timeout, this, boost::asio::placeholders::error));
}

void connection::schedule_headers_read()
{
	schedule_timeout();
	socket_.async_read_some(boost::asio::buffer(buffer_),
		boost::bind(&connection::handle_headers_read,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}

void connection::schedule_content_read()
{
	schedule_timeout();
	socket_.async_read_some(boost::asio::buffer(buffer_),
		boost::bind(&connection::handle_content_read,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}

void connection::schedule_reply_write()
{
	schedule_timeout();
	boost::asio::async_write(
		socket_,
		reply_.to_buffers(),
		boost::bind(&connection::handle_reply_write, shared_from_this(), boost::asio::placeholders::error));
}

void connection::reply_with_error(reply::status_type status)
{
	reply_ = reply::stock_reply(status);
	reply_.headers.header("Connection", "close");
	schedule_reply_write();
}

void connection::handle_connection_timeout(const boost::system::error_code& e)
{
	if ( e != boost::asio::error::operation_aborted) {
		connection_manager_.stop(shared_from_this());
	}
}

void connection::handle_content_read(const boost::system::error_code& e, std::size_t bytes_transferred)
{
	++serial_;
	if (!e) {
		request_.content.append(buffer_.data(), buffer_.data() + bytes_transferred);
		if (request_.content.size() > max_content_size) {
			connection_manager_.stop(shared_from_this());
		}

		if (request_.content.size() < request_.headers.content_length()) {
			schedule_content_read();
		} else {
//			request_.headers.header("From-Address", socket_.remote_endpoint().address().to_string());
			request_handler_.handle_request(request_, reply_, serial_);
			schedule_reply_write();
		}
	} else if (e != boost::asio::error::operation_aborted) {
		connection_manager_.stop(shared_from_this());
	}
}

void connection::handle_headers_read(const boost::system::error_code& e, std::size_t bytes_transferred)
{
	++serial_;

	if (!e) {
		boost::tribool result;
		size_t parsed_size = 0;
		boost::tie(result, parsed_size) = request_parser_.parse_headers(request_, buffer_.data(), buffer_.data() + bytes_transferred);
		request_.headers_size += parsed_size;
		if (request_.headers_size > max_headers_size || request_.headers.content_length() > max_content_size) {
			server_.log_error_message("Bad Request from remote %s, headers size: %lu, content size: %lu",
					remote_.c_str(),
					(unsigned long)request_.headers_size,
					(unsigned long)request_.content.size());
			reply_with_error(reply::bad_request);
			return;
		}

		if (result) {
			request_.content.append(buffer_.data() + parsed_size, buffer_.data() + bytes_transferred);
			if (request_.content.size() < request_.headers.content_length()) {
				schedule_content_read();
			} else {
				request_.headers.header("From-Address", socket_.remote_endpoint().address().to_string());
				request_handler_.handle_request(request_, reply_, serial_);
				schedule_reply_write();
			}
		} else if (!result) {
			reply_ = reply::stock_reply(reply::bad_request);
			schedule_reply_write();
		} else {
			schedule_headers_read();
		}
	} else if (e != boost::asio::error::operation_aborted) {
		server_.log_debug_message("Closing connection to remote %s", remote_.c_str());
		connection_manager_.stop(shared_from_this());
	}
}

void connection::handle_reply_write(const boost::system::error_code& e)
{
	++serial_;
	if (!e) {
		if (reply_.headers["Connection"] == "keep-alive") {
			request_parser_.reset();
			request_.reset();
			reply_.reset();
			schedule_headers_read();
		} else {
			server_.log_debug_message("Closing connection to remote %s, Connection header is set to: %s",
					remote_.c_str(),
					reply_.headers["Connection"].c_str());
			connection_manager_.stop(shared_from_this());
		}
	} else if (e != boost::asio::error::operation_aborted) {
		server_.log_debug_message("Closing connection to remote %s", remote_.c_str());
		connection_manager_.stop(shared_from_this());
	}
}

} // namespace server
} // namespace http
