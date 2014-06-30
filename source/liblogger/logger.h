#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdarg.h>
#include <string>
#include <boost/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/filesystem.hpp>

template<class LOGFILE>
class logger
{
public:
	logger(const char* prefix, boost::shared_ptr<LOGFILE> logfile = boost::shared_ptr<LOGFILE>()) : prefix_(prefix), logfile_(logfile)
	{
	}

	void set_log_file(boost::shared_ptr<LOGFILE> logfile)
	{
		logfile_ = logfile;
	}

	boost::shared_ptr<LOGFILE> get_log_file()
	{
		return logfile_;
	}

	bool log_debug_message(const char *fmt, va_list args)
	{
		if (!logfile_)
			return false;
		return logfile_->add_log_v(LOGFILE::debug, (prefix_ + fmt).c_str(), args);
	}

	bool log_info_message(const char *fmt, va_list args)
	{
		if (!logfile_)
			return false;
		return logfile_->add_log_v(LOGFILE::info, (prefix_ + fmt).c_str(), args);
	}

	bool log_warning_message(const char *fmt, va_list args)
	{
		if (!logfile_)
			return false;
		return logfile_->add_log_v(LOGFILE::warning, (prefix_ + fmt).c_str(), args);
	}

	bool log_error_message(const char *fmt, va_list args)
	{
		if (!logfile_)
			return false;
		return logfile_->add_log_v(LOGFILE::error, (prefix_ + fmt).c_str(), args);
	}

	bool log_critical_message(const char *fmt, va_list args)
	{
		if (!logfile_)
			return false;
		return logfile_->add_log_v(LOGFILE::critical, (prefix_ + fmt).c_str(), args);
	}

	bool log_debug_message(const char *fmt, ...)
	{
		bool result = true;
		va_list args;
		va_start(args, fmt);
		result = log_debug_message(fmt, args);
		va_end(args);
		return result;
	}

	bool log_info_message(const char *fmt, ...)
	{
		bool result = true;
		va_list args;
		va_start(args, fmt);
		result = log_info_message(fmt, args);
		va_end(args);
		return result;
	}

	bool log_warning_message(const char *fmt, ...)
	{
		bool result = true;
		va_list args;
		va_start(args, fmt);
		result = log_warning_message(fmt, args);
		va_end(args);
		return result;
	}

	bool log_error_message(const char *fmt, ...)
	{
		bool result = true;
		va_list args;
		va_start(args, fmt);
		result = log_error_message(fmt, args);
		va_end(args);
		return result;
	}

	bool log_critical_message(const char *fmt, ...)
	{
		bool result = true;
		va_list args;
		va_start(args, fmt);
		result = log_critical_message(fmt, args);
		va_end(args);
		return result;
	}

protected:
	std::string prefix_;
	boost::shared_ptr<LOGFILE> logfile_;
};

#endif // _LOGGER_H_
