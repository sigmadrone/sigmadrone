/*
 * _CommandArgs.cpp
 *
 *  Created on: Sep 8, 2013
 *      Author: svassilev
 */

#include "commandargs.h"
#include <stdexcept>
#include "jsonrpcparser.h"

static cmd_arg_spec s_argSpec[] = {
		{"help",		"h", "Display this help", CMD_ARG_BOOL},
		{"command",		"c", "reset|exit|run|ping|thrust|config|getconfig|getthrust", CMD_ARG_STRING},
		{"server",		"s", "Run as server and control drone hardware", CMD_ARG_BOOL},
		{"daemon",		"d", "Run as daemon, note: server must be specified", CMD_ARG_BOOL},
		{"rot-matrix",	"",	 "Print rotational matrix", CMD_ARG_BOOL},
		{"host",        "",	 "Server IP address; client role assumed  when arg present", CMD_ARG_STRING},
		{"port",        "",	 "Server port, default port is 2222", CMD_ARG_STRING},
		{"maxdps",      "",	 "Gyro max angular speed in deg/s", CMD_ARG_STRING},
		{"rate",        "",	 "Gyro and accelerometer sampling rate", CMD_ARG_STRING},
		{"thrust",      "",	 "Set desired thrust per motor [0..1], used with --command=run", CMD_ARG_STRING},
		{"maxthrust",   "",	 "Set min thrust per motor [0..1], used with --command=run", CMD_ARG_STRING},
		{"minthrust",   "",	 "Set max thrust per motor [0..1], used with --command=run", CMD_ARG_STRING},
		{"lograte",     "",	 "How many times to log per second", CMD_ARG_STRING},
		{"logfile",     "",	 "Capture daemon output to file", CMD_ARG_STRING},
		{"infile",      "",	 "Input file for sensor readings (testing only)", CMD_ARG_STRING},
		{"outfile",     "",	 "Output file for servo commands (testing only)", CMD_ARG_STRING},
		{"imuangle",    "",	 "Yaw angle between sensor's and motor's axis", CMD_ARG_STRING},
		{"Kp",          "",	 "PID controller proportional coefficient", CMD_ARG_STRING},
		{"Ki",          "",	 "PID controller integral coefficient", CMD_ARG_STRING},
		{"Kd",          "",	 "PID controller derivative coefficient", CMD_ARG_STRING},
		{"configfile",  "",  "Configuration file name", CMD_ARG_STRING},
};


CommandLineArgs::CommandLineArgs(int argc, char* argv[])
{
	m_cmdArgs.add_specs(s_argSpec,sizeof(s_argSpec)/sizeof(s_argSpec[0]));
	try {
		m_cmdArgs.parse_command_line(argc, argv);
	} catch (std::out_of_range exc) {
		printf("%s\n",exc.what());
	}
}

CommandLineArgs::~CommandLineArgs() {}


void CommandLineArgs::PrintUsage(int argc, char* argv[]) const
{
	printf("Sigma Drone 0.1\n\n");
	printf("Usage: %s <options>\n", argc>0 ? argv[0] : "sdroned");
	printf("%s\n",m_cmdArgs.get_help_message().c_str());

	printf( "Additional notes:\n"
			"Server process can be started just by specifying --server; if command is \n"
			"specified in conjunction with --server then the process will carry out the\n"
			"command and will wait for further instructions. --server or --host must be\n"
			"specified. If specified, --configfile must contain a valid json schema. If config\n"
			"file is passed on the daemon command line, then the config file will be parsed,\n"
			"together with any additional command line arguments; the command line arguments\n"
			"will take precedence over the values in the config file.\n\n");
}

SdCommandCode CommandLineArgs::GetCommand() const
{
	SdCommandCode command = SD_COMMAND_NONE;
	std::string commandAsStr = m_cmdArgs.get_value("command").c_str();
	if (commandAsStr == std::string("run")) {
		command = SD_COMMAND_RUN;
	} else if (commandAsStr == std::string("reset")) {
		command = SD_COMMAND_RESET;
	} else if (commandAsStr == std::string("exit")) {
		command = SD_COMMAND_EXIT;
	} else if (commandAsStr  == std::string("ping")) {
		command = SD_COMMAND_PING;
	} else if (commandAsStr == std::string("getconfig")) {
		command = SD_COMMAND_GET_CONFIG;
	} else if (commandAsStr == std::string("getthrust")) {
		command = SD_COMMAND_GET_THRUST;
	} else if (commandAsStr == std::string("config")) {
		command = SD_COMMAND_SET_CONFIG;
	} else if (commandAsStr == std::string("thrust")) {
		command = SD_COMMAND_SET_THRUST;
	}
	return command;
}

double CommandLineArgs::GetMinThrust() const {
	double thrust;
	return GetValueAsDouble("minthrust",&thrust) ? thrust : SdThrustValues::MIN;
}

double CommandLineArgs::GetMaxThrust() const {
	double thrust;
	return GetValueAsDouble("maxthrust",&thrust) ? thrust : SdThrustValues::MIN;
}

double CommandLineArgs::GetDesiredThrust() const {
	double thrust;
	return GetValueAsDouble("thrust",&thrust) ? thrust : SdThrustValues::MIN;
}

int CommandLineArgs::GetServerPort() const
{
	int port = SD_DEFAULT_TCP_PORT;
	GetValueAsInt("port",&port);
	return port;
}

std::string CommandLineArgs::GetHostAddress() const {
	return m_cmdArgs.get_value("host");
}

bool CommandLineArgs::IsRoleClient() const {
	return GetHostAddress().length() > 0;
}

bool CommandLineArgs::IsRoleServer() const {
	return GetBoolValue("server");
}

bool CommandLineArgs::IsDaemon() const {
	return m_cmdArgs.get_value("daemon") == "1";
}

bool CommandLineArgs::IsHelpNeeded() const {
	return GetBoolValue("help");
}

QuaternionD CommandLineArgs::GetTargetAttitude() const {
	return QuaternionD(1,0,0,0);
}

std::string CommandLineArgs::GetLogFile() const {
	return m_cmdArgs.get_value("log").c_str();
}

bool CommandLineArgs::GetValueAsDouble(const std::string &name, double* value) const
{
	std::string strVal = m_cmdArgs.get_value(name);
	if (strVal.length()) {
		*value = atof(strVal.c_str());
	}
	return !!strVal.length();
}

bool CommandLineArgs::GetValueAsInt(const std::string &name, int32_t* value) const
{
	std::string strVal = m_cmdArgs.get_value(name);
	if (strVal.length()) {
		*value = atoi(strVal.c_str());
	}
	return !!strVal.length();
}

bool CommandLineArgs::GetBoolValue(const std::string &name) const
{
	return m_cmdArgs.get_value(name) == "1";
}

bool CommandLineArgs::GetThrust(double* thrust) const
{
	return GetValueAsDouble("thrust",thrust);
}

bool CommandLineArgs::GetMinThrust(double* thrust) const
{
	return GetValueAsDouble("minthrust",thrust);
}

bool CommandLineArgs::GetMaxThrust(double* thrust) const
{
	return GetValueAsDouble("maxthrust",thrust);
}
