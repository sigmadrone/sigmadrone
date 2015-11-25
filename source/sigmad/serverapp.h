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
#include "librexjsonrpc/rpcclientuart.h"

#include "userrpcserver.h"
#include "blackbox.h"


class server_app : private boost::noncopyable, public logger<logfile>
{
public:
	server_app(const cmd_args& args = cmd_args());
	~server_app();
	int run(int argc, const char *argv[]);
	void stop();

public:
	/*
	 * Log file shared among all logger derived classes,
	 * so they all log in the same file.
	 */
	boost::shared_ptr<logfile> logfile_;
	boost::scoped_ptr<user_rpcserver> user_rpcserver_;
	boost::scoped_ptr<black_box> black_box_;
	std::string firmware_uart_;
	uint32_t firmware_uart_speed_;

protected:
	void signal_handler_terminate();
	void usage();
	void init_user_rpcserver();
	void init_black_box();

	boost::asio::io_service io_service_;
	boost::asio::signal_set signals_;
	const cmd_args& args_;
};

#endif
