/*
 * _CommandArgs.cpp
 *
 *  Created on: Sep 8, 2013
 *      Author: svassilev
 */

#include "commandargs.h"


_CommandArgs::_CommandArgs() :
	m_argc(0),
	m_argv(0)
{
	memset(&m_parsedArgs,0,sizeof(m_parsedArgs));
	m_parsedArgs.DroneCfg.Accel.DeviceName = "/dev/accel0";
	m_parsedArgs.DroneCfg.Accel.Rate = 200;
	m_parsedArgs.DroneCfg.Accel.Scale = 4;
	m_parsedArgs.DroneCfg.Accel.MaxReading = 32768;
	m_parsedArgs.DroneCfg.Gyro.DeviceName = "/dev/gyro0";
	m_parsedArgs.DroneCfg.Gyro.Rate = 200;
	m_parsedArgs.DroneCfg.Gyro.Scale = 2000;
	m_parsedArgs.DroneCfg.Gyro.MaxReading = 32768;
	m_parsedArgs.DroneCfg.Mag.DeviceName = "/dev/mag0";
	m_parsedArgs.DroneCfg.Servo.DeviceName = "/dev/pwm0";
	m_parsedArgs.DroneCfg.Servo.ChannelMask = 0xff;
	m_parsedArgs.DroneCfg.Servo.Rate = 100;
	m_parsedArgs.DroneCfg.Quad.Motor[0] = 0;
	m_parsedArgs.DroneCfg.Quad.Motor[1] = 2;
	m_parsedArgs.DroneCfg.Quad.Motor[2] = 3;
	m_parsedArgs.DroneCfg.Quad.Motor[3] = 4;
	m_parsedArgs.DroneCfg.Quad.ImuAngleAxisZ = 45;
	m_parsedArgs.DroneCfg.Quad.Kp = 0.6;
	m_parsedArgs.DroneCfg.Quad.Ki = 0.00000005;
	m_parsedArgs.DroneCfg.Quad.Kd = 0.0015;

	m_targetAttitude = QuaternionD(1,0,0,0);
}

_CommandArgs::~_CommandArgs()
{
}

void _CommandArgs::PrintUsage()
{
	fprintf(stdout, "\nSigma Drone 0.1\n");
	fprintf(stdout, "Controls the drone hardware; runs as a daemon server or "
			        "client process.\n");
	fprintf(stdout,"\n");
	fprintf(stdout, "Usage: \n");
	fprintf(stdout, "%s [command] [options]\n\n", !!m_argv ? m_argv[0] : "sdroned");
	fprintf(stdout, "command:\n");
	fprintf(stdout, " run                     Start the main control loop or execute\n"
			        "                         flight control command, aka --thrust\n");
	fprintf(stdout, " reset                   Reset drone hardware\n");
	fprintf(stdout, " exit                    Exit/terminate the daemon server\n");
	fprintf(stdout,"\n");
	fprintf(stdout, "options:\n");
	fprintf(stdout, " --help                  Display this help and exit\n");
	fprintf(stdout,"\n");
	fprintf(stdout, " --srv                   Execute as daemon server\n");
	fprintf(stdout, " --pid-pilot             Use PID pilot\n");
	fprintf(stdout, " --rot-matrix            Print the rotation matrix\n");
	fprintf(stdout, " --host    <HOSTNAME>    Execute as client and connect to\n"
			        "                         HOSTNAME\n");
	fprintf(stdout, " --port    <PORT>        Server TCP port. Default port is %d\n",SD_DEFAULT_PORT);
	fprintf(stdout,"\n");
	fprintf(stdout, " --maxdps  <DPS>         Gyro max angular speed in deg/s\n");
	fprintf(stdout, " --rate    <RATE>        Gyro and accel sampling rate\n");
	fprintf(stdout, " --motor   <NO> <CHANN>  Mapping between motor No and servo channel\n");
	fprintf(stdout, " --thrust  <0..1>        Set desired thrust per motor.\n"
					"                         NOTE: this will fire-up the engines!\n");
	fprintf(stdout, " --minthrust <0..9>      Min thrust value per motor, default is %1.2f\n",
			0.3);
	fprintf(stdout, " --maxthrust <0..99>     Max thrust value per motor, default is %1.2f\n",
			0.99);
	fprintf(stdout, " --sleep   <MILLISECS>   Time to sleep between state dump\n");
	fprintf(stdout, " --log     <FILENAME>    Capture daemon output to file\n");
	fprintf(stdout, " --infile  <FILENAME>    Input file for IMU readings, testing only\n");
	fprintf(stdout, " --outfile <FILENAME>    Out file for servo, testing only\n");
	fprintf(stdout, " --imuangle <ANGLE DEG>  Angle between the IMU and motor axis\n");
	fprintf(stdout,"\n");
	fprintf(stdout,
			"Additional notes:\n "
			"When command is not specified changes in the configuration\n"
			"are accumulated until following reset/run. Multiple run commands\n"
			"can be issued in order to control the flight; currently only\n"
			"--thrust is supported. Server process can be started just by\n"
			"specifying --srv; if command is specified in conjunction with\n"
			"--srv then the daemon will carry out the command and will wait\n"
			"for further instructions. If neither --srv nor --host are \n"
			"specified then the process will assume both client and server\n"
			"roles and will execute as console app.\n");
	fprintf(stdout,"\n");

	//fprintf(stdout, "	--config  <FILENAME>    Configuration file name (todo)\n");
}

