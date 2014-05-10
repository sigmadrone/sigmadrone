#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdarg.h>
#include <string>
#include <boost/filesystem.hpp>
#include "sync.h"
#include "http/logger.hpp"

typedef sdrone::lock logger_lock;
typedef sdrone::mutex logger_mutex;



class logger: public http::logger
{
public:
	typedef enum {none = 0, critical, error, warning, info, debug, unknown} message_type;

	logger(const std::string& path = "default.log", size_t rotsize = default_rotsize, message_type level = none);
	~logger();
	bool add_log(message_type type, const char *fmt, ...);
	bool add_log_v(message_type type, const char *fmt, va_list args);
	std::string get_log_filepath();
	void set_log_filepath(const std::string& path);
	void set_max_size(size_t rotsize);
	void set_log_level(message_type level);
	void set_rotation_extension(const std::string& ext);
	std::string get_message_type(message_type level);
	static const size_t default_rotsize = 1024 * 1024 * 10;		// 10 MB
	static const size_t default_msgcount = 100;					// check log size every default_msgcount messages

private:
	void rotate();
	std::string get_time();
	void init();

	bool log_debug_message(const char *fmt, va_list args) {
		return add_log_v(debug,fmt,args);
	}
	bool log_warning_message(const char *fmt, va_list args) {
		return add_log_v(warning,fmt,args);
	}
	bool log_error_message(const char *fmt, va_list args) {
		return add_log_v(error,fmt,args);
	}

private:
	std::string logfile_;
	std::string rotext_;
	size_t rotsize_;
	int msgcount_;
	message_type level_;
	logger_mutex m_;
	std::vector<std::string> strlevels_;
};



#endif /* _LOGGER_H_ */
