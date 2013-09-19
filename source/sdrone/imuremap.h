/*
 * ImuRemap.h
 *
 *  Created on: August 16, 2013
 *      Author: svassilev
 */

#ifndef IMUREMAP_H_
#define IMUREMAP_H_

#include "commoninc.h"
#include "imudevice.h"

/*
 * Reads data from all the sensor devices
 * Should be able to read from a file
 */
class ImuRemap: public IPlugin
{
public:
	ImuRemap();
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
	~ImuRemap();

private:
	Matrix3d m_AccelMap;
	Matrix3d m_GyroMap;
	Matrix3d m_MagMap;
	Vector3d m_AccelData;
	Vector3d m_GyroData;
	Vector3d m_MagData;
	IPluginRuntime* m_Runtime;
	int m_RefCnt;
};

#endif /* IMUREMAP_H_ */
