#include <iostream>
#include "cmdargs.h"
#include "daemon.h"
#include "signals.h"
#include "clientapp.h"
#include "serverapp.h"


static cmd_arg_spec g_argspec[] = {
		{"help",	"h",	"Display this help", CMD_ARG_BOOL},
		{"daemon",	"d",	"Run as daemon", CMD_ARG_BOOL},
		{"service",	"",		"Same as daemon", CMD_ARG_BOOL},
		{"pid-file","",		"PID file location", CMD_ARG_STRING},
		{"loglevel","",		"Valid loglevels: none, critical, error, info, debug. Default: info", CMD_ARG_STRING},
};

static cmd_arg_spec g_argspec2[] = {
		{"Kp",		"",		"PID proportional component", CMD_ARG_STRING},
		{"Kd",		"",		"PID differential component", CMD_ARG_STRING},
		{"Ki",		"",		"PID integral component", CMD_ARG_MULTISTRING},
};

int main(int argc, char *argv[])
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

