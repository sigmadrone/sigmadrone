#ifndef HTTPLOGGER_HPP
#define HTTPLOGGER_HPP

#include <string>
#include <cstdarg>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

namespace http {


class logger
{
public:
	virtual ~logger() { }
	virtual bool log_debug_message(const char *fmt, va_list args) { return false; }
	virtual bool log_info_message(const char *fmt, va_list args) { return false; }
	virtual bool log_warning_message(const char *fmt, va_list args){ return false; }
	virtual bool log_error_message(const char *fmt, va_list args){ return false; }
};

typedef boost::shared_ptr<http::logger> logger_ptr;


} // namespace http

#endif
