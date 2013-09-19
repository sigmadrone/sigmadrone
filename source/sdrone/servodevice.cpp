/*
 * servodvice.cpp
 *
 *  Created on: May 26, 2013
 *      Author: svassilev
 */

#include "servodevice.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <math.h>
#include "pca9685.h"

ServoDevice::ServoDevice()
{
	m_Fd = -1;
	m_ChannelMask = 0;
	m_File = 0;
	m_MaxValue = 0;
	m_OneMsInterval = 0;
	m_Rate = 0;
	m_RefCnt = 1;
	m_Runtime = 0;
}

ServoDevice::~ServoDevice()
{
	assert(0 == m_RefCnt);
	Close();
}

void ServoDevice::Close()
{
	if (-1 != m_Fd) {
		Stop(0);
		close(m_Fd);
		m_Fd = -1;
	}
	if (0 != m_File && stdout != m_File) {
		fclose(m_File);
		m_File = 0;
	}
}

int ServoDevice::Start(CommandArgs* cmdArgs)
{
	const SdDroneConfig* droneConfig = cmdArgs->GetSdDroneConfig();
	const SdServoConfig* config = &droneConfig->Servo;
	int err = EINVAL;

	Close();

	m_MaxValue = (config->BitCount > 0) ? (1 << config->BitCount) : (1 << 12);
	m_ChannelMask = config->ChannelMask;

	if (0 != strncmp(config->DeviceName,"/dev/", strlen("/dev/"))) {
		if (0 == strcmp(config->DeviceName,"stdout")) {
			m_File = stdout;
		} else if (0 == (m_File = fopen(config->DeviceName,"w"))) {
			fprintf(stdout,"ServoDevice::Init failed to open %s, err=%d\n",
					config->DeviceName,errno);
			err = -1;
			goto __return;
		}
		fprintf(stdout,"ServoDevice is operating in text mode.\n");
	} else if (-1 == (m_Fd = open(config->DeviceName,O_RDWR|O_CREAT))) {
		fprintf(stdout,"ServoDevice::Init failed to open %s, err=%d\n",
				config->DeviceName,errno);
		err = -1;
		goto __return;
	}

	fprintf(stdout,"ServoDevice opened %s\n",config->DeviceName);
	fprintf(stdout,"ServoDevice channel mask 0x%x\n",config->ChannelMask);

	if (0 == (err = SetRate((config->Rate > 0) ? config->Rate : 50))) {
		/*
		 * Reset all the available channels
		 */
		printf("ServoDevice resetting all available channels...\n");
		uint32_t mask = config->ChannelMask;
		for (size_t i = 0; i < sizeof(mask)*8 && 0 != mask; i++) {
			if (!!(mask & 0x1)) {
				SetPulse((int)i,0.0);
			}
			mask >>= 1;
		}
		printf("ServoDevice - all channels are armed and ready!\n");
	}

	if (0 == err) {
		/*
		 * Tell the runtime we are interested in writes towards our
		 * own device
		 */
		m_Runtime->SetIoFilters(SD_DEVICEID_TO_FLAG(SD_DEVICEID_SERVO),
				SD_IOCODE_TO_FLAG(SD_IOCODE_SEND));
	}

	__return:
	return err;
}

void ServoDevice::Stop(int flags)
{
	// disarm the motors
	if (-1 != m_Fd) {
		uint32_t mask = m_ChannelMask;
		for (size_t i = 0; i < sizeof(mask)*8 && 0 != mask; i++) {
			if (!!(mask & 0x1)) {
				printf("Disarming servo channel %d\n",(int)i);
				SetPulse((int)i,m_MaxValue/2,m_MaxValue/2);
			}
			mask >>= 1;
		}
	}
	if (!!(flags&FLAG_STOP_AND_DETACH)) {
		m_Runtime->DetachPlugin();
	}
}

int ServoDevice::AttachToChain(
		void* droneContext,
		SdPluginAttachFunc attachFunc)
{
	int err = attachFunc(
			droneContext,
			this,
			SD_ALTITUDE_GROUP_DEVICE,
			0,
			0,
			&m_Runtime);
	return err;
}

int ServoDevice::AddRef()
{
	return __sync_fetch_and_add(&m_RefCnt,1);
}

int ServoDevice::Release()
{
	int refCnt = __sync_sub_and_fetch(&m_RefCnt,1);
	if (0 == refCnt) {
		delete this;
	}
	return refCnt;
}

