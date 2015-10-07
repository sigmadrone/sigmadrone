/*
 *  Sigmadrone
 *  Copyright (c) 2013-2015 The Sigmadrone Developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Martin Stoilov <martin@sigmadrone.org>
 *  Svetoslav Vassilev <svassilev@sigmadrone.org>
 */
#ifndef _LOGFILE_H_
#define _LOGFILE_H_

#include <stdarg.h>
#include <string>
#include <boost/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/filesystem.hpp>

class logfile_base : public boost::enable_shared_from_this<logfile_base>
{
public:
	typedef enum {none = 0, critical, error, warning, info, debug, unknown} message_type;

	logfile_base() {}
	virtual ~logfile_base() {}
	virtual bool add_log(message_type type, const char *fmt, ...) { return false; }
	virtual bool add_log_v(message_type type, const char *fmt, va_list args) { return false; };
	virtual std::string get_log_filepath() { return "none"; }
	virtual void set_log_filepath(const std::string& path) {}
	virtual void set_max_size(size_t rotsize) {}
	virtual int log_level(message_type level) { return 0; }
	virtual int log_level(const std::string& level) { return 0; }
	virtual int log_level() { return 0; }
	virtual int get_fd() { return -1; }
	virtual void set_rotation_extension(const std::string& ext) { }
	virtual std::string get_message_type(message_type level) { return "none"; }
	virtual void enable_disable_date_prefix(bool enable) {}
	static const size_t default_rotsize = 1024 * 1024 * 10;		// 10 MB
	static const size_t default_msgcount = 100;					// check log size every default_msgcount messages
};

class logfile : public logfile_base
{
public:
	logfile(const std::string& path = "default.log",
			size_t rotsize = default_rotsize,
			message_type level = none,
			bool log_date_time_prefix = true);
	virtual ~logfile();
	virtual bool add_log(message_type type, const char *fmt, ...);
	virtual bool add_log_v(message_type type, const char *fmt, va_list args);
	virtual std::string get_log_filepath();
	virtual void set_log_filepath(const std::string& path);
	virtual void set_max_size(size_t rotsize);
	virtual int log_level(message_type level);
	virtual int log_level(const std::string& level);
	virtual int log_level();
	virtual int get_fd();
	virtual void set_rotation_extension(const std::string& ext);
	virtual std::string get_message_type(message_type level);
	virtual void enable_disable_date_prefix(bool enable);

private:
	void rotate();
	std::string get_time();
	void init();

private:
	FILE *pfile_;
	std::string logfile_;
	std::string rotext_;
	size_t rotsize_;
	int msgcount_;
	message_type level_;
	mutable boost::mutex m_;
	std::vector<std::string> strlevels_;
	bool log_date_time_;
};

#endif /* _LOGFILE_H_ */
