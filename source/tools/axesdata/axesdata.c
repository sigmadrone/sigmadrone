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
#include "axesdata.h"

#define GYRO_BUFFER_SIZE 256
#define DEFAULT_FRAME_SIZE 60
#define AXIS_NET_PROT "axes://"

static int g_textdata = 0;
static int g_delay = 0;
static int g_gnuplot = 0;
static int g_framesize = DEFAULT_FRAME_SIZE;
const char *g_devname = "/dev/gyro0";

static void axesdata_update_gnuplot(FILE *gnuplot, short3d_t *framebuf, int frameptr);
static void axesdata_int_gnuplot(FILE *gnuplot);


static int usage(int argc, const char *argv[])
{
	fprintf(stderr, "Axesdata: 1.0 \n");
	fprintf(stderr, "Copyright (C) 2012 Martin Stoilov\n\n");
	fprintf(stderr, "Usage: \n %s [OPTIONS] <devname>\n", argv[0]);
	fprintf(stderr, " OPTIONS:\n");
	fprintf(stderr, "\t-h, --help               Display this help.\n");
	fprintf(stderr, "\t--text                   Read text data.\n");
	fprintf(stderr, "\t--set-rate N             Set sample rate. Setting to 0 powers down the device.\n");
	fprintf(stderr, "\t--get-rate               Get sample rate.\n");
	fprintf(stderr, "\t--set-scale N            Set scale.\n");
	fprintf(stderr, "\t--get-scale              Get scale.\n");
	fprintf(stderr, "\t--set-mode N             Set the device operation mode. AXESDATA_OPMODE_STREAM = 0, AXESDATA_OPMODE_BYPASS = 1\n");
	fprintf(stderr, "\t--get-mode               Get the current device operation mode.\n");
	fprintf(stderr, "\t--set-watermark N        Set FIFO watermark, N must be between 1 and 31.\n");
	fprintf(stderr, "\t--get-watermark          Get the current FIFO watermark.\n");
	fprintf(stderr, "\t--enable                 Enable sensor sampling.\n");
	fprintf(stderr, "\t--disable                Disable sensor sampling.\n");
	fprintf(stderr, "\t--gnuplot                Read text data.\n");
	fprintf(stderr, "\t--framesize N            Frame size.\n");
	fprintf(stderr, "\t--hp-enable              Enable high pass filter.\n");
	fprintf(stderr, "\t--hp-disable             Disable high pass filter.\n");
	fprintf(stderr, "\t--set-hpmode N           Set high pass mode.\n");
	fprintf(stderr, "\t--delay N                Add delay of N ms after every data output.\n");
	return 0;
}

static int axesdata_parse_commandline(int argc, const char *argv[])
{
	int i;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "/?") == 0 || strcmp(argv[i], "-h") == 0) {
			usage(argc, argv);
			return -1;
		} else if (strcmp(argv[i], "--text") == 0) {
			g_textdata = 1;
		} else if (strcmp(argv[i], "--gnuplot") == 0) {
			g_gnuplot = 1;
		} else if (strcmp(argv[i], "--delay") == 0) {
			if (++i < argc) {
				g_delay = atoi(argv[i]);
				if (g_delay < 0)
					g_delay = 0;
			}
		} else if (strcmp(argv[i], "--framesize") == 0) {
			if (++i < argc) {
				g_framesize = atoi(argv[i]);
				if (g_framesize < 0)
					g_framesize = DEFAULT_FRAME_SIZE;
			}
		} else if (argv[i][0] != '-') {
			g_devname = argv[i];
		}
	}
	return 0;
}


