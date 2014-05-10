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
	virtual bool log_critical_message(const char *fmt, va_list args){ return false; }
	virtual bool log_debug_message(const char *fmt, ...)
	{
		bool result = true;
		va_list args;
		va_start(args, fmt);
		result = log_debug_message(fmt, args);
		va_end(args);
		return result;
	}
	virtual bool log_info_message(const char *fmt, ...)
	{
		bool result = true;
		va_list args;
		va_start(args, fmt);
		result = log_info_message(fmt, args);
		va_end(args);
		return result;
	}
	virtual bool log_warning_message(const char *fmt, ...)
	{
		bool result = true;
		va_list args;
		va_start(args, fmt);
		result = log_warning_message(fmt, args);
		va_end(args);
		return result;
	}
	virtual bool log_error_message(const char *fmt, ...)
	{
		bool result = true;
		va_list args;
		va_start(args, fmt);
		result = log_error_message(fmt, args);
		va_end(args);
		return result;
	}
	virtual bool log_critical_message(const char *fmt, ...)
	{
		bool result = true;
		va_list args;
		va_start(args, fmt);
		result = log_critical_message(fmt, args);
		va_end(args);
		return result;
	}
};

typedef boost::shared_ptr<http::logger> logger_ptr;


} // namespace http

#endif