bool
_CommandArgs::ParseArgs(
		int argc,
		char* argv[])
{
	int i = 1;
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
	m_parsedArgs.DroneCfg.Pilot = SD_PILOT_TYPE_QUADROTOR;

	// TODO: must be controlled from cmd line
	m_parsedArgs.DroneCfg.AccMap = Matrix3d( 0,-1, 0,
											1, 0, 0,
											0, 0, 1);
	m_parsedArgs.DroneCfg.MagMap = m_parsedArgs.DroneCfg.AccMap;
	m_parsedArgs.DroneCfg.GyroMap = Matrix3d(1, 0, 0,
											0, 1, 0,
											0, 0, 1);

	if (i < m_argc) {
		if (strcmp(m_argv[i], "run") == 0) {
			++i;
			m_parsedArgs.Command = SD_COMMAND_RUN;
		} else if (strcmp(m_argv[i], "reset") == 0) {
			++i;
			m_parsedArgs.Command = SD_COMMAND_RESET;
		}
		else if (strcmp(m_argv[i], "exit") == 0) {
			++i;
			m_parsedArgs.Command = SD_COMMAND_EXIT;
		}
	}

	for (; i < m_argc; i++) {
		if (strcmp(m_argv[i], "--help") == 0 ||
			strcmp(m_argv[i], "/?") == 0 ||
			strcmp(m_argv[i], "-?") == 0) {
			return false;
		} else if (strcmp(m_argv[i], "--maxdps") == 0) {
			if (++i < m_argc) {
				droneCfg->Gyro.Scale = atoi(m_argv[i]);
			}
		} else if (strcmp(m_argv[i], "--rate") == 0) {
			if (++i < m_argc) {
				droneCfg->Accel.Rate = droneCfg->Gyro.Rate = atoi(m_argv[i]);
			}
		} else if (strcmp(m_argv[i], "--motor") == 0) {
			if (i+2 < m_argc) {
				int motor = atoi(m_argv[++i]);
				if ((size_t)motor < ARRAYSIZE(droneCfg->Quad.Motor)) {
					droneCfg->Quad.Motor[motor]= atoi(m_argv[++i]);
				}
			}
		} else if (strcmp(m_argv[i], "--host") == 0) {
			if (++i < m_argc) {
				m_parsedArgs.HostAddress = m_argv[i];
				m_parsedArgs.IsClient = true;
			}
		} else if (strcmp(m_argv[i], "--port") == 0) {
			if (++i < m_argc) {
				m_parsedArgs.ServerPort = atoi(m_argv[i]);
			}
		} else if (strcmp(m_argv[i], "--thrust") == 0) {
			if (++i < m_argc) {
				m_parsedArgs.Thrust = atof(m_argv[i]);
			}
		} else if (strcmp(m_argv[i], "--minthrust") == 0) {
			if (++i < m_argc) {
				m_parsedArgs.MinThrust = atof(m_argv[i]);
			}
		} else if (strcmp(m_argv[i], "--maxthrust") == 0) {
			if (++i < m_argc) {
				m_parsedArgs.MaxThrust = atof(m_argv[i]);
			}
		} else if (strcmp(m_argv[i], "--Kd") == 0) {
			if (++i < m_argc) {
				m_parsedArgs.DroneCfg.Quad.Kd = atof(m_argv[i]);
			}
		} else if (strcmp(m_argv[i], "--Kp") == 0) {
			if (++i < m_argc) {
				m_parsedArgs.DroneCfg.Quad.Kp = atof(m_argv[i]);
			}
		} else if (strcmp(m_argv[i], "--Ki") == 0) {
			if (++i < m_argc) {
				m_parsedArgs.DroneCfg.Quad.Ki = atof(m_argv[i]);
			}
		} else if (strcmp(m_argv[i], "--srv") == 0) {
			m_parsedArgs.IsServer = true;
		} else if (strcmp(m_argv[i], "--infile") == 0) {
			if (++i < m_argc) {
				droneCfg->Gyro.DeviceName = droneCfg->Accel.DeviceName = m_argv[i];
			}
		} else if (strcmp(m_argv[i], "--log") == 0) {
			if (++i < m_argc) {
				m_parsedArgs.LogFile = m_argv[i];
			}
		} else if (strcmp(m_argv[i], "--outfile") == 0) {
			if (++i < m_argc) {
				droneCfg->Servo.DeviceName = m_argv[i];
			}
		} else if (strcmp(m_argv[i], "--sleep") == 0) {
			if (++i < m_argc) {
				int timeToSleep = atoi(m_argv[i]);
				m_parsedArgs.TimeToSleep = (timeToSleep >= 0) ? timeToSleep : 1000;
			}
		} else if (strcmp(m_argv[i], "--imuangle") == 0) {
			if (++i < m_argc) {
				droneCfg->Quad.ImuAngleAxisZ = atoi(m_argv[i]);
			}
		} else if (strcmp(m_argv[i], "--pid-pilot") == 0) {
			droneCfg->Pilot = SD_PILOT_TYPE_PID;
		} else if (strcmp(m_argv[i], "--rot-matrix") == 0) {
			droneCfg->PrintRotationMatrix = 1;
		} else {
			return false;
		}
	}
	return true;
}

SdCommand _CommandArgs::GetCommand() {
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