static int axesdata_apply_commandline(int fd, int argc, const char *argv[])
{
	unsigned long arg;
	int ret, i;
	int	bexit = 0;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--disable") == 0) {
			ret = ioctl(fd, AXISDATA_IOC_SETENABLED, 0);
			if (ret < 0) {
				perror("AXISDATA_IOC_SETENABLED");
				return -1;
			}
			ret = ioctl(fd, AXISDATA_IOC_RESETFIFO, 0);
			if (ret < 0) {
				perror("AXISDATA_IOC_RESETFIFO");
				return -1;
			}
			bexit = 1;
		}
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--enable") == 0) {
			ret = ioctl(fd, AXISDATA_IOC_SETENABLED, 1);
			if (ret < 0) {
				perror("AXISDATA_IOC_SETENABLED");
				return -1;
			}
			bexit = 1;
		}
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--hp-enable") == 0) {
			ret = ioctl(fd, AXISDATA_IOC_HPEN, 1);
			if (ret < 0) {
				perror("AXISDATA_IOC_HPEN");
				return -1;
			}
			bexit = 1;
		}
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--hp-disable") == 0) {
			ret = ioctl(fd, AXISDATA_IOC_HPEN, 0);
			if (ret < 0) {
				perror("AXISDATA_IOC_HPEN");
				return -1;
			}
			bexit = 1;
		}
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--set-hpmode") == 0) {
			if (++i < argc) {
				arg = atol(argv[i]);
				ret = ioctl(fd, AXISDATA_IOC_HPM, arg);
				if (ret < 0) {
					perror("AXISDATA_IOC_HPM");
					return -1;
				}
			}
			bexit = 1;
		}
	}


	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--set-rate") == 0) {
			if (++i < argc) {
				arg = atol(argv[i]);
				ret = ioctl(fd, AXISDATA_IOC_SETRATE, arg);
				if (ret < 0) {
					perror("AXISDATA_IOC_SETRATE");
					return -1;
				}
			}
			bexit = 1;
		}
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--set-scale") == 0) {
			if (++i < argc) {
				arg = atol(argv[i]);
				ret = ioctl(fd, AXISDATA_IOC_SETSCALE, arg);
				if (ret < 0) {
					perror("AXISDATA_IOC_SETSCALE");
					return -1;
				}
			}
			bexit = 1;
		}
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--get-scale") == 0) {
			ret = ioctl(fd, AXISDATA_IOC_GETSCALE, 0);
			if (ret < 0) {
				perror("AXISDATA_IOC_GETSCALE");
				return -1;
			} else {
				fprintf(stdout, "%d\n", ret);
			}
			return 1;
		}
	}
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--get-rate") == 0) {
			ret = ioctl(fd, AXISDATA_IOC_GETRATE, 0);
			if (ret < 0) {
				perror("AXISDATA_IOC_GETRATE");
				return -1;
			} else {
				fprintf(stdout, "%d\n", ret);
			}
			return 1;
		}
	}

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--set-mode") == 0) {
			if (++i < argc) {
				arg = atol(argv[i]);
				ret = ioctl(fd, AXISDATA_IOC_SETMODE, arg);
				if (ret < 0) {
					perror("AXISDATA_IOC_SETMODE");
					return -1;
				}
			}
			bexit = 1;
		}
	}

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--get-mode") == 0) {
			ret = ioctl(fd, AXISDATA_IOC_GETMODE, 0);
			if (ret < 0) {
				perror("AXISDATA_IOC_GETMODE");
				return -1;
			} else {
				fprintf(stdout, "%d\n", ret);
			}
			return 1;
		}
	}


	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--set-watermark") == 0) {
			if (++i < argc) {
				arg = atol(argv[i]);
				ret = ioctl(fd, AXISDATA_IOC_SETWATERMARK, arg);
				if (ret < 0) {
					perror("AXISDATA_IOC_SETWATERMARK");
					return -1;
				}
			}
			bexit = 1;
		}
	}

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--get-watermark") == 0) {
			ret = ioctl(fd, AXISDATA_IOC_GETWATERMARK, 0);
			if (ret < 0) {
				perror("AXISDATA_IOC_GETWATERMARK");
				return -1;
			} else {
				fprintf(stdout, "%d\n", ret);
			}
			return 1;
		}
	}

	return bexit;
}


static int axesdata_open_device(const char *devname)
{
	int fd;
	char servername[256];
	struct sockaddr_in server_addr;
	int serverport = 0;
	struct hostent *host = NULL;

	memset(&server_addr, 0, sizeof(server_addr));
	if (strncmp(devname, AXIS_NET_PROT, strlen(AXIS_NET_PROT)) == 0) {
		const char *name = devname + strlen(AXIS_NET_PROT);
		const char *col = strchr(name, ':');
		memset(servername, 0, sizeof(servername));
		if (col && (col - name) + 1 < (long) sizeof(servername)) {
			strncpy(servername, name, (col - name));
			serverport = atoi(col + 1);
		} else {
			strcpy(servername, name);
		}
		host = gethostbyname(servername);
		if (!host) {
			perror("gethost by name");
			exit(1);
		}
		if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			perror("Socket");
			return -1;
		}
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(serverport);
		server_addr.sin_addr = *((struct in_addr *)host->h_addr);
		if (connect(fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
			perror("Connect");
			close(fd);
			return -1;
		}
		return fd;
	} else {
		fd = open(g_devname, O_RDONLY);
		if (fd < 0) {
			perror("Open");
			return -1;
		}
	}
	return fd;
}


