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
#include "imusensor.h"

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


};


int main(int argc, const char *argv[])
{
	int i = 0;
	cmd_args args;
	imu_sensor::double3d_t acc_data = {0, 0, 0};
	imu_sensor::double3d_t gyr_data = {0, 0, 0};
	imu_sensor::double3d_t mag_data = {0, 0, 0};

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
		std::string gyr_fifo = args.get_value("gyr-fifo", "4");
		std::string acc_fifo = args.get_value("acc-fifo", "4");
		std::string gyr_device = args.get_value("gyr-device", "/dev/gyro0");
		std::string acc_device = args.get_value("acc-device", "/dev/accel0");
		std::string mag_device = args.get_value("mag-device", "/dev/mag0");

		imu_sensor gyr(gyr_device);
		imu_sensor acc(acc_device, imu_sensor::o_nonblock);
		imu_sensor mag(mag_device);
		gyr.open();
		acc.open();
		mag.open();
		gyr.set_rate(atoi(gyr_rate.c_str()));
		acc.set_rate(atoi(acc_rate.c_str()));
		gyr.set_fifo_threshold(atoi(gyr_fifo.c_str()));
		acc.set_fifo_threshold(atoi(acc_fifo.c_str()));
		while (true) {
			gyr.read_average(gyr_data.x, gyr_data.y, gyr_data.z);
			acc.read_average(acc_data.x, acc_data.y, acc_data.z);
			if ((i++) % 5 == 0)
				mag.read_average(mag_data.x, mag_data.y, mag_data.z);
			fprintf(stdout, "%10.0f %10.0f %10.0f %10.0f %10.0f %10.0f %10.0f %10.0f %10.0f\n",
					acc_data.x, acc_data.y, acc_data.z, gyr_data.x, gyr_data.y, gyr_data.z, mag_data.x, mag_data.y, mag_data.z);
		}

	} catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}



































#if 0
#include "axesdata.h"
#define GYRO_BUFFER_SIZE 256
#define DEFAULT_FRAME_SIZE 60
#define AXIS_NET_PROT "axes://"

const char *g_devgyro = "/dev/gyro0";
const char *g_devacc = "/dev/accel0";
const char *g_devmag = "/dev/mag0";
static int g_angle = 0;

typedef struct alldata_s {
	short3d_t gyro;
	short3d_t accel;
	short3d_t mag;
	int3d_t gyrV;
	int3d_t gyrS;
} alldata_t;

static int usage(int argc, const char *argv[])
{
	fprintf(stderr, "Axesdata: 1.0 \n");
	fprintf(stderr, "Copyright (C) 2012 Martin Stoilov\n\n");
	fprintf(stderr, "Usage: \n %s [OPTIONS] <devname>\n", argv[0]);
	fprintf(stderr, " OPTIONS:\n");
	fprintf(stderr, "\t-h, --help               Display this help.\n");
	fprintf(stderr, "\t-a, --angle              Display angle.\n");

	return 0;
}

static int alldata_parse_commandline(int argc, const char *argv[])
{
	int i;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "/?") == 0 || strcmp(argv[i], "-h") == 0) {
			usage(argc, argv);
			return -1;
		}
		if (strcmp(argv[i], "--angle") == 0) {
			g_angle = 1;
		}

	}
	return 0;
}


static int alldata_calculate_bias(int gyr_fd, int3d_t *bias)
{
	int i, ret;
	short3d_t raw;

	memset(bias, 0, sizeof(*bias));

	for (i = 0; i < 1000; i++) {
		ret = read(gyr_fd, &raw, sizeof(raw));
		if (ret < 0) {
			perror("Read Gyro");
			return -1;
		}
		bias->x += raw.x;
		bias->y += raw.y;
		bias->z += raw.z;
	}
	return 0;
}

