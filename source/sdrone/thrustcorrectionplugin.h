/*
 * thrustcorrectionplugin.h
 *
 *  Created on: Oct 23, 2014
 *      Author: svassilev
 */

#ifndef SDRONE_THRUSTCORRECTIONPLUGIN_H_
#define SDRONE_THRUSTCORRECTIONPLUGIN_H_

#include "commoninc.h"

class ThrustCorrectionPlugin : public IPlugin {
public:
	ThrustCorrectionPlugin();
	int AttachToChain(void*,SdPluginAttachFunc attachPlugin);
	int AddRef();
	int Release();
	int ExecuteCommand(SdCommandParams* params);
	const char* GetName();
	SdDeviceId GetDeviceId();
	const char* GetVersion();
	const char* GetDlFileName();
	int IoCallback(SdIoPacket* ioPacket);
	int IoDispatchThread();
	int Start(const SdDroneConfig* droneConfig);

private:
	virtual ~ThrustCorrectionPlugin();

private:
	Vector4d m_thrustCorrection;
	IPluginRuntime* m_runtime;
	int m_refCnt;
};

#endif /* SDRONE_THRUSTCORRECTIONPLUGIN_H_ */
