/*
 * servodevice.h
 *
 *  Created on: May 18, 2013
 *      Author: svassilev
 */

#ifndef SERVODEVICE_H_
#define SERVODEVICE_H_

#include "commoninc.h"

class ServoDevice : public IPlugin
{
public:
	ServoDevice();

	int AttachToChain(void*,SdPluginAttachFunc attachPlugin);

	int AddRef();
	int Release();
	int ExecuteCommand(SdCommandParams* commandArgs);
	const char* GetName();
	SdDeviceId GetDeviceId();
	const char* GetVersion();
	const char* GetDlFileName();
	int IoCallback(SdIoPacket* ioPacket);
	int IoDispatchThread();


	/*
	 *  Writes pulse to the specified channel.
	 *  pulseWidth specifies relative range 0..1
	 */
	int SetPulse(
			int channelNo,
			double pulseWidth
			);
	int SetPulse(
			int channelNo,
			int on,
			int off
			);
	int GetPulse(
			int channelNo,
			double* pulseWidth
			);
	int SetRate(
			int freq
			);
	int GetRate(
			int* freq
			);

	inline bool IsInTextMode() { return !!m_File; }

private:
	void Close();
	~ServoDevice();
	int Start(const SdDroneConfig* config);
	void Stop(bool detach);
	int TurnPowerOn();
	int TurnPowerOff();

private:
	int m_Fd;
	uint32_t m_ChannelMask;
	int m_MaxValue;
	int m_OneMsInterval;
	int m_Rate;
	int m_RefCnt;
	FILE* m_File;
	IPluginRuntime* m_Runtime;
};


#endif /* SERVODEVICE_H_ */
