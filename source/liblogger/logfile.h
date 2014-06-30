#ifndef _LOGFILE_H_
#define _LOGFILE_H_

#include <stdarg.h>
#include <string>
#include <boost/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/filesystem.hpp>

class logfile : public boost::enable_shared_from_this<logfile>
{
public:
	typedef enum {none = 0, critical, error, warning, info, debug, unknown} message_type;

	logfile(const std::string& path = "default.log", size_t rotsize = default_rotsize, message_type level = none);
	~logfile();
	bool add_log(message_type type, const char *fmt, ...);
	bool add_log_v(message_type type, const char *fmt, va_list args);
	std::string get_log_filepath();
	void set_log_filepath(const std::string& path);
	void set_max_size(size_t rotsize);
	int log_level(message_type level);
	int log_level(const std::string& level);
	int log_level();
	void set_rotation_extension(const std::string& ext);
	std::string get_message_type(message_type level);
	static const size_t default_rotsize = 1024 * 1024 * 10;		// 10 MB
	static const size_t default_msgcount = 100;					// check log size every default_msgcount messages

private:
	void rotate();
	std::string get_time();
	void init();

private:
	std::string logfile_;
	std::string rotext_;
	size_t rotsize_;
	int msgcount_;
	message_type level_;
	mutable boost::mutex m_;
	std::vector<std::string> strlevels_;
};


#endif /* _LOGFILE_H_ */
