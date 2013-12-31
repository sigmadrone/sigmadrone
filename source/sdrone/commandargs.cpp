/*
 * _CommandArgs.cpp
 *
 *  Created on: Sep 8, 2013
 *      Author: svassilev
 */

#include "commandargs.h"
#include <stdexcept>

#include <json-glib/json-glib.h>


static CmdArgSpec s_argSpec[] = {
		{"help",		"h", "Display this help", CMD_ARG_BOOL},
		{"command",		"c", "reset|exit|run|ping|loadplugin|unloadplugin", CMD_ARG_STRING},
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


_CommandArgs::_CommandArgs() :
	m_argc(0),
	m_argv(0)
{
	memset(&m_parsedArgs,0,sizeof(m_parsedArgs));
	m_parsedArgs.DroneCfg.Accel.DeviceName = "/dev/accel0";
	m_parsedArgs.DroneCfg.Accel.SamplingRate = 200;
	m_parsedArgs.DroneCfg.Accel.Scale = 4;
	m_parsedArgs.DroneCfg.Accel.MaxReading = 32768;
	m_parsedArgs.DroneCfg.Accel.Watermark = 2;
	m_parsedArgs.DroneCfg.Gyro.DeviceName = "/dev/gyro0";
	m_parsedArgs.DroneCfg.Gyro.SamplingRate = 200;
	m_parsedArgs.DroneCfg.Gyro.Scale = 2000;
	m_parsedArgs.DroneCfg.Gyro.MaxReading = 32768;
	m_parsedArgs.DroneCfg.Gyro.NumBiasSamples = 4000;
	m_parsedArgs.DroneCfg.Gyro.Watermark = 2;
	m_parsedArgs.DroneCfg.Mag.DeviceName = "/dev/mag0";
	m_parsedArgs.DroneCfg.Servo.DeviceName = "/dev/pwm0";
	m_parsedArgs.DroneCfg.Servo.ChannelMask = 0xff;
	m_parsedArgs.DroneCfg.Servo.Rate = 390;
	m_parsedArgs.DroneCfg.Quad.Motor[0] = 0;
	m_parsedArgs.DroneCfg.Quad.Motor[1] = 1;
	m_parsedArgs.DroneCfg.Quad.Motor[2] = 2;
	m_parsedArgs.DroneCfg.Quad.Motor[3] = 3;
	m_parsedArgs.DroneCfg.Quad.ImuAngleAxisZ = 45;
	/*
	 * PID coefficients for big props on DJI F450
	 * Kp = 1.0 <-> 1.3
	 * Ki = 1.2, with leak rate dT
	 * Kd = 0.3 <-> 0.45
	 */
	m_parsedArgs.DroneCfg.Quad.Kp = 1.0;
	m_parsedArgs.DroneCfg.Quad.Ki = 1.2;
	m_parsedArgs.DroneCfg.Quad.Kd = 0.35;
	m_parsedArgs.DroneCfg.LogLevel = SD_LOG_LEVEL_VERBOSE;
	m_parsedArgs.DroneCfg.LogRate = 1;

	m_targetAttitude = QuaternionD(1,0,0,0);

	m_cmdArgs.AddSpecs(s_argSpec,sizeof(s_argSpec)/sizeof(s_argSpec[0]));
}

_CommandArgs::~_CommandArgs()
{
}


void _CommandArgs::PrintUsage()
{
	printf("Sigma Drone 0.1\n\n");
	printf("Usage: %s <options>\n", !!m_argv ? m_argv[0] : "sdroned");
	printf("%s\n",m_cmdArgs.HelpMessage().c_str());

	printf( "Additional notes:\n"
			"When command is not specified changes in the configuration are accumulated \n"
			"until following reset/run. Multiple run commands can be issued in order to \n"
			"control the flight; currently only --thrust is supported. Server process can \n"
			"be started just by specifying --server; if command is specified in conjunction \n"
			"with --server then the daemon will carry out the command and will wait for \n"
			"further instructions. If neither --server nor --host are specified then the \n"
			"process will assume both client and server roles and will execute as console app.\n");
}

bool
_CommandArgs::ParseArgs(
		int argc,
		char* argv[])
{
	SdDroneConfig* droneCfg = &m_parsedArgs.DroneCfg;

	m_argc = argc;
	m_argv = argv;

	//
	// Init with the current default values
	//
	m_parsedArgs.Command = SD_COMMAND_NONE;
	m_parsedArgs.HostAddress = "127.0.0.1";
	m_parsedArgs.ServerPort = SD_DEFAULT_PORT;
	m_parsedArgs.IsClient = m_parsedArgs.IsServer = false;
	m_parsedArgs.MinThrust = 0.3;
	m_parsedArgs.MaxThrust = 0.99;
	m_parsedArgs.Thrust = 0.5;

	m_parsedArgs.DroneCfg.Accel.CoordinateMap = Matrix3d(
			0,-1, 0,
			1, 0, 0,
			0, 0, 1);
	m_parsedArgs.DroneCfg.Mag.CoordinateMap = m_parsedArgs.DroneCfg.Accel.CoordinateMap;
	m_parsedArgs.DroneCfg.Gyro.CoordinateMap = Matrix3d(
			1, 0, 0,
			0, 1, 0,
			0, 0, 1);

	try {
		m_cmdArgs.Parse(argc, argv);
	} catch (std::out_of_range exc) {
		printf("%s\n",exc.what());
		return false;
	}

	if (m_cmdArgs.GetBoolValue("help")) {
		return false;
	}

	if (m_cmdArgs.GetValue("command").c_str() == std::string("run")) {
		m_parsedArgs.Command = SD_COMMAND_RUN;
	} else if (m_cmdArgs.GetValue("command").c_str() == std::string("reset")) {
		m_parsedArgs.Command = SD_COMMAND_RESET;
	} else if (m_cmdArgs.GetValue("command").c_str() == std::string("run")) {
		m_parsedArgs.Command = SD_COMMAND_EXIT;
	} else if (m_cmdArgs.GetValue("command").c_str() == std::string("ping")) {
		m_parsedArgs.Command = SD_COMMAND_PING;
	}
	m_cmdArgs.GetValueAsInt("maxdps",&droneCfg->Gyro.Scale);
	m_cmdArgs.GetValueAsInt("rate",&droneCfg->Gyro.SamplingRate);
	droneCfg->Accel.SamplingRate = droneCfg->Gyro.SamplingRate;
	if (m_cmdArgs.GetValue("host").length() > 0) {
		m_parsedArgs.HostAddress = m_cmdArgs.GetValue("host").c_str();
		m_parsedArgs.IsClient = true;
	}
	m_parsedArgs.IsServer = m_cmdArgs.GetBoolValue("server");
	m_cmdArgs.GetValueAsInt("port",&m_parsedArgs.ServerPort);
	m_cmdArgs.GetValueAsDouble("thrust",&m_parsedArgs.Thrust);
	m_cmdArgs.GetValueAsDouble("minthrust",&m_parsedArgs.MinThrust);
	m_cmdArgs.GetValueAsDouble("maxthrust",&m_parsedArgs.MaxThrust);
	if (m_cmdArgs.GetValue("infile").length() > 0) {
		droneCfg->Gyro.DeviceName =m_cmdArgs.GetValue("infile").c_str();
		droneCfg->Accel.DeviceName = droneCfg->Gyro.DeviceName;
		droneCfg->Mag.DeviceName = droneCfg->Gyro.DeviceName;
	}
	if (m_cmdArgs.GetValue("outfile").length() > 0) {
		droneCfg->Servo.DeviceName =m_cmdArgs.GetValue("outfile").c_str();
	}
	if (m_cmdArgs.GetValue("log").length() > 0) {
		m_parsedArgs.LogFile = m_cmdArgs.GetValue("log").c_str();
	}
	m_cmdArgs.GetValueAsDouble("lograte", &droneCfg->LogRate);
	droneCfg->LogRotationMatrix = m_cmdArgs.GetBoolValue("rot-matrix");
	m_cmdArgs.GetValueAsInt("imuangle",&droneCfg->Quad.ImuAngleAxisZ);
	m_cmdArgs.GetValueAsDouble("Kp", &droneCfg->Quad.Kp);
	m_cmdArgs.GetValueAsDouble("Ki", &droneCfg->Quad.Ki);
	m_cmdArgs.GetValueAsDouble("Kd", &droneCfg->Quad.Kd);
	return true;
}

SdCommandCode _CommandArgs::GetCommand() {
	return m_parsedArgs.Command;
}

const char* _CommandArgs::GetArgAsString(int index) {
	return (index < m_argc) ? m_argv[index] : "";
}

int _CommandArgs::GetArgAsInt(int index) {
	return (index < m_argc) ? atoi(m_argv[index]) : -1;
}

int64_t _CommandArgs::GetArgAsInt64(int index) {
	return (index < m_argc) ? atol(m_argv[index]) : -1;
}

double _CommandArgs::GetArgAsDouble(int index) {
	return (index < m_argc) ? atof(m_argv[index]) : -1;
}

int _CommandArgs::IsArgPresent(const char* argName) {
	for (int i = 0; i < m_argc; i++) {
		if (0 == strcmp(argName,m_argv[i])) {
			return i;
		}
	}
	return -1;
}

const char** _CommandArgs::GetArgv() {
	return (const char**)m_argv;
}

int _CommandArgs::GetArgc() {
	return m_argc;
}

double _CommandArgs::GetMinThrust() {
	return m_parsedArgs.MinThrust;
}

double _CommandArgs::GetMaxThrust() {
	return m_parsedArgs.MaxThrust;
}

double _CommandArgs::GetDesiredThrust() {
	return m_parsedArgs.Thrust;
}

const QuaternionD* _CommandArgs::GetTargetAttitude() {
	return &m_targetAttitude;
}

bool _CommandArgs::IsRoleServer() {
	return m_parsedArgs.IsServer;
}

bool _CommandArgs::IsRoleClient() {
	return m_parsedArgs.IsClient;
}
