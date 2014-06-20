#include <iostream>
#include "cmdargs.h"
#include "daemon.h"
#include "signals.h"

cmd_args g_args;

static cmd_arg_spec g_argspec[] = {
		{"help",	"h",	"Display this help", CMD_ARG_BOOL},
		{"daemon",	"d",	"Run as daemon", CMD_ARG_BOOL},
		{"service",	"",		"Same as daemon", CMD_ARG_BOOL},
		{"pid-file","",		"PID file location", CMD_ARG_STRING},

};

static cmd_arg_spec g_argspec2[] = {
		{"Kp",	"",			"PID proportional component", CMD_ARG_STRING},
		{"Kd",	"",			"PID differential component", CMD_ARG_STRING},
		{"Ki",	"",			"PID integral component", CMD_ARG_MULTISTRING},
};

void usage(int argc, char *argv[])
{
	std::cout << argv[0] << " <options>" << std::endl;
	std::cout << g_args.get_help_message() << std::endl;
}

int main(int argc, char *argv[])
{
	std::map<std::string, std::vector<std::string> >::iterator it;
	std::vector<std::string> args;
	std::vector<std::string>::const_iterator argit;

	g_args.add_specs(g_argspec, sizeof(g_argspec)/sizeof(*g_argspec));
	g_args.add_specs(g_argspec2, sizeof(g_argspec2)/sizeof(*g_argspec2));
	g_args.parse_command_line(argc, argv);

	if (!g_args.get_value("help").empty()) {
		usage(argc, argv);
		return 0;
	}

	if (g_args.get_value("daemon") == "1") {
		daemon_init();
		signals_init();
	}

	for (it = g_args.args_.begin(); it != g_args.args_.end(); it++) {
		std::cout << it->first << " " << *it->second.begin() << std::endl;
	}
	std::cout << std::endl;

	std::cout << "get_value:" << std::endl;
	std::cout << "daemon " << g_args.get_value("daemon") << std::endl;
	std::cout << "Kd " << g_args.get_value("Kd") << std::endl;
	std::cout << "unknown " << g_args.get_value("unknown") << std::endl;
	std::cout << std::endl;

	args = g_args.get_values("Ki");
	for (argit = args.begin(); argit != args.end(); argit++) {
		std::cout << "Ki " << *argit << std::endl;
	}

	return 0;
}
