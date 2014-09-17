#ifndef SERVERAPP_H_
#define SERVERAPP_H_

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include "libcmdargs/cmdargs.h"
#include "liblogger/logfile.h"
#include "liblogger/logger.h"
#include "libhttp/logger.hpp"
#include "userrpcserver.h"


class server_app : private boost::noncopyable, public logger<logfile>
{
public:
	server_app(const cmd_args& args = cmd_args());
	~server_app();
	int run(int argc, const char *argv[]);
	void stop();

public:
	boost::scoped_ptr<user_rpcserver> user_rpcserver_;

protected:
	void signal_handler_terminate();
	void usage();
	void init_user_rpcserver();

	boost::asio::io_service io_service_;
	boost::asio::signal_set signals_;
	const cmd_args& args_;
};

#endif
