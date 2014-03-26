
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "http_client.hpp"

namespace http {
namespace client {


http_client::http_client()
	: server_()
	, port_()
	, log_prefix_(std::string("HTTP::CLIENT") + "; ")
	, logger_()
	, io_service_()
	, resolver_(io_service_)
	, socket_(io_service_)
	, buffer_()
	, endpoint_()
{
}

http_client::~http_client()
{
	disconnect();
}

void http_client::connect(const std::string& server, const std::string& port) throw(std::exception)
{
	disconnect();
	server_ = server;
	port_ = port;
	boost::asio::ip::tcp::resolver::query query(server_, port_);
	log_prefix_ = std::string("HTTP::CLIENT") + "@" + server_ + ":" + port_ + "; ";
	endpoint_ = resolver_.resolve(query);
	connect_endpoint();
}


void http_client::connect(const std::string& server, const std::string& port, boost::system::error_code& ec)
{
	try {
		connect(server, port);
	} catch (boost::system::system_error& e) {
		ec = e.code();
	}
}


void http_client::connect_endpoint()
{
	if (boost::asio::connect(socket_, endpoint_) == boost::asio::ip::tcp::resolver::iterator()) {
		log_error_message("Failed to connect to %s:%s", server_.c_str(), port_.c_str());
	}
}

void http_client::connect_endpoint(boost::system::error_code& ec)
{
	try {
		connect_endpoint();
	} catch (boost::system::system_error& e) {
		ec = e.code();
	}
}

void http_client::disconnect()
{
	boost::system::error_code ignored;

	socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored);
	socket_.close();
}


namespace misc_strings {
const char name_value_separator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };
const char space[] = { ' ' };
const char http_1_0[] = { 'H', 'T', 'T', 'P', '/', '1', '.', '0'};
}

std::vector<boost::asio::const_buffer> http_client::prepare_request_buffers(
		const std::string& method,
		const std::string& url,
		const std::string& content,
		const http::headers& headers)
{
	std::vector<boost::asio::const_buffer> buffers;
	buffers.push_back(boost::asio::buffer(method));
	buffers.push_back(boost::asio::buffer(misc_strings::space));
	buffers.push_back(boost::asio::buffer(url));
	buffers.push_back(boost::asio::buffer(misc_strings::space));
	buffers.push_back(boost::asio::buffer(misc_strings::http_1_0));
	buffers.push_back(boost::asio::buffer(misc_strings::crlf));
	for (headers::const_iterator it = headers.begin(); it != headers.end(); it++) {
		buffers.push_back(boost::asio::buffer(it->first));
		buffers.push_back(boost::asio::buffer(misc_strings::name_value_separator));
		buffers.push_back(boost::asio::buffer(it->second));
		buffers.push_back(boost::asio::buffer(misc_strings::crlf));
	}
	buffers.push_back(boost::asio::buffer(misc_strings::crlf));
	if (content.size())
		buffers.push_back(boost::asio::buffer(content));
	return buffers;
}

void http_client::add_default_headers(http::headers& headers, size_t content_size)
{
	if (headers.header("Host").empty())
		headers.header("Host", server_);
	if (headers.header("Accept").empty())
		headers.header("Accept", "*/*");
	if (headers.header("Content-Length").empty())
		headers.header("Content-Length", boost::lexical_cast<std::string>(content_size));
	if (headers.header("Connection").empty())
		headers.header("Connection", "close");
}


void http_client::request(
		http::client::response& response,
		const std::string& method,
		const std::string& url,
		const std::string& content,
		boost::system::error_code& ec)
{
	http::headers userheaders;
	request(response, method, url, content, userheaders, ec);
}

void http_client::request(
		http::client::response& response,
		const std::string& method,
		const std::string& url,
		boost::system::error_code& ec)
{
	http::headers userheaders;
	std::string content;
	request(response, method, url, content, userheaders, ec);
}

void http_client::request(http::client::response& response,
		const std::string& method,
		const std::string& url,
		const std::string& content,
		const http::headers& userheaders,
		boost::system::error_code& ec)
{
	try {
		request(response, method, url, content, userheaders);
	} catch (boost::system::system_error& e) {
		ec = e.code();
	}
}


void http_client::request(http::client::response& response,
		const std::string& method,
		const std::string& url,
		const std::string& content,
		const http::headers& userheaders) throw(std::exception)
{
	boost::system::error_code ec;
	http::headers headers(userheaders);
	add_default_headers(headers, content.size());
	std::vector<boost::asio::const_buffer> request_buffers = prepare_request_buffers(method, url, content, headers);

	ec = send_request(response, request_buffers);
	if (ec == boost::asio::error::eof || ec == boost::asio::error::bad_descriptor) {
		disconnect();
		connect_endpoint();
		ec = send_request(response, request_buffers);
	}
	if (ec) {
		log_error_message("Error: %s, Failed to send the request", ec.message().c_str());
		response.content.clear();
		response.headers.clear();
		response.status = 0;
		boost::asio::detail::throw_error(ec, "http_client::request");
	}
}

