#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "http_client.hpp"

namespace http {
namespace client {


http_client::http_client(const std::string& server, const std::string& port, size_t timeout)
	: exit_(false)
	,io_timeout_(timeout)
	, bytes_transferred_(0)
	, server_(server)
	, port_(port)
	, log_prefix_(std::string("HTTP::CLIENT") + "; ")
	, logger_()
	, io_service_()
	, resolver_(io_service_)
	, socket_(io_service_)
	, buffer_()
	, endpoint_()
	, timer_(io_service_)
{
	log_prefix_ = std::string("HTTP::CLIENT") + "@" + server_ + ":" + port_ + "; ";

}

http_client::~http_client()
{
	disconnect();
}

void http_client::handle_async_resolve_timeout(const boost::system::error_code& ec)
{
	if ( ec == boost::asio::error::operation_aborted) {
		/*
		 * The timeout timer was cancelled. Do nothing
		 */
		return;
	}
	log_debug_message("  --->http_client::handle_async_resolve_timeout (timer fired), error:  %s, code: %d", ec.message().c_str(), ec.value());
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

	if (io_timeout_) {
		timer_.expires_from_now(boost::posix_time::milliseconds(io_timeout_));
		timer_.async_wait(boost::bind(&http_client::handle_async_resolve_timeout, this, boost::asio::placeholders::error));
	}
	resolver_.async_resolve(
			query,
			boost::bind(&http_client::handle_async_resolve,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::iterator));
	if (io_timeout_) {
		ioservice_run_one();
	}
	ioservice_run_one();
	if (ec_) {
		log_debug_message("Failed to resolve: %s:%s", server_.c_str(), port_.c_str());
	}
	boost::asio::detail::throw_error(ec_, "http_client::async_resolve");
}

void http_client::stop()
{
	exit_ = true;
	io_service_.stop();
}

void http_client::ioservice_run_one()
{
	io_service_.run_one();
	if (exit_) {
		io_service_.reset();
		boost::asio::detail::throw_error(boost::system::error_code(boost::system::errc::interrupted, boost::system::generic_category()));
	}
	io_service_.reset();
}

void http_client::handle_async_connect_timeout(const boost::system::error_code& ec)
{
	if ( ec == boost::asio::error::operation_aborted) {
		/*
		 * The timeout timer was cancelled. Do nothing
		 */
		return;
	}
	log_debug_message("  --->http_client::handle_async_bytestransferred_timeout (timer fired), error:  %s, code: %d", ec.message().c_str(), ec.value());
	disconnect();
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
		if (io_timeout_) {
			timer_.expires_from_now(boost::posix_time::milliseconds(io_timeout_));
			timer_.async_wait(boost::bind(&http_client::handle_async_connect_timeout, this, boost::asio::placeholders::error));
		}
		boost::asio::async_connect(socket_, it, connect_condition(*this),
				boost::bind(&http_client::handle_async_connect,
							this,
							boost::asio::placeholders::error));
		if (io_timeout_) {
			ioservice_run_one();
		}
		ioservice_run_one();
		if (!ec_) {
			log_debug_message("Connected to: %s:%d", it->endpoint().address().to_string().c_str(), it->endpoint().port());
			break;
		}
		log_error_message("Failed to connect: %s:%d", it->endpoint().address().to_string().c_str(), it->endpoint().port());
		disconnect();
		++it;
	}
	boost::asio::detail::throw_error(ec_, "http_client::async_connect_endpoint");
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

	exit_ = false;
	if (!socket_.is_open()) {
		disconnect();
		async_connect_endpoint();
	}

	try {
		send_request(response, request_buffers);
		return;
	} catch (boost::system::system_error& e) {
		boost::system::error_code ec = e.code();
		if (ec != boost::asio::error::eof && ec != boost::asio::error::bad_descriptor && ec != boost::asio::error::broken_pipe)
			throw;
		disconnect();
		async_connect_endpoint();
	}
	try {
		send_request(response, request_buffers);
		return;
	} catch (boost::system::system_error& e) {
		disconnect();
		response.content.clear();
		response.headers.clear();
		response.status = 0;
		throw;
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
	if (response.headers.header("Connection") != "keep-alive")
		disconnect();
}

void http_client::handle_async_bytestransferred_timeout(const boost::system::error_code& ec)
{
	if ( ec == boost::asio::error::operation_aborted) {
		/*
		 * The timeout timer was cancelled. Do nothing
		 */
		return;
	}
	ec_ = boost::asio::error::timed_out;
}

void http_client::handle_async_bytestransferred(const boost::system::error_code& ec, std::size_t bytes_transferred)
{
	boost::system::error_code ignored;
	timer_.cancel(ignored);
	ec_ = ec;
	bytes_transferred_ = (!ec) ? bytes_transferred : 0UL;
}

void http_client::async_write_request_buffers(const std::vector<boost::asio::const_buffer>& request_buffers)
{
	if (io_timeout_) {
		timer_.expires_from_now(boost::posix_time::milliseconds(io_timeout_));
		timer_.async_wait(boost::bind(&http_client::handle_async_bytestransferred_timeout, this, boost::asio::placeholders::error));
	}
	boost::asio::async_write(socket_, request_buffers,
			boost::bind(&http_client::handle_async_bytestransferred,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
	if (io_timeout_) {
		ioservice_run_one();
	}
	ioservice_run_one();
	boost::asio::detail::throw_error(ec_, "http_client::async_write_request_buffers");
}

size_t http_client::async_read_data()
{
	if (io_timeout_) {
		timer_.expires_from_now(boost::posix_time::milliseconds(io_timeout_));
		timer_.async_wait(boost::bind(&http_client::handle_async_bytestransferred_timeout, this, boost::asio::placeholders::error));
	}

	boost::asio::async_read(socket_, boost::asio::buffer(buffer_), boost::asio::transfer_at_least(1),
			boost::bind(&http_client::handle_async_bytestransferred,
									this,
									boost::asio::placeholders::error,
									boost::asio::placeholders::bytes_transferred));
	if (io_timeout_) {
		ioservice_run_one();
	}
	ioservice_run_one();
	if (ec_ != boost::asio::error::eof)
		boost::asio::detail::throw_error(ec_, "http_client::async_read_data");
	return bytes_transferred_;
}

void http_client::read_headers(http::client::response& response)
{
	boost::tribool result;
	size_t parsed_size = 0;
	size_t bytes_transferred = 0;
	size_t headers_size = 0;

	while ((bytes_transferred = async_read_data()) > 0) {
		boost::tie(result, parsed_size) = response_parser_.parse_headers(response, buffer_.data(), buffer_.data() + bytes_transferred);
		headers_size += parsed_size;
		if (result) {
			// Append whatever is left in the buffer behind the headers to the content
			response.content.append(buffer_.data() + parsed_size, buffer_.data() + bytes_transferred);
			break;
		} else if (!result) {
			boost::asio::detail::throw_error(boost::system::error_code(boost::system::errc::illegal_byte_sequence, boost::system::generic_category()));
		}
	}
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
			// Keep reading to the end of the content
			response.content.append(buffer_.data(), buffer_.data() + bytes_transferred);
		}
	} else {
		while ((response.content.size() < response.headers.content_length())
				&& (bytes_transferred = async_read_data()) > 0) {
			// Keep reading to the end of the content
			response.content.append(buffer_.data(), buffer_.data() + bytes_transferred);
		}
	}
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

void http_client::timeout(size_t millisec)
{
	io_timeout_ = millisec;
}

size_t http_client::timeout()
{
	return io_timeout_;
}


} // namespace server
} // namespace http
