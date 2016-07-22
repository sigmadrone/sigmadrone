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
#include <cstring>
#include <iostream>
#include <syslog.h>
#include <boost/algorithm/string.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include "libhttp/base64.hpp"
#include "serverapp.h"
#include "daemon.h"

server_app::server_app(const cmd_args& args)
	: logger("server_app: ")
	, logfile_(new logfile("sigmad.log", 1024*1024*10, logfile::none))
	, firmware_uart_speed_(B460800)
	, io_service_()
	, signals_(io_service_)
	, args_(args)
{
	set_log_file(logfile_);
	// Register to handle the signals that indicate when the server should exit.
	// It is safe to register for the same signal multiple times in a program,
	// provided all registration for the specified signal is made through Asio.
	signals_.add(SIGINT);
	signals_.add(SIGTERM);
	signals_.add(SIGHUP);
#if defined(SIGQUIT)
	signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
	signals_.async_wait(boost::bind(&server_app::signal_handler_terminate, this));
}

server_app::~server_app()
{

}

void server_app::signal_handler_terminate()
{
	user_rpcserver_->stop(100);
	io_service_.stop();
	log_info_message("Received terminate signal.");
}

void server_app::stop()
{
	/*
	 * This will cause the signal_handler_terminate
	 * to be called.
	 */
	signals_.cancel();
}


void server_app::init_user_rpcserver()
{
	user_rpcserver_.reset(new user_rpcserver(
			*this,
			io_service_,
			args_.get_value("rpcconnect", "0.0.0.0"),
			args_.get_value("rpcport", "18222")));
	user_rpcserver_->set_log_file(logfile_);
	log_info_message("Starting user rpc server.");

	/*
	 * Use the global log file to log the RPC server messages.
	 */
	user_rpcserver_->set_log_file(logfile_);
}

void server_app::init_black_box()
{
	black_box_.reset(new black_box(*this));
	black_box_->start_recording();
}

int server_app::run(int argc, const char *argv[])
{
	if (args_.get_value("daemon") == "1") {
		// Inform the io_service that we are about to become a daemon. The
		// io_service cleans up any internal resources, such as threads, that may
		// interfere with forking.
		io_service_.notify_fork(boost::asio::io_service::fork_prepare);

		daemon_init();

		// Inform the io_service that we have finished becoming a daemon. The
		// io_service uses this opportunity to create any internal file descriptors
		// that need to be private to the new process.
		io_service_.notify_fork(boost::asio::io_service::fork_child);

		/*
		 * Redirect stdout and stderr to
		 * the log file.
		 */
		dup2(get_log_file()->get_fd(), 1);
		dup2(get_log_file()->get_fd(), 2);
		setlinebuf(stdout);
		setlinebuf(stderr);

		firmware_uart_ = args_.get_value("firmware-uart", "/dev/ttymxc1");
	}
	get_log_file()->log_level(args_.get_value("loglevel", "info"));
	log_info_message("Server starting.");
	init_user_rpcserver();
	init_black_box();
	boost::thread rpc_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
	rpc_thread.join();
	log_info_message("Server stopping.");
	return 0;
}
