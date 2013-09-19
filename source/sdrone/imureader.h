/*
 * imureader.h
 *
 *  Created on: August 16, 2013
 *      Author: svassilev
 */

#ifndef IMUREADER_H_
#define IMUREADER_H_

#include "commoninc.h"
#include "imudevice.h"

/*
 * Reads data from all the sensor devices
 * Should be able to read from a file
 */
class ImuReader: public IPlugin
{
public:
	ImuReader();
	int AttachToChain(void*,SdPluginAttachFunc attachPlugin);

public:
	int AddRef();
	int Release();
	int Start(CommandArgs*);
	void Stop(int flags);
	const char* GetName();
	SdDeviceId GetDeviceId();
	const char* GetVersion();
	const char* GetDlFileName();
	int IoCallback(SdIoPacket* ioPacket);
	int IoDispatchThread();

private:
	inline bool IsInTextMode() { return !!m_File; }
	double DeltaT();
	void Close();
	~ImuReader();

private:
	ImuDevice m_AccDevice;
	ImuDevice m_MagDevice;
	ImuDevice m_GyroDevice;
	SdImuData m_ImuData;
	Vector3d m_AccelData;
	Vector3d m_GyroData;
	Vector3d m_MagData;
	IPluginRuntime* m_RunTime;
	int m_RefCnt;
	FILE* m_File;
	timespec m_LastTime;
};

#endif /* IMUREADER_H_ */
