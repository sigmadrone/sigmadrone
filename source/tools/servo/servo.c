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
#include "pca9685.h"


const char *g_devname = "/dev/pwm0";


static int usage(int argc, const char *argv[])
{
	fprintf(stderr, "Servo control: 1.0 \n");
	fprintf(stderr, "Copyright (C) 2012 Martin Stoilov\n\n");
	fprintf(stderr, "Usage: \n %s [OPTIONS] <devname>\n", argv[0]);
	fprintf(stderr, " OPTIONS:\n");
	fprintf(stderr, "\t-h, --help                          Display this help.\n");
	fprintf(stderr, "\t--set-rate N                        Set sample rate. Setting to 0 powers down the device.\n");
	fprintf(stderr, "\t--get-rate                          Get sample rate.\n");
	fprintf(stderr, "\t--set-pulse <chan> <on> <off>       Set pulse on channel: 1..15, on: 0..4095, off: 0..4095\n");
	fprintf(stderr, "\t--get-pulse <chan>                  Set pulse on channel: 1..15, on: 0..4095, off: 0..4095\n");

	return 0;
}


static int servo_parse_commandline(int argc, const char *argv[])
{
	int i;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "/?") == 0 || strcmp(argv[i], "-h") == 0) {
			usage(argc, argv);
			return -1;
		} else if (argv[i][0] != '-') {
			g_devname = argv[i];
		}
	}
	return 0;
}


static int servo_apply_commandline(int fd, int argc, const char *argv[])
{
	unsigned long arg;
	int ret, i;
	unsigned short chan, on, off;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--set-rate") == 0) {
			if (++i < argc) {
				arg = atol(argv[i]);
				ret = ioctl(fd, PCA9685_IOC_SETRATE, arg);
				if (ret < 0) {
					perror("SERVO_IOC_SETRATE");
					return -1;
				}
			}
			return 0;
		}
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--get-rate") == 0) {
			ret = ioctl(fd, PCA9685_IOC_GETRATE, 0);
			if (ret < 0) {
				perror("SERVO_IOC_GETRATE");
				return -1;
			} else {
				fprintf(stdout, "%d\n", ret);
			}
			return 0;
		}
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--set-pulse") == 0) {
			if (++i < argc) {
				chan = (unsigned short)atol(argv[i]);
			} else {
				return -1;
			}
			if (++i < argc) {
				on = (unsigned short)atol(argv[i]);
			} else {
				return -1;
			}
			if (++i < argc) {
				off = (unsigned short)atol(argv[i]);
			} else {
				return -1;
			}
			ret = ioctl(fd, PCA9685_IOC_SETLED0 + chan, PCA9685_PULSE(on, off));
			if (ret < 0) {
				perror("SERVO_IOC_SETLED");
				return -1;
			}
			return 0;
		}
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--get-pulse") == 0) {
			if (++i < argc) {
				chan = (unsigned short)atol(argv[i]);
			} else {
				return -1;
			}
			ret = ioctl(fd, PCA9685_IOC_GETLED0 + chan, 0);
			if (ret < 0) {
				perror("SERVO_IOC_GETLED");
				return -1;
			}
			fprintf(stdout, "%d %d\n", ret >> 16, (ret & 0xFFFF));
			return 0;
		}
	}

	return 0;
}

int main(int argc, const char *argv[])
{
	int fd;

	if (servo_parse_commandline(argc, argv) < 0)
		return 1;

	fd = open(g_devname, O_RDONLY);
	if (fd < 0) {
		perror("Open");
		return 2;
	}
	if (servo_apply_commandline(fd, argc, argv) < 0)
		return 2;
	fflush(stdout);
	close(fd);
	return 0;
}
