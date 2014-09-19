/*
 * Copyright (C) 2013 Martin Stoilov
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include "cmdargs.h"
#include "sampler.h"

static cmd_arg_spec g_argspec[] = {
		{"help",		"h",	"Display this help", CMD_ARG_BOOL},
		{"gyr-rate",	"",		"Set gyroscope sampling rate. Supported rates: 95, 190, 380, 760. Default: 190", CMD_ARG_STRING},
		{"acc-rate",	"",		"Set accelerometer sampling rate. Supported rates: 1, 10, 25, 50, 100, 200, 400. Default: 200", CMD_ARG_STRING},
		{"mag-rate",	"",		"Set magnetometer sampling rate. Supported rates: 3, 15, 30, 75, 220. Default: 220", CMD_ARG_STRING},
		{"gyr-fifo",	"",		"Set gyroscope FIFO threshold. Must be from 1 to 32. Default: 4", CMD_ARG_STRING},
		{"acc-fifo",	"",		"Set accelerometer FIFO threshold. Must be from 1 to 32. Default: 4", CMD_ARG_STRING},
		{"gyr-device",	"",		"Set the gyroscope device filename. Default: /dev/gyro0", CMD_ARG_STRING},
		{"acc-device",	"",		"Set the accelerometer device filename. Default: /dev/accel0", CMD_ARG_STRING},
		{"mag-device",	"",		"Set the magnetometer device filename. Default: /dev/mag0", CMD_ARG_STRING},
		{"bar-device",	"",		"Set the barometric pressure device filename. Default: /sys/bus/i2c/devices/4-0077/pressure0_input", CMD_ARG_STRING},
		{"gyr-scale",	"",		"Set gyroscope full scale. Supported scales (DPS): 250, 500, 2000. Default: 2000", CMD_ARG_STRING},
		{"acc-scale",	"",		"Set accelerometer full scale. Supported scales (G): 2, 4, 8, 16. Default: 4", CMD_ARG_STRING},
		{"mag-scale",	"",		"Set magnetometer full scale. Supported scales (Gauss): 1300, 1900, 2500, 4000, 4700, 5600, 8100. Default: 1300", CMD_ARG_STRING},
};


int main(int argc, const char *argv[])
{
	cmd_args args;

	try {
		args.add_specs(g_argspec, sizeof(g_argspec)/sizeof(*g_argspec));
		args.parse_command_line(argc, argv);
		if (!args.get_value("help").empty()) {
			std::cout << argv[0] << " <options>" << std::endl;
			std::cout << args.get_help_message() << std::endl;
			return 0;
		}
		std::string gyr_rate = args.get_value("gyr-rate", "190");
		std::string acc_rate = args.get_value("acc-rate", "200");
		std::string mag_rate = args.get_value("mag-rate", "220");
		std::string gyr_scale = args.get_value("gyr-scale", "2000");
		std::string acc_scale = args.get_value("acc-scale", "4");
		std::string mag_scale = args.get_value("mag-scale", "1300");
		std::string gyr_fifo = args.get_value("gyr-fifo", "4");
		std::string acc_fifo = args.get_value("acc-fifo", "4");
		std::string gyr_device = args.get_value("gyr-device", "/dev/gyro0");
		std::string acc_device = args.get_value("acc-device", "/dev/accel0");
		std::string mag_device = args.get_value("mag-device", "/dev/mag0");
		std::string bar_device = args.get_value("bar-device", "/sys/bus/i2c/devices/4-0077/pressure0_input");

		sampler sensorsamples(gyr_device, acc_device, mag_device, bar_device);
		sensorsamples.gyr_.set_rate(atoi(gyr_rate.c_str()));
		sensorsamples.acc_.set_rate(atoi(acc_rate.c_str()));
		sensorsamples.gyr_.set_full_scale(atoi(gyr_scale.c_str()));
		sensorsamples.acc_.set_full_scale(atoi(acc_scale.c_str()));
		sensorsamples.mag_.set_full_scale(atoi(mag_scale.c_str()));
		sensorsamples.gyr_.set_fifo_threshold(atoi(gyr_fifo.c_str()));
		sensorsamples.acc_.set_fifo_threshold(atoi(acc_fifo.c_str()));
		sensorsamples.init();
		while (true) {
			sensorsamples.update();
			fprintf(stdout, "%10.2lf %10.2lf %10.2lf    %10.2lf %10.2lf %10.2lf    %10.2lf %10.2lf %10.2lf    %10.2lf    %10.3lf\n",
					sensorsamples.data.acc3d_.at(0, 0), sensorsamples.data.acc3d_.at(1, 0), sensorsamples.data.acc3d_.at(2, 0),
					sensorsamples.data.gyr3d_.at(0, 0), sensorsamples.data.gyr3d_.at(1, 0), sensorsamples.data.gyr3d_.at(2, 0),
					sensorsamples.data.mag3d_.at(0, 0), sensorsamples.data.mag3d_.at(1, 0), sensorsamples.data.mag3d_.at(2, 0),
					sensorsamples.data.bar1d_,
					sensorsamples.data.dtime_);
			fflush(stdout);
		}

	} catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}
