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
#include <iostream>
#include <syslog.h>
#include <unistd.h>
#include "cmdargs.h"
#include "daemon.h"
#include "signals.h"
#include "clientapp.h"
#include "serverapp.h"


static cmd_arg_spec g_argspec[] = {
		{"help",			"h",	"Display this help", CMD_ARG_BOOL},
		{"daemon",			"d",	"Run as daemon", CMD_ARG_BOOL},
		{"service",			"",		"Same as daemon", CMD_ARG_BOOL},
		{"pid-file",		"",		"PID file location", CMD_ARG_STRING},
		{"loglevel",		"",		"Valid loglevels: none, critical, error, info, debug. Default: info", CMD_ARG_STRING},
		{"rpc-server",		"",		"Remote RPC server. Default: localhost", CMD_ARG_STRING},
		{"rpc-port",		"",		"Remote RPC port. Default: 18222", CMD_ARG_STRING},
		{"rpc-uri",			"",		"Remote RPC uri. Default: /jsonrpc", CMD_ARG_STRING},
		{"firmware",		"f",	"Use firmware uri for json rpc calls. This is the same as setting --rpc-uri=/firmware", CMD_ARG_BOOL},
};

static cmd_arg_spec g_argspec2[] = {
		{"Kp",				"",		"PID proportional component", CMD_ARG_STRING},
		{"Kd",				"",		"PID differential component", CMD_ARG_STRING},
		{"Ki",				"",		"PID integral component", CMD_ARG_MULTISTRING},
		{"firmware-uart",	"",		"Firmware uart device. Default: /dev/ttymxc0", CMD_ARG_STRING},
};

int main(int argc, const char *argv[])
{
	cmd_args args;

	try {
		args.add_specs(g_argspec, sizeof(g_argspec)/sizeof(*g_argspec));
		args.add_specs(g_argspec2, sizeof(g_argspec2)/sizeof(*g_argspec2));
		args.parse_command_line(argc, argv);
		if (!args.get_value("help").empty()) {
			std::cout << argv[0] << " <options>" << std::endl;
			std::cout << args.get_help_message() << std::endl;
		} else if (client_app::is_client(argc, argv)) {
			client_app cli(args);
			return cli.run(argc, argv);
		} else {
			server_app server(args);
			return server.run(argc, argv);
		}
	} catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}