const char* ServoDevice::GetName()
{
	return SD_PLUGIN_SERVO_PCA9685;
}

SdDeviceId ServoDevice::GetDeviceId()
{
	return SD_DEVICEID_SERVO;
}

const char* ServoDevice::GetVersion()
{
	return SD_PLUGIN_VERSION;
}

extern char **g_Argv;
const char* ServoDevice::GetDlFileName()
{
	return g_Argv[0];
}

int ServoDevice::IoCallback(
	SdIoPacket* ioPacket)
{
	if (ioPacket->inData.dataType != SdIoData::TYPE_SERVO ||
		0 == ioPacket->inData.asServoData) {
		return EINVAL;
	}
	const SdServoIoData* servoData =ioPacket->inData.asServoData;
	assert(servoData->numChannels <= sizeof(servoData->channels));
	for (uint32_t i = 0; i < servoData->numChannels; i++) {
		if (SetPulse(servoData->channels[i],servoData->value[i]) < 0) {
			return errno;
		}
	}
	return SD_ESUCCESS;
}

int ServoDevice::IoDispatchThread()
{
	assert(false);
	return EINVAL;
}

int ServoDevice::SetPulse(
	int channel,
	double pulseWidth)
{
	int on = m_MaxValue/2 - m_OneMsInterval;
	int off = m_MaxValue/2;
	int err = 0;
	pulseWidth = fmin(fmax(0,pulseWidth),1.0);
	off += (int)(pulseWidth * (double)m_OneMsInterval);
	err = SetPulse(channel,on,off);
	if (err < 0) {
		fprintf(stdout,"ServoDevice::SetPulse channel %d %f, failed err=%d\n",
			channel,pulseWidth,errno);
	}
	return err;
}

int ServoDevice::SetPulse(
	int channel,
	int on,
	int off)
{
	int err = 0;
	if (IsInTextMode()) {
		double pulseWidth = (double)(off-on-m_OneMsInterval)/m_OneMsInterval;
		fprintf(m_File,"Channel %d: Pulse %.4f ON %4d OFF %4d\n",
				channel, pulseWidth, on, off);
		fflush(m_File);
	} else {
		err = ioctl(m_Fd, PCA9685_IOC_SETLED0 + channel, PCA9685_PULSE(on, off));
	}
	return err;
}

int ServoDevice::GetPulse(
	int channelNo,
	double* pulseWidth)
{
	int on = 0;
	int off = 0;
	int ret = 0;

	assert(pulseWidth);
	if (IsInTextMode()) {
		return ENOTTY;
	}
	if ((ret = ioctl(m_Fd,PCA9685_IOC_GETLED0,0)) < 0) {
		fprintf(stdout,"ServoDevice::GetPulse failed, err=%d\n",errno);
		return ret;
	}
	assert(pulseWidth);
	*pulseWidth = 0;
	on = ret>>16;
	off = ret & 0xffff;
	if (off > on + m_OneMsInterval) {
		*pulseWidth = (double)(off - on - m_OneMsInterval) / m_OneMsInterval;
	}
	return ret;
}

int ServoDevice::SetRate(int freq)
{
	int err = EINVAL;
	double numOneMsIntervals;

	assert(freq > 0);
	numOneMsIntervals = (1.0 / (double)freq) / 0.001;
	if (numOneMsIntervals < 3) {
		fprintf(stdout, "ServoDevice::SetRate - rate too high %d\n",freq);
		return err;
	}

	err = IsInTextMode() ? 0 : ioctl(m_Fd, PCA9685_IOC_SETRATE, freq);

	if (0 == err) {
		m_Rate = freq;
		m_OneMsInterval = (int)(double)m_MaxValue/numOneMsIntervals;
		assert(m_MaxValue/2 - m_OneMsInterval > 0);
		fprintf(stdout, "ServoDevice rate %d, PWMmin = %d, PWMmax = %d\n",
				freq, m_OneMsInterval, 2*m_OneMsInterval);
	} else {
		fprintf(stdout,"ServoDevice::SetRate failed, err=%d\n",errno);
	}
	return err;
}

int ServoDevice::GetRate(int* freq)
{
	assert(freq);
	*freq = IsInTextMode() ? m_Rate : ioctl(m_Fd,PCA9685_IOC_GETRATE,0);
	return (*freq > 0) ? 0 : *freq;
}
