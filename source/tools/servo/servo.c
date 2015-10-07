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
#include "pca9685.h"

/*
 *
 * When controlling a servo, typically the on time of the pulse will
 * be varied from around 1 millisecond to 2 milliseconds with the off time around 20 milliseconds.
 *
 *    +---+                +---+
 *    |   |                |   |
 *    |   |                |   |
 *    |   |                |   |
 * ---+   +----------------+   +------
 * -->   <--1 ms
 *    <-------- 20 ms ---->
 *
 *
 *    +------+            +------+
 *    |      |            |      |
 *    |      |            |      |
 *    |      |            |      |
 * ---+      +------------+      +------
 *  -->      <--2 ms
 *    <------- 20 ms ----->
 *
 * The 20 ms period is not a hard requirement and depends on the specific servo motor.
 *
 */

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
	fprintf(stderr, "\t--set-motoroe 1|0                   Enable or disable power OE to motors.\n");
	fprintf(stderr, "\t--get-motoroe                       Get motor power OE state.\n");
	fprintf(stderr, "\t--set-pwmoe 1|0                     Enable or disable PWM OE from the servo controller.\n");
	fprintf(stderr, "\t--get-pwmoe                         Get PWM OE state.\n");
	fprintf(stderr, "\t--set-pulse <chan> <on> <off>       Set pulse on channel: 1..15, on: 0..4095, off: 0..4095\n");
	fprintf(stderr, "\t--get-pulse <chan>                  Set pulse on channel: 1..15, on: 0..4095, off: 0..4095\n");
	fprintf(stderr, "\n\n");
	fprintf(stderr, "Example: \n");
	fprintf(stderr, "At rate 250Hz (4ms period), the motor will barely start with pulse 1370 (1.3 ms)\n");
	fprintf(stderr, "and will reach its maximum with pulse 2200 (2.2 ms)\n");

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
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--set-pwmoe") == 0) {
			if (++i < argc) {
				arg = atol(argv[i]);
				ret = ioctl(fd, PCA9685_IOC_SETPWMOE, arg);
				if (ret < 0) {
					perror("PCA9685_IOC_SETPWMOE");
					return -1;
				}
			}
			return 0;
		}
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--get-pwmoe") == 0) {
			ret = ioctl(fd, PCA9685_IOC_GETPWMOE, 0);
			if (ret < 0) {
				perror("PCA9685_IOC_GETPWMOE");
				return -1;
			} else {
				fprintf(stdout, "%d\n", ret);
			}
			return 0;
		}
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--set-motoroe") == 0) {
			if (++i < argc) {
				arg = atol(argv[i]);
				ret = ioctl(fd, PCA9685_IOC_SETMOTOROE, arg);
				if (ret < 0) {
					perror("PCA9685_IOC_SETMOTOROE");
					return -1;
				}
			}
			return 0;
		}
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--get-motoroe") == 0) {
			ret = ioctl(fd, PCA9685_IOC_GETMOTOROE, 0);
			if (ret < 0) {
				perror("PCA9685_IOC_GETMOTOROE");
				return -1;
			} else {
				fprintf(stdout, "%d\n", ret);
			}
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
