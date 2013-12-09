#include <iostream>
#include "cmdargs.h"

/*
 * To test this you can do something like:
 * # ./test_cmdargs -h
 * # ./test_cmdargs --help
 * # ./test_cmdargs --Ki=5.0 --Ki=4.0 --Kd=4.0 --daemon --Kp=3
 */

CmdArgs g_args;

/*
 * Cmd args spec:
 * "argument name" "argument alias" "Help message"
 */
static CmdArgSpec g_argspec[] = {
		{"help",	"h",	"Display this help", CMD_ARG_BOOL},
		{"daemon",	"d",	"Run as daemon", CMD_ARG_BOOL},
		{"service",	"",		"Same as daemon", CMD_ARG_BOOL},
		{"pid-file","",		"PID file location", CMD_ARG_STRING},

};

static CmdArgSpec g_argspec2[] = {
		{"Kp",	"",			"PID proportional component", CMD_ARG_STRING},
		{"Kd",	"",			"PID differential component", CMD_ARG_STRING},
		{"Ki",	"",			"PID integral component", CMD_ARG_MULTISTRING},
};

void usage(int argc, char *argv[])
{
	std::cout << argv[0] << " <options>" << std::endl;
	std::cout << g_args.HelpMessage() << std::endl;
}

int main(int argc, char *argv[])
{
	std::map<std::string, std::vector<std::string> >::iterator it;
	std::vector<std::string> args;
	std::vector<std::string>::const_iterator argit;

	g_args.AddSpecs(g_argspec, sizeof(g_argspec)/sizeof(*g_argspec));
	g_args.AddSpecs(g_argspec2, sizeof(g_argspec2)/sizeof(*g_argspec2));
	g_args.Parse(argc, argv);

	if (!g_args.GetValue("help").empty()) {
		usage(argc, argv);
		return 0;
	}

	for (it = g_args.m_args.begin(); it != g_args.m_args.end(); it++) {
		std::cout << it->first << " " << *it->second.begin() << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Testing GetValue:" << std::endl;
	std::cout << "daemon " << g_args.GetValue("daemon") << std::endl;
	std::cout << "Kd = " << g_args.GetValue("Kd") << std::endl;
	std::cout << std::endl;

	/*
	 * Get multiple values argument.
	 */
	std::cout << "Testing GetValues:" << std::endl;
	args = g_args.GetValues("Ki");
	for (argit = args.begin(); argit != args.end(); argit++) {
		std::cout << "Ki " << *argit << std::endl;
	}

	return 0;
}
