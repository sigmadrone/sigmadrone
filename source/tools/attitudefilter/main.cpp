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
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
using namespace std;

#include "matrix.h"
#include "axesdata.h"
#include "attitudefilter.h"

typedef struct alldata_s {
	short3d_t gyro;
	short3d_t accel;
	short3d_t mag;
	int3d_t gyrV;
	int3d_t gyrS;
} alldata_t;

int FPS = 80;
int FS_HZ = 200;
int GYRO_MAX_DPS = 575;
int MAX_ADC = 32768;
int ACC_MAX_G = 4;
int ACC_ONE_G = MAX_ADC/ACC_MAX_G;
float Gx, Gy, Gz;

typedef struct _imu_params {
	int Fs;
	int GyroMaxDps;
	int GyroMaxAdc;
	int AccMaxG;
	int AccMaxAdc;
} imu_params_t;

static bool read_sensor_data(
		short3d_t* acc,
		short3d_t* gyro,
		short3d_t* mag)
{
	bool ret = false;
	char buffer[256];
	if (fgets(buffer, sizeof(buffer), stdin)) {
		sscanf(buffer, "%hd %hd %hd %hd %hd %hd %hd %hd %hd",
				&acc->x, &acc->y, &acc->z,
				&gyro->y, &gyro->x, &gyro->z,
				&mag->x, &mag->y, &mag->z);
		gyro->x = -gyro->x;
		gyro->z = -gyro->z;
		ret = true;
	}
	return ret;
}

static void fc_calculate_bias(
		int3d_t* gyroBias,
		int3d_t* accBias,
		int numSamples)
{
	int i;
	short3d_t accel, gyro, mag;
	memset(gyroBias, 0, sizeof(*gyroBias));
	memset(accBias, 0, sizeof(*accBias));
	for (i = 0; i < numSamples; i++) {
		if (read_sensor_data(&accel,&gyro,&mag)) {
			gyroBias->x += gyro.x;
			gyroBias->y += gyro.y;
			gyroBias->z += gyro.z;
			accBias->x += accel.x;
			accBias->y += accel.y;
			accBias->z += accel.z;
		}
	}
	gyroBias->x /= numSamples;
	gyroBias->y /= numSamples;
	gyroBias->z /= numSamples;

	accBias->x /= numSamples;
	accBias->y /= numSamples;
	accBias->z /= numSamples;

	if (abs(accBias->x) > ACC_ONE_G / 2) {
		if (accBias->x > ACC_ONE_G / 2) {
			accBias->x = ACC_ONE_G - accBias->x;
		} else  {
			accBias->x = ACC_ONE_G + accBias->x;
		}
	} else if (abs(accBias->y) > ACC_ONE_G / 2) {
		if (accBias->y > ACC_ONE_G / 2) {
			accBias->y = ACC_ONE_G - accBias->y;
		} else {
			accBias->y = ACC_ONE_G + accBias->y;
		}
	} else if (abs(accBias->z) > ACC_ONE_G / 2) {
		if (accBias->z > ACC_ONE_G / 2) {
			accBias->z = ACC_ONE_G - accBias->z;
		} else {
			accBias->z = ACC_ONE_G + accBias->z;
		}
	}
}

#define _WHANT_FPE_

#ifdef _WHANT_FPE_
#include <fenv.h>
#endif

