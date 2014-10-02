#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "http_client.hpp"

namespace http {
namespace client {


http_client::http_client(const std::string& server, const std::string& port, size_t timeout)
	: logger("HTTP Client: ")
	, stopped_(false)
	, io_timeout_(timeout)
	, bytes_transferred_(0)
	, server_(server)
	, port_(port)
	, io_service_()
	, buffer_()
	, endpoint_()
	, timer_(io_service_)
	, resolver_(io_service_)
	, socket_(io_service_)
{

}

http_client::~http_client()
{
}

void http_client::stop()
{
	stopped_ = true;
	socket_.close();
	resolver_.cancel();
	io_service_.stop();
}

void http_client::ioservice_run()
{
	if (stopped_)
		boost::asio::detail::throw_error(boost::asio::error::interrupted, "http_client::ioservice_run");
	io_service_.run();
	io_service_.reset();
	if (stopped_)
		boost::asio::detail::throw_error(boost::asio::error::interrupted, "http_client::ioservice_run");
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

	stopped_ = false;
	if (socket_.is_open()) {
		try {
			send_request(response, request_buffers);
			if (!boost::iequals(response.headers.header("Connection"), "keep-alive"))
				socket_.close();
			return;
		} catch (boost::system::system_error& e) {
			boost::system::error_code ec = e.code();
			if (ec != boost::asio::error::eof && ec != boost::asio::error::bad_descriptor && ec != boost::asio::error::broken_pipe) {
				socket_.close();
				response.content.clear();
				response.headers.clear();
				response.status = 0;
				throw;
			}
		}
	}
	try {
		socket_.close();
		async_connect_endpoint();
		send_request(response, request_buffers);
		if (!boost::iequals(response.headers.header("Connection"), "keep-alive"))
			socket_.close();
	} catch (boost::system::system_error& e) {
		boost::system::error_code ec = e.code();
		if (ec) {
			socket_.close();
			response.content.clear();
			response.headers.clear();
			response.status = 0;
			throw;
		}
	}
}

void http_client::send_request(
		http::client::response& response,
		const std::vector<boost::asio::const_buffer>& request_buffers)
{
	response_parser_.reset();
	response.headers.clear();
	response.content.clear();
	response.status = 0;
	async_write_request_buffers(request_buffers);
	read_headers(response);
	read_content(response);
	log_debug_message("<===================\n%s", (debug_headers_ + response.content).c_str());
}


void http_client::handle_async_resolve_timeout(const boost::system::error_code& ec)
{
	log_debug_message("  --->http_client::handle_async_resolve_timeout, error:  %s, code: %d", ec.message().c_str(), ec.value());
	if ( ec == boost::asio::error::operation_aborted) {
		/*
		 * The timeout timer was cancelled. Do nothing
		 */
		return;
	}
	resolver_.cancel();
}

void http_client::handle_async_resolve(const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
	log_debug_message("  --->http_client::handle_async_resolve, error:  %s, code: %d", ec.message().c_str(), ec.value());
	boost::system::error_code ignored;
	timer_.cancel(ignored);
	endpoint_ = (!ec) ? endpoint_iterator : boost::asio::ip::tcp::resolver::iterator();
	ec_ = ec;
}

void http_client::async_resolve()
{
	boost::asio::ip::tcp::resolver::query query(server_, port_);
	log_debug_message("http_client::async_resolve, resolving:  %s, port: %s", server_.c_str(), port_.c_str());

	ec_.clear();
	if (io_timeout_) {
		timer_.expires_from_now(boost::posix_time::milliseconds(io_timeout_));
		timer_.async_wait(boost::bind(&http_client::handle_async_resolve_timeout,
				this,
				boost::asio::placeholders::error));
	}
	resolver_.async_resolve(
			query,
			boost::bind(&http_client::handle_async_resolve,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::iterator));
	ioservice_run();
	if (ec_) {
		log_debug_message("Failed to resolve: %s:%s", server_.c_str(), port_.c_str());
	}
	boost::asio::detail::throw_error(ec_, "http_client::async_resolve");
}

void http_client::handle_async_connect_timeout(const boost::system::error_code& ec)
{
	log_debug_message("  --->http_client::handle_async_connect_timeout, error:  %s, code: %d", ec.message().c_str(), ec.value());
	if ( ec == boost::asio::error::operation_aborted) {
		/*
		 * The timeout timer was cancelled. Do nothing
		 */
		return;
	}
	socket_.close();
}

void http_client::handle_async_connect(const boost::system::error_code& ec)
{
	log_debug_message("  --->http_client::handle_async_connect, error:  %s, code: %d", ec.message().c_str(), ec.value());
	boost::system::error_code ignored;
	timer_.cancel(ignored);
	ec_ = ec;
}

void http_client::async_connect_endpoint()
{
	boost::asio::ip::tcp::resolver::iterator end;
	if (endpoint_ == end) {
		async_resolve();
	}
	boost::asio::ip::tcp::resolver::iterator it = endpoint_;
	while (it != end) {
		log_debug_message("http_client::async_connect_endpoint, connecting:  %s, port: %d", it->endpoint().address().to_string().c_str(), it->endpoint().port());
		ec_.clear();
		if (io_timeout_) {
			timer_.expires_from_now(boost::posix_time::milliseconds(io_timeout_));
			timer_.async_wait(boost::bind(&http_client::handle_async_connect_timeout, this, boost::asio::placeholders::error));
		}
		boost::asio::async_connect(socket_, it,
				boost::bind(&http_client::handle_async_connect,
							this,
							boost::asio::placeholders::error));
		ioservice_run();
		if (!ec_) {
			log_debug_message("Connected to: %s:%s", get_remote_address().c_str(), get_remote_port().c_str());
			break;
		}
		log_error_message("Failed to connect: %s:%d, error: %s", it->endpoint().address().to_string().c_str(), it->endpoint().port(), ec_.message().c_str());
		++it;
	}
	boost::asio::detail::throw_error(ec_, "http_client::async_connect_endpoint");
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
		if (!it->second.empty()) {
			buffers.push_back(boost::asio::buffer(it->first));
			buffers.push_back(boost::asio::buffer(misc_strings::name_value_separator));
			buffers.push_back(boost::asio::buffer(it->second));
			buffers.push_back(boost::asio::buffer(misc_strings::crlf));
		}
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

void http_client::handle_async_bytestransferred_timeout(const boost::system::error_code& ec)
{
	log_debug_message("  --->http_client::handle_async_bytestransferred_timeout, error:  %s, code: %d", ec.message().c_str(), ec.value());
	if ( ec == boost::asio::error::operation_aborted) {
		/*
		 * The timeout timer was cancelled. Do nothing
		 */
		return;
	}
	socket_.close();
}

void http_client::handle_async_read_bytestransferred(const boost::system::error_code& ec, std::size_t bytes_transferred)
{
	log_debug_message("  --->http_client::handle_async_read_bytestransferred, error:  %s, bytes_transferred: %lu", ec.message().c_str(), bytes_transferred);
	boost::system::error_code ignored;
	timer_.cancel(ignored);
	ec_ = ec;
	bytes_transferred_ = bytes_transferred;
}

void http_client::handle_async_write_bytestransferred(const boost::system::error_code& ec, std::size_t bytes_transferred)
{
	log_debug_message("  --->http_client::handle_async_write_bytestransferred, error:  %s, bytes_transferred: %lu", ec.message().c_str(), bytes_transferred);
	boost::system::error_code ignored;
	timer_.cancel(ignored);
	ec_ = ec;
	bytes_transferred_ = bytes_transferred;
}


void http_client::async_write_request_buffers(const std::vector<boost::asio::const_buffer>& request_buffers)
{
	if (io_timeout_) {
		timer_.expires_from_now(boost::posix_time::milliseconds(io_timeout_));
		timer_.async_wait(boost::bind(&http_client::handle_async_bytestransferred_timeout, this, boost::asio::placeholders::error));
	}
	boost::asio::async_write(socket_, request_buffers,
			boost::bind(&http_client::handle_async_write_bytestransferred,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
	ioservice_run();
	boost::asio::detail::throw_error(ec_, "http_client::async_write_request_buffers");
}

size_t http_client::async_read_data()
{
	if (io_timeout_) {
		timer_.expires_from_now(boost::posix_time::milliseconds(io_timeout_));
		timer_.async_wait(boost::bind(&http_client::handle_async_bytestransferred_timeout, this, boost::asio::placeholders::error));
	}

	boost::asio::async_read(socket_, boost::asio::buffer(buffer_),
							boost::asio::transfer_at_least(1),
							boost::bind(&http_client::handle_async_read_bytestransferred,
									this,
									boost::asio::placeholders::error,
									boost::asio::placeholders::bytes_transferred));
	ioservice_run();
	return bytes_transferred_;
}

void http_client::read_headers(http::client::response& response)
{
	boost::tribool result;
	size_t parsed_size = 0;
	size_t bytes_transferred = 0;
	size_t headers_size = 0;

	debug_headers_.clear();
	while ((bytes_transferred = async_read_data()) > 0) {
		boost::tie(result, parsed_size) = response_parser_.parse_headers(response, buffer_.data(), buffer_.data() + bytes_transferred);
		headers_size += parsed_size;
		debug_headers_ += std::string(buffer_.data(), parsed_size);
		if (result) {
			// Append whatever is left in the buffer behind the headers to the content
			response.content.append(buffer_.data() + parsed_size, buffer_.data() + bytes_transferred);
			break;
		} else if (!result) {
			boost::asio::detail::throw_error(boost::system::error_code(boost::system::errc::illegal_byte_sequence, boost::system::generic_category()));
		}
	}
	boost::asio::detail::throw_error(ec_, "http_client::read_headers");
}

void http_client::read_content(http::client::response& response)
{
	boost::tribool result;
	size_t bytes_transferred = 0;

	if (response.headers.header("Content-Length").empty()) {
		/*
		 * There is no Content-Length header, so we will read until we get EOF (read 0 bytes)
		 */
		while ((bytes_transferred = async_read_data()) > 0) {
			// Keep reading until we receive eof (read 0 bytes).
			response.content.append(buffer_.data(), buffer_.data() + bytes_transferred);
		}
		if (ec_ != boost::asio::error::eof)
			boost::asio::detail::throw_error(ec_, "http_client::read_content");
	} else {
		while ((response.content.size() < response.headers.content_length())
				&& (bytes_transferred = async_read_data()) > 0) {
			// Keep reading until we read the entire response.headers.content_length() number of bytes.
			response.content.append(buffer_.data(), buffer_.data() + bytes_transferred);
		}
		boost::asio::detail::throw_error(ec_, "http_client::read_content");
	}
}

void http_client::timeout(size_t millisec)
{
	io_timeout_ = millisec;
}

size_t http_client::timeout()
{
	return io_timeout_;
}

std::string http_client::get_local_address()
{
	std::string address;
	try {
		address = socket_.local_endpoint().address().to_string();
	} catch (boost::system::system_error& e) {
		address.clear();
	}
	return address;
}

std::string http_client::get_local_port()
{
	std::string port;
	try {
		unsigned int local_port = socket_.local_endpoint().port();
		port = boost::lexical_cast<std::string>(local_port);
	} catch (boost::system::system_error& e) {
		port.clear();
	}
	return port;
}


std::string http_client::get_remote_address()
{
	std::string address;
	try {
		address = socket_.remote_endpoint().address().to_string();
	} catch (boost::system::system_error& e) {
		address.clear();
	}
	return address;
}

std::string http_client::get_remote_port()
{
	std::string port;
	try {
		unsigned int remote_port = socket_.remote_endpoint().port();
		port = boost::lexical_cast<std::string>(remote_port);
	} catch (boost::system::system_error& e) {
		port.clear();
	}
	return port;
}

std::string http_client::address()
{
	return server_;
}

std::string http_client::port()
{
	return port_;
}


} // namespace server
} // namespace http
