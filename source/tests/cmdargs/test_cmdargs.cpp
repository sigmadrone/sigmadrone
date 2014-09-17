#include <iostream>
#include "cmdargs.h"

/*
 * To test this you can do something like:
 * # ./test_cmdargs -h
 * # ./test_cmdargs --help
 * # ./test_cmdargs --Ki=5.0 --Ki=4.0 --Kd=4.0 --daemon --Kp=3
 */

cmd_args g_args;

/*
 * Cmd args spec:
 * "argument name" "argument alias" "Help message"
 */
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

void usage(int argc, const char *argv[])
{
	std::cout << argv[0] << " <options>" << std::endl;
	std::cout << g_args.get_help_message() << std::endl;
}

int main(int argc, const char *argv[])
{
	std::map<std::string, std::vector<std::string> >::iterator it;
	std::vector<std::string> args;
	std::vector<std::string>::const_iterator argit;

	g_args.add_specs(g_argspec, sizeof(g_argspec)/sizeof(*g_argspec));
	g_args.add_specs(g_argspec2, sizeof(g_argspec2)/sizeof(*g_argspec2));
	try {
		g_args.parse_command_line(argc, argv);
	} catch (std::exception e) {
		cout << "Incoorect command line, got error: " << e.what() << std::endl;
		usage(argc,argv);
		return -1;
	}

	if (!g_args.get_value("help").empty()) {
		usage(argc, argv);
		return 0;
	}

	std::cout << "Testing GetValue:" << std::endl;
	std::cout << "daemon " << g_args.get_value("daemon") << std::endl;
	std::cout << "Kd = " << g_args.get_value("Kd") << std::endl;
	std::cout << std::endl;

	/*
	 * Get multiple values argument.
	 */
	std::cout << "Testing GetValues:" << std::endl;
	args = g_args.get_values("Ki");
	for (argit = args.begin(); argit != args.end(); argit++) {
		std::cout << "Ki " << *argit << std::endl;
	}

	return 0;
}
