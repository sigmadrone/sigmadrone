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
		{"servo-ctrl",		"",		"Specify which servo controller class to instantiate. Supported values: pca9685", CMD_ARG_STRING},
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
		{"gyr-rate",		"",		"Set gyroscope sampling rate. Supported rates: 95, 190, 380, 760. Default: 190", CMD_ARG_STRING},
		{"acc-rate",		"",		"Set accelerometer sampling rate. Supported rates: 1, 10, 25, 50, 100, 200, 400. Default: 200", CMD_ARG_STRING},
		{"mag-rate",		"",		"Set magnetometer sampling rate. Supported rates: 3, 15, 30, 75, 220. Default: 220", CMD_ARG_STRING},
		{"gyr-fifo",		"",		"Set gyroscope FIFO threshold. Must be from 1 to 32. Default: 4", CMD_ARG_STRING},
		{"acc-fifo",		"",		"Set accelerometer FIFO threshold. Must be from 1 to 32. Default: 4", CMD_ARG_STRING},
		{"gyr-device",		"",		"Set the gyroscope device filename. Default: /dev/gyro0", CMD_ARG_STRING},
		{"acc-device",		"",		"Set the accelerometer device filename. Default: /dev/accel0", CMD_ARG_STRING},
		{"mag-device",		"",		"Set the magnetometer device filename. Default: /dev/mag0", CMD_ARG_STRING},
		{"bar-device",		"",		"Set the barometric pressure device filename. Default: /sys/bus/i2c/devices/4-0077/pressure0_input", CMD_ARG_STRING},
		{"gyr-scale",		"",		"Set gyroscope full scale. Supported scales (DPS): 250, 500, 2000. Default: 2000", CMD_ARG_STRING},
		{"acc-scale",		"",		"Set accelerometer full scale. Supported scales (G): 2, 4, 8, 16. Default: 4", CMD_ARG_STRING},
		{"mag-scale",		"",		"Set magnetometer full scale. Supported scales (Gauss): 1300, 1900, 2500, 4000, 4700, 5600, 8100. Default: 1300", CMD_ARG_STRING},
		{"gyr-adjustment",	"",		"Set adjustment factor of the gyroscope readings. The sensor reading is multiplied by this factor. Default: 1.0", CMD_ARG_STRING},
		{"bias-samples",	"",		"The number of samples to be used to calculate sensors bias", CMD_ARG_STRING},
		{"acc-preiod",		"",		"The accelerometer period used in attitude tracker.", CMD_ARG_STRING},
		{"fps",				"",		"Frames per second. How often to output. Default: 1000", CMD_ARG_STRING},
		{"firmware-uart",	"",		"Firmware uart device. Default: /dev/ttymxc0", CMD_ARG_STRING},
		{"gyr-disable",		"",		"Disable reading gyroscope", CMD_ARG_BOOL},
		{"acc-disable",		"",		"Disable reading accelerometer", CMD_ARG_BOOL},
		{"mag-disable",		"",		"Disable reading magnetometer", CMD_ARG_BOOL},
		{"bar-disable",		"",		"Disable reading baromerter", CMD_ARG_BOOL},
		{"disable-sensors",	"",		"Disable sensor reading.", CMD_ARG_BOOL},

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