int main(int argc, const char *argv[])
{
	char buffer[256];
	short3d_t raw[GYRO_BUFFER_SIZE];
	int fd;
	FILE *file = NULL, *output = stdout;
	int i, ret, count;
	short3d_t *framebuf;
	int frameptr = 0;
	unsigned int step = 0;

	if (axesdata_parse_commandline(argc, argv) < 0)
		return 1;

	step = g_framesize/7 ? g_framesize/7 : 1;
	framebuf = malloc(sizeof(short3d_t)*g_framesize);
	memset(framebuf, 0, sizeof(short3d_t)*g_framesize);
	fd = axesdata_open_device(g_devname);
	if (fd < 0)
		return 1;
	if (axesdata_apply_commandline(fd, argc, argv) != 0)
		return 2;
	if (g_textdata) {
		file = fdopen(fd, "r");
		if (!file) {
			fprintf(stderr, "ERROR: %d, Can't open device: %s\n", errno, g_devname);
			return 1;
		}
	}
	if (g_gnuplot) {
		output = popen("gnuplot", "w");
		if (!output) {
			perror("gnuplot");
			return 1;
		}
		axesdata_int_gnuplot(output);
	}
	memset(&raw, 0, sizeof(raw));
	while (1) {
		if (g_textdata) {
			if (!fgets(buffer, sizeof(buffer), file)) {
				break;
			}
			sscanf(buffer, "%hd %hd %hd", &raw[0].x, &raw[0].y, &raw[0].z);
			ret = sizeof(raw[0]);
		} else {
			ret = read(fd, &raw, sizeof(raw));
			if (ret < 0) {
				return 1;
			}
		}
		if (ret > 0) {
			count = ret / sizeof(raw[0]);
			for (i = 0; i < count; i++) {
				if (g_gnuplot) {
					framebuf[frameptr] = raw[i];
					frameptr = (frameptr + 1) % g_framesize;
				} else {
					fprintf(output, "%7d %7d %7d\n", raw[i].x, raw[i].y, raw[i].z);
				}
				if (g_delay) {
					usleep(g_delay * 1000);
				}
			}
			fflush(output);
			if (g_gnuplot && (frameptr % step) == 0) {
				axesdata_update_gnuplot(output, framebuf, frameptr);
			}
		}
	}

	return 0;
}

static void axesdata_int_gnuplot(FILE *gnuplot)
{
	fprintf(gnuplot, "set yrange [-32768:32767]\n");
	fprintf(gnuplot, "set ylabel \"DPS\"\n");
}

static void axesdata_update_gnuplot(FILE *gnuplot, short3d_t *framebuf, int frameptr)
{
	int i;

	fprintf(gnuplot, "plot \"-\" using 1 title 'X' with line");
	fprintf(gnuplot, ", \"-\" using 2 title 'Y' with line");
	fprintf(gnuplot, ", \"-\" using 3 title 'Z' with line");
	fprintf(gnuplot, "\n");

	for (i = 0; i < g_framesize; i += 1) {
		fprintf(gnuplot, "%7d %7d %7d\n",
				framebuf[(frameptr + i) % g_framesize].x,
				framebuf[(frameptr + i) % g_framesize].y,
				framebuf[(frameptr + i) % g_framesize].z);
	}
	fprintf(gnuplot, "e\n");

	for (i = 0; i < g_framesize; i += 1) {
		fprintf(gnuplot, "%7d %7d %7d\n",
				framebuf[(frameptr + i) % g_framesize].x,
				framebuf[(frameptr + i) % g_framesize].y,
				framebuf[(frameptr + i) % g_framesize].z);
	}
	fprintf(gnuplot, "e\n");

	for (i = 0; i < g_framesize; i += 1) {
		fprintf(gnuplot, "%7d %7d %7d\n",
				framebuf[(frameptr + i) % g_framesize].x,
				framebuf[(frameptr + i) % g_framesize].y,
				framebuf[(frameptr + i) % g_framesize].z);
	}
	fprintf(gnuplot, "e\n");
	fflush(gnuplot);
}

