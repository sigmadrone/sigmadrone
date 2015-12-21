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
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include <pthread.h>
#include <string.h>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "matrix.h"
#include "libcmdargs/cmdargs.h"
#include "libhttp/http_client.hpp"
#include "librexjsonrpc/rpcclienthttp.h"
#include "jsonserialization.h"

static cmd_arg_spec g_argspec[] = {
		{"help",			"h",	"Display this help", CMD_ARG_BOOL},
		{"rpcserver",		"",		"RPC server name", CMD_ARG_STRING},
		{"rpcport",			"",		"RPC server port. Default: 18222", CMD_ARG_STRING},

};



static std::string rpcserver;
static std::string rpcport;
static rpc_client_http* rpcclient = NULL;

int main(int argc, char *argv[])
{
	cmd_args args;

	try {
		args.add_specs(g_argspec, sizeof(g_argspec)/sizeof(*g_argspec));
		args.parse_command_line(argc, (const char**)argv);
		if (!args.get_value("help").empty()) {
			std::cout << argv[0] << " <options>" << std::endl;
			std::cout << args.get_help_message() << std::endl;
			return 0;
		}
		rpcserver = args.get_value("rpcserver");
		rpcport = args.get_value("rpcport", "18222");
	} catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
	if (!rpcserver.empty())
		rpcclient = new rpc_client_http(rpcserver, rpcport, 30000);
		try {
			while (1) {
				rexjson::array arr = rpcclient->call("/firmware", "sd_get_datastream").get_array();
				for (size_t i = 0; i < arr.size(); i++) {
					std::cout << arr[i].write(false) << std::endl;
				}
			}
		} catch (std::exception& e) {
			std::cout << "Error: " << e.what() << std::endl;
		}

	if (rpcclient)
		delete rpcclient;
	return 0;
}

