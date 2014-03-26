#include <stdio.h>
#include <ctime>

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/convenience.hpp"
#include "logger.h"


logger::logger(const std::string& path, size_t rotsize, message_type level)
	: logfile_(boost::filesystem::system_complete(boost::filesystem::path(path)).string()),
	  rotext_(".0"),
	  rotsize_(rotsize),
	  msgcount_(default_msgcount),
	  level_(level)
{
	init();
}

logger::~logger()
{

}

void logger::init()
{
	strlevels_.push_back("NONE    ");
	strlevels_.push_back("CRITICAL");
	strlevels_.push_back("ERROR   ");
	strlevels_.push_back("WARNING ");
	strlevels_.push_back("INFO    ");
	strlevels_.push_back("DEBUG   ");
}

void logger::set_log_level(message_type level)
{
	level_ = level;
}

std::string logger::get_message_type(message_type level)
{
	if (level >= strlevels_.size())
		return std::string("UNKNOWN ");
	return strlevels_[level];
}

std::string logger::get_time()
{
    char run_date[200];
    std::time_t tod;
    std::time( &tod );
    std::strftime(run_date, sizeof(run_date), "%Y-%m-%d %H:%M:%S", std::localtime( &tod ) );
    return std::string(run_date);
}

bool logger::add_log(message_type type, const char *fmt, ...)
{
	bool result = true;
	va_list args;

	if (type > none && type <= level_) {
		va_start(args, fmt);
		result = add_log_v(type, fmt, args);
		va_end(args);
	}
	return result;
}

bool logger::add_log_v(message_type type, const char *fmt, va_list args)
{
	FILE *pfile = NULL;
	logger_lock lock(m_);
	if ((pfile = fopen(logfile_.c_str(), "a")) == NULL)
		return false;
	fprintf(pfile, "%s; %s; ", get_time().c_str(), get_message_type(type).c_str());
	vfprintf(pfile, fmt, args);
	fprintf(pfile, "\n");
	fclose(pfile);
	if (--msgcount_ <= 0) {
		msgcount_ = default_msgcount;
		rotate();
	}
	return true;
}

std::string logger::get_log_filepath()
{
	return logfile_;
}

void logger::set_log_filepath(const std::string& path)
{
	logfile_ = boost::filesystem::system_complete(boost::filesystem::path(path)).string();
}

void logger::set_max_size(size_t rotsize)
{
	rotsize_ = rotsize;
}

void logger::set_rotation_extension(const std::string& oldext)
{
	rotext_ = oldext;
}

void logger::rotate()
{
	boost::filesystem::path cur_filepath(logfile_);
	boost::filesystem::path old_filepath(logfile_ + rotext_);

	if (boost::filesystem::file_size(cur_filepath) > rotsize_) {
		if (boost::filesystem::exists(old_filepath))
			boost::filesystem::remove(old_filepath);
		boost::filesystem::rename(cur_filepath, old_filepath);
	}
}