boost::system::error_code http_client::send_request(
		http::client::response& response,
		const std::vector<boost::asio::const_buffer>& request_buffers)
{
	boost::system::error_code ec;

	response_parser_.reset();
	response.headers.clear();
	response.content.clear();
	response.status = 0;

	if ((ec = write_request_buffers(request_buffers)))
		goto error;
	if ((ec = read_headers(response)))
		goto error;
	if ((ec = read_content(response)))
		goto error;
	if (response.headers.header("Connection") != "keep-alive")
		disconnect();
error:
	return ec;
}

boost::system::error_code http_client::write_request_buffers(const std::vector<boost::asio::const_buffer>& request_buffers)
{
	boost::system::error_code ec;

	boost::asio::write(socket_, request_buffers, ec);
	return ec;
}

boost::system::error_code http_client::read_headers(http::client::response& response)
{
	boost::tribool result;
	size_t parsed_size = 0;
	size_t bytes_transferred = 0;
	size_t headers_size = 0;
	boost::system::error_code ec;

	while ((bytes_transferred = socket_.read_some(boost::asio::buffer(buffer_), ec)) > 0) {
		boost::tie(result, parsed_size) = response_parser_.parse_headers(response, buffer_.data(), buffer_.data() + bytes_transferred);
		headers_size += parsed_size;
		if (result) {
			// Append whatever is left in the buffer behind the headers to the content
			response.content.append(buffer_.data() + parsed_size, buffer_.data() + bytes_transferred);
			break;
		} else if (!result) {
			return boost::system::error_code(boost::system::errc::illegal_byte_sequence, boost::system::generic_category());
		}
	}
	return ec;
}

boost::system::error_code http_client::read_content(http::client::response& response)
{
	boost::tribool result;
	size_t bytes_transferred = 0;
	boost::system::error_code ec;

	while (response.content.size() < response.headers.content_length() && (bytes_transferred = socket_.read_some(boost::asio::buffer(buffer_), ec)) > 0) {
		if (ec)
			break;
		// Keep reading to the end of the content
		response.content.append(buffer_.data(), buffer_.data() + bytes_transferred);
	}
	return ec;
}


void http_client::async_request(const std::string& method, const std::string& url, const std::string& content)
{
//	schedule_connect();
}

void http_client::schedule_connect()
{
	boost::asio::ip::tcp::resolver::query query(server_, port_);
	resolver_.async_resolve(query, boost::bind(&http::client::http_client::handle_resolve, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator));
}


void http_client::handle_resolve(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
	if (!err) {
		// Attempt a connection to each endpoint in the list until we
		// successfully establish a connection.
		boost::asio::async_connect(socket_, endpoint_iterator, boost::bind(&http_client::handle_connect, this, boost::asio::placeholders::error));
		while (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator()) {
			std::stringstream oss;
			oss << endpoint_iterator->endpoint();
			log_debug_message("Resolved: %s", oss.str().c_str());
			std::cout << "Resolved: " << endpoint_iterator->endpoint() << std::endl;
			endpoint_iterator++;
		}
	} else {
		std::cout << "Error: " << err.message() << "\n";
	}

}

void http_client::handle_connect(const boost::system::error_code& err)
{
	if (!err) {
		// The connection was successful. Send the request.
//		boost::asio::async_write(socket_, request_, boost::bind(&http_client::handle_write_request, this, boost::asio::placeholders::error));
		std::cout << "socket Connected" << std::endl;
	} else {
		std::cout << "Error: " << err.message() << "\n";
	}
}

void http_client::handle_write_request(const boost::system::error_code& err)
{
	if (!err) {
		// Read the response status line. The response_ streambuf will
		// automatically grow to accommodate the entire line. The growth may be
		// limited by passing a maximum size to the streambuf constructor.
//		boost::asio::async_read_until(socket_, response_, "\r\n", boost::bind(&http_client::handle_read_status_line, this, boost::asio::placeholders::error));
	} else {
		std::cout << "Error: " << err.message() << "\n";
	}
}

void http_client::handle_content_read(const boost::system::error_code& e, std::size_t bytes_transferred)
{
	std::cout << std::string(buffer_.data(), buffer_.data() + bytes_transferred);
	schedule_content_read();
}

void http_client::schedule_content_read()
{
	socket_.async_read_some(boost::asio::buffer(buffer_),
		boost::bind(&http_client::handle_content_read,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}


bool http_client::log_debug_message(const char *fmt, ...)
{
	bool result = false;
	va_list args;

	if (!logger_)
		return result;
	va_start(args, fmt);
	std::string format(log_prefix_ + fmt);
	result = logger_->log_debug_message(format.c_str(), args);
	va_end(args);
	return result;
}

bool http_client::log_warning_message(const char *fmt, ...)
{
	bool result = false;
	va_list args;

	if (!logger_)
		return result;
	va_start(args, fmt);
	std::string format(log_prefix_ + fmt);
	result = logger_->log_warning_message(format.c_str(), args);
	va_end(args);
	return result;
}

bool http_client::log_error_message(const char *fmt, ...)
{
	bool result = false;
	va_list args;

	if (!logger_)
		return result;
	va_start(args, fmt);
	std::string format(log_prefix_ + fmt);
	result = logger_->log_error_message(format.c_str(), args);
	va_end(args);
	return result;
}

void http_client::set_logger(http::logger_ptr ptr)
{
	logger_ = ptr;
}

} // namespace server
} // namespace http