int main(int argc, char *argv[])
{
	int3d_t gyroBias;
	int3d_t accBias;
	unsigned int frame;
	Matrix4d rotMx = Matrix3f::createIdentityMatrix();
	Vector3d X(1, 0, 0), Y(0, 1, 0), Z(0, 0, 1);
	AttitudeFilter attiFilt(
			FS_HZ,
			GYRO_MAX_DPS,
			MAX_ADC,
			0.9,
			0.001,
			0.00001);

#ifdef _WHANT_FPE_
	feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
#endif

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--help") == 0) {
			fprintf(stdout, "Usage: \n");
			fprintf(stdout, "%s <options>\n", argv[0]);
			fprintf(stdout, "	--help                 Display this help and exit\n");
			fprintf(stdout, "	--host HOSTNAME        Connect to HOSTNAME using TCP/IP port 5555\n");
			fprintf(stdout, "	--maxdps DPS           Gyro max angular speed in deg/s\n");
			fprintf(stdout, "	--file FILENAME        File name\n");
			return 0;
		} else if (strcmp(argv[i], "--fps") == 0) {
			if (++i < argc) {
				FPS = atoi(argv[i]);
			}
		} else if (strcmp(argv[i], "--maxdps") == 0) {
			if (++i < argc) {
				GYRO_MAX_DPS = atoi(argv[i]);
			}
		} else if (strcmp(argv[i], "--host") == 0) {
			if (++i < argc) {
				char *hostname = argv[i];
				struct hostent *host;
				struct sockaddr_in addr;
				int fd;

				memset(&addr, 0, sizeof(addr));
				if ((host = gethostbyname(hostname)) == NULL) {
					perror("gethostbyname error");
					return 1;
				}
				if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
					perror("Socket");
					return 2;
				}
				addr.sin_family = AF_INET;
				addr.sin_port = htons(5555);
				addr.sin_addr = *((struct in_addr *)host->h_addr);
				if (connect(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) == -1) {
					perror("Connect");
					close(fd);
					return 3;
				}
				stdin = fdopen(fd, "r");
				if (!stdin) {
					perror("fdopen");
					return 4;
				}
			}
		} else if (strcmp(argv[i], "--file") == 0) {
			if (++i < argc) {
				FILE* f = 0;
				f = fopen(argv[i],"r");
				stdin = f;
			}
		}
	}

	fc_calculate_bias(&gyroBias,&accBias,FS_HZ);
	attiFilt.SetBias(gyroBias,accBias);

	short3d_t accRaw;
	short3d_t gyroRaw;
	short3d_t magRaw;
	while (1)
	{
		if (read_sensor_data(&accRaw,&gyroRaw,&magRaw))
		{
			QuaternionD attQ;

			attiFilt.Update(gyroRaw,accRaw,magRaw);
			attQ = attiFilt.GetAttitude();

			X = attQ.rotate(Vector3d(1, 0, 0));
			Y = attQ.rotate(Vector3d(0, 1, 0));
			Z = attQ.rotate(Vector3d(0, 0, 1));

			if ((++frame % (FS_HZ/FPS)) == 0) {
				rotMx = attQ.rotMatrix4();
				fprintf(stdout, "%5.9lf %5.9lf %5.9lf %5.9lf %5.9lf %5.9lf %5.9lf %5.9lf %5.9lf \n",
						rotMx.at(0, 0), rotMx.at(0, 1), rotMx.at(0, 2),
						rotMx.at(1, 0), rotMx.at(1, 1), rotMx.at(1, 2),
						rotMx.at(2, 0), rotMx.at(2, 1), rotMx.at(2, 2));
				cout << "--> X : " << X.transpose();
				cout << "--> Y : " << Y.transpose();
				cout << "--> Z : " << Z.transpose();
				fflush(stdout);
			}
		} else {
			QuaternionD attQ = attiFilt.GetAttitude();
			X = attQ.rotate(Vector3d(1, 0, 0));
			Y = attQ.rotate(Vector3d(0, 1, 0));
			Z = attQ.rotate(Vector3d(0, 0, 1));
			fprintf(stdout,"IMU accel bias  : %3d %3d %3d\n",accBias.x,accBias.y,accBias.z);
			fprintf(stdout,"IMU gyro  bias  : %3d %3d %3d\n",gyroBias.x,gyroBias.y,gyroBias.z);
			fprintf(stdout,"Final Quaternion: %5.9lf %5.9lf %5.9lf %5.9lf\n",
							attQ.w,attQ.x,attQ.y,attQ.z);
			break;
		}
	}

	return 0;
}
