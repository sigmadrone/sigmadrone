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
#include "attitudetracker.h"

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
		{"gyr-adjustment","",	"Set adjustment factor of the gyroscope readings. The sensor reading is multiplied by this factor. Default: 1.0", CMD_ARG_STRING},
		{"bias-samples","",		"The number of samples to be used to calculate sensors bias", CMD_ARG_STRING},
		{"nlerp-blend","",		"The g_blend used in attitude tracker.", CMD_ARG_STRING},
		{"fps",			"",		"Frames per second. How often to output. Default: 1000", CMD_ARG_STRING},
		{"gyr-disable",	"",		"Disable reading gyroscope", CMD_ARG_BOOL},
		{"acc-disable",	"",		"Disable reading accelerometer", CMD_ARG_BOOL},
		{"mag-disable",	"",		"Disable reading magnetometer", CMD_ARG_BOOL},
		{"bar-disable",	"",		"Disable reading baromerter", CMD_ARG_BOOL},
		{"rot-matrix",	"",		"Display the rotation matrix", CMD_ARG_BOOL},
		{"gyr-notrack",	"",		"Disable attitude tracking from gyroscope readings", CMD_ARG_BOOL},
		{"acc-notrack",	"",		"Disable attitude tracking from accelerometer readings", CMD_ARG_BOOL},

};


int main(int argc, const char *argv[])
{
	cmd_args args;
	double fperiod = 0.0;

	try {
		args.add_specs(g_argspec, sizeof(g_argspec)/sizeof(*g_argspec));
		args.parse_command_line(argc, argv);
		if (!args.get_value("help").empty()) {
			std::cout << argv[0] << " <options>" << std::endl;
			std::cout << args.get_help_message() << std::endl;
			return 0;
		}
		attitudetracker attitude(atof(args.get_value("nlerp-blend", "0.05").c_str()));
		sampler sensorsamples(
				args.get_value("gyr-disable").empty() ? args.get_value("gyr-device", "/dev/gyro0") : "",
				args.get_value("acc-disable").empty() ? args.get_value("acc-device", "/dev/accel0") : "",
				args.get_value("mag-disable").empty() ? args.get_value("mag-device", "/dev/mag0") : "",
				args.get_value("bar-disable").empty() ? args.get_value("bar-device", "/sys/bus/i2c/devices/4-0077/pressure0_input") : "");
		if (args.get_value("gyr-disable").empty() && !args.get_value("gyr-rate").empty())
			sensorsamples.gyr_.set_rate(atoi(args.get_value("gyr-rate").c_str()));
		if (args.get_value("acc-disable").empty() && !args.get_value("acc-rate").empty())
			sensorsamples.acc_.set_rate(atoi(args.get_value("acc-rate").c_str()));
		if (args.get_value("mag-disable").empty() && !args.get_value("mag-rate").empty())
			sensorsamples.mag_.set_rate(atoi(args.get_value("mag-rate").c_str()));
		if (args.get_value("gyr-disable").empty() && !args.get_value("gyr-scale").empty())
			sensorsamples.gyr_.set_full_scale(atoi(args.get_value("gyr-scale").c_str()));
		if (args.get_value("acc-disable").empty() && !args.get_value("acc-scale").empty())
			sensorsamples.acc_.set_full_scale(atoi(args.get_value("acc-scale").c_str()));
		if (args.get_value("mag-disable").empty() && !args.get_value("mag-scale").empty())
			sensorsamples.mag_.set_full_scale(atoi(args.get_value("mag-scale").c_str()));
		if (args.get_value("gyr-disable").empty() && !args.get_value("gyr-fifo").empty())
			sensorsamples.gyr_.set_fifo_threshold(atoi(args.get_value("gyr-fifo").c_str()));
		if (args.get_value("acc-disable").empty() && !args.get_value("acc-fifo").empty())
			sensorsamples.acc_.set_fifo_threshold(atoi(args.get_value("acc-fifo").c_str()));
		sensorsamples.gyr_.set_adjustment(atof(args.get_value("gyr-adjustment", "1.2").c_str()));
		sensorsamples.gyr_.bias_update(atoi(args.get_value("bias-samples", "200").c_str()));
		sensorsamples.init();
		while (true) {
			sensorsamples.update();
			if (args.get_value("gyr-notrack").empty())
				attitude.track_gyroscope(DEG2RAD(sensorsamples.data.gyr3d_), sensorsamples.data.dtime_gyr_);
			if (args.get_value("acc-notrack").empty() && sensorsamples.data.acc3d_upd_)
				attitude.track_accelerometer(sensorsamples.data.acc3d_.normalize(), sensorsamples.data.dtime_acc_);
			fperiod += sensorsamples.data.dtime_gyr_;
			if (fperiod < 1.0/(atof(args.get_value("fps", "1000").c_str()) + 1))
				continue;
			fperiod = 0.0;
			if (!args.get_value("rot-matrix").empty()) {
				QuaternionD q = attitude.get_attitude();
				fprintf(stdout, "%5.9lf %5.9lf %5.9lf %5.9lf\n", q.w, q.x, q.y, q.z);
			} else {
				fprintf(stdout, "%10.2lf %10.2lf %10.2lf    %10.2lf %10.2lf %10.2lf    %10.2lf %10.2lf %10.2lf    %10.2lf    %10.3lf\n",
						sensorsamples.data.acc3d_.at(0, 0), sensorsamples.data.acc3d_.at(1, 0), sensorsamples.data.acc3d_.at(2, 0),
						sensorsamples.data.gyr3d_.at(0, 0), sensorsamples.data.gyr3d_.at(1, 0), sensorsamples.data.gyr3d_.at(2, 0),
						sensorsamples.data.mag3d_.at(0, 0), sensorsamples.data.mag3d_.at(1, 0), sensorsamples.data.mag3d_.at(2, 0),
						sensorsamples.data.bar1d_,
						sensorsamples.data.dtime_gyr_);
			}
			fflush(stdout);
		}

	} catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}
