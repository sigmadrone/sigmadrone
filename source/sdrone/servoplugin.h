/*
 * servodevice.h
 *
 *  Created on: May 18, 2013
 *      Author: svassilev
 */

#ifndef SERVODEVICE_H_
#define SERVODEVICE_H_

#include "commoninc.h"
#include "servocontroller.h"
#include <boost/scoped_ptr.hpp>

class ServoDevicePlugin : public IPlugin
{
public:
	ServoDevicePlugin();

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

private:
	void Close();
	~ServoDevicePlugin();
	int Start(const SdDroneConfig* config);
	void Stop(bool detach);

private:
	int ref_cnt_;
	boost::scoped_ptr<servocontroller> servoctrl_;
	IPluginRuntime* runtime_;
};


#endif /* SERVODEVICE_H_ */