int main(int argc, const char *argv[])
{
	alldata_t raw, last;
	int3d_t bias;
	int acc_fd, gyr_fd, mag_fd;
	int ret, counter = 0;
	int N = 190;
	int FS = 2000;
	int FSMAX = 32768;

	if (alldata_parse_commandline(argc, argv) < 0)
		return 1;
	memset(&raw, 0, sizeof(raw));
	memset(&last, 0, sizeof(last));
	gyr_fd = open(g_devgyro, O_RDONLY);
	if (gyr_fd < 0) {
		perror("Open Gyro");
		return -1;
	}
	acc_fd = open(g_devacc, O_RDONLY|O_NONBLOCK);
	if (acc_fd < 0) {
		perror("Open Accelerometer");
		return -1;
	}
	mag_fd = open(g_devmag, O_RDONLY);
	if (acc_fd < 0) {
		perror("Open Magntetometer");
		return -1;
	}

	ioctl(acc_fd, AXISDATA_IOC_SETRATE, 200);
	ioctl(gyr_fd, AXISDATA_IOC_SETRATE, 190);
	ioctl(acc_fd, AXISDATA_IOC_SETSCALE, 4);
	ioctl(gyr_fd, AXISDATA_IOC_SETSCALE, 2000);
	ioctl(acc_fd, AXISDATA_IOC_SETENABLED, 0);
	ioctl(gyr_fd, AXISDATA_IOC_SETENABLED, 0);
	ioctl(acc_fd, AXISDATA_IOC_RESETFIFO, 0);
	ioctl(gyr_fd, AXISDATA_IOC_RESETFIFO, 0);
	ioctl(acc_fd, AXISDATA_IOC_SETENABLED, 1);
	ioctl(gyr_fd, AXISDATA_IOC_SETENABLED, 1);

	read(mag_fd, &raw.mag, sizeof(raw.mag));

	if (g_angle) {
		if (alldata_calculate_bias(gyr_fd, &bias) < 0)
			return 1;
		last.gyrV.x = bias.x;
		last.gyrV.y = bias.y;
		last.gyrV.z = bias.z;
		N = ioctl(gyr_fd, AXISDATA_IOC_GETRATE, 0);
		FS = ioctl(gyr_fd, AXISDATA_IOC_GETSCALE, 0);
		FS += FS*20/100;
		ioctl(acc_fd, AXISDATA_IOC_RESETFIFO, 0);
		ioctl(gyr_fd, AXISDATA_IOC_RESETFIFO, 0);
		ioctl(acc_fd, AXISDATA_IOC_SETENABLED, 1);
		ioctl(gyr_fd, AXISDATA_IOC_SETENABLED, 1);
	}

	while (1) {
		ret = read(acc_fd, &raw.accel, sizeof(raw.accel));
		if (ret < 0 && errno != EAGAIN) {
			perror("Read Accel");
			return -1;
		}
		ret = read(gyr_fd, &raw.gyro, sizeof(raw.gyro));
		if (ret < 0) {
			perror("Read Gyro");
			return -1;
		}
		if (counter++ % 15) {
			ret = read(mag_fd, &raw.mag, sizeof(raw.mag));
			if (ret < 0) {
				perror("Read Magnetometer");
				return -1;
			}
		}
		fprintf(stdout, "%7d %7d %7d %7d %7d %7d %7d %7d %7d",
				raw.accel.x, raw.accel.y, raw.accel.z, raw.gyro.x, raw.gyro.y, raw.gyro.z, raw.mag.x, raw.mag.y, raw.mag.z);
		if (g_angle) {
			raw.gyrV.x = ((int)raw.gyro.x) * 1000 - bias.x;
			raw.gyrV.y = ((int)raw.gyro.y) * 1000 - bias.y;
			raw.gyrV.z = ((int)raw.gyro.z) * 1000 - bias.z;

			raw.gyrS.x = last.gyrS.x + (raw.gyrV.x  + last.gyrV.x)/1000; // * FS/FSMAX/N/2;
			raw.gyrS.y = last.gyrS.y + (raw.gyrV.y  + last.gyrV.y)/1000; // * FS/FSMAX/N/2;
			raw.gyrS.z = last.gyrS.z + (raw.gyrV.z  + last.gyrV.z)/1000; // * FS/FSMAX/N/2;
			last = raw;

//			fprintf(stdout, " %7d %7d %7d",
//				raw.gyrS.x,
//				raw.gyrS.y,
//				raw.gyrS.z);

			fprintf(stdout, " %7.1lf %7.1lf %7.1lf",
				((double)raw.gyrS.x)*FS/FSMAX/N/2.0,
				((double)raw.gyrS.y)*FS/FSMAX/N/2.0,
				((double)raw.gyrS.z)*FS/FSMAX/N/2.0);
		}
		fprintf(stdout, "\n");
		fflush(stdout);
	}
	return 0;
}

#endif
