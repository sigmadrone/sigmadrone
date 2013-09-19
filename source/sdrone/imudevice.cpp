/*
 * imudevice.cpp
 *
 *  Created on: May 25, 2013
 *      Author: svassilev
 */

#include "commoninc.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "imudevice.h"

ImuDevice::ImuDevice()
{
	m_Fid = s_InvalidFileId;
	m_Counter = 0;
	m_ReadDataThrottle = 1;
	m_DeviceId = SD_IMU_DEVICE_UNKNOWN;
	memset(&m_LastData,0,sizeof(m_LastData));
	m_DeviceName = 0;
}

ImuDevice::~ImuDevice()
{
	Close();
}

int ImuDevice::Open(
	const char* deviceName,
	SdImuDeviceId deviceId)
{
	int err = EINVAL;

	assert(deviceName);

	Close();

	m_Fid = open(deviceName,O_RDONLY);
	m_DeviceName = strdup(deviceName);
	m_DeviceId = deviceId;

	if (s_InvalidFileId == m_Fid) {
		char servername[256];
		struct sockaddr_in server_addr;
		int serverport = 0;
		struct hostent *host = NULL;
		memset(&server_addr, 0, sizeof(server_addr));
		if (strncmp(deviceName, DRONE_AXIS_NET_PROT, strlen(DRONE_AXIS_NET_PROT)) == 0) {
			const char *name = deviceName + strlen(DRONE_AXIS_NET_PROT);
			const char *col = strchr(name, ':');
			memset(servername, 0, sizeof(servername));
			if (!!col && (size_t)((col - name) + 1) < sizeof(servername)) {
				strncpy(servername, name, (col - name));
				serverport = atoi(col + 1);
			} else {
				strcpy(servername, name);
			}
			host = gethostbyname(servername);
			if (!host) {
				fprintf(stdout,"ImuDevice::Open gethost by name %s failed, err=%d\n",
					servername,errno);
				goto __return;
			}
			if ((m_Fid = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
				fprintf(stdout,"ImuDevice::Open socket failed, err=%d\n",errno);
				goto __return;
			}
			server_addr.sin_family = AF_INET;
			server_addr.sin_port = htons(serverport);
			server_addr.sin_addr = *((struct in_addr *)host->h_addr);
			if (connect(m_Fid, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1) {
				fprintf(stdout,"ImuDevice::Open, connect failed, err=%d\n",errno);
				goto __return;
			}
		}
	}

	err = 0;

	__return:

	if (0 == err) {
		fprintf(stdout,"ImuDevice %s opened\n",deviceName);
	}
	else {
		err = errno;
		fprintf(stdout,"ImuDevice::Open(%s) failed, err=%d\n",deviceName,errno);
	}

	return err;
}

void ImuDevice::Close()
{
	if (s_InvalidFileId != m_Fid) {
		close(m_Fid);
		m_Fid = s_InvalidFileId;
	}
	if (0 != m_DeviceName) {
		free(m_DeviceName);
		m_DeviceName = 0;
	}
	m_DeviceId = SD_IMU_DEVICE_UNKNOWN;
}

int ImuDevice::ResetFifo()
{
	int err = ioctl(m_Fid,AXISDATA_IOC_RESETFIFO,0);
	if (err < 0) {
		printf("ERROR: AXISDATA_IOC_RESETFIFO %s failed, err=%d\n",
				m_DeviceName, errno);
	}
	return err;
}

int ImuDevice::SetRate(int rate)
{
	int err = ioctl(m_Fid,AXISDATA_IOC_SETRATE,rate);
	if (err < 0) {
		printf("ERROR: AXISDATA_IOC_SETRATE %s failed, err=%d\n",
			m_DeviceName, errno);
	}
	return err;
}

int ImuDevice::GetRate(int* rate)
{
	*rate = ioctl(m_Fid,AXISDATA_IOC_GETRATE);
	if (*rate < 0) {
		printf("ERROR: AXISDATA_IOC_GETRATE %s failed, err=%d\n",
				m_DeviceName, errno);
		return -1;
	}
	return 0;
}

int ImuDevice::SetScale(int scale)
{
	int err = ioctl(m_Fid,AXISDATA_IOC_SETSCALE, scale);
	if (err < 0) {
		printf("ERROR: AXISDATA_IOC_SETSCALE %s failed, scale %d, err=%d\n",
				m_DeviceName, scale, errno);
	}
	return err;
}

int ImuDevice::GetScale(int* scale)
{
	*scale = ioctl(m_Fid,AXISDATA_IOC_GETSCALE,0);
	if (*scale < 0) {
		printf("ERROR: AXISDATA_IOC_GETSCALE %s failed, err=%d\n",
				m_DeviceName, errno);
		return -1;
	}
	return 0;
}

int ImuDevice::Enable(int enable)
{
	int err = ioctl(m_Fid,AXISDATA_IOC_SETENABLED,enable);
	if (err < 0) {
		printf("ERROR: AXISDATA_IOC_SETENABLED %s failed, err=%d\n",
					m_DeviceName, errno);
	}
	return err;
}

bool ImuDevice::IsEnabled()
{
	return (0 == ioctl(m_Fid,AXISDATA_IOC_GETENABLED,0) ? true : false);
}

/*
 * Return the number of records read, or negative
 * in case of error
 */
int ImuDevice::ReadData(short3d_t* data, size_t size)
{
	int ret = 0;
	if (0 == (m_Counter++ % m_ReadDataThrottle)) {
		if ((ret = read(m_Fid, data, size)) < 0) {
			fprintf(stdout,"ImuDevice::ReadData(%s) failed, err=%d\n",
				m_DeviceName, errno);
			return ret;
		} else if (ret == 0) {
			return 0;
		}
		m_LastData = data[(ret - 1)/sizeof(short3d_t)];
	} else {
		*data = m_LastData;
		ret = sizeof(short3d_t);
	}
	return ret/sizeof(short3d_t);
}
