/*
 * navigator.h
 *
 *  Created on: Aug 18, 2013
 *      Author: svassilev
 */

#ifndef NAVIGATOR_H_
#define NAVIGATOR_H_

#include "commoninc.h"

class Navigator: public IPlugin
{
public:
	Navigator();
	int AttachToChain(void*,SdPluginAttachFunc attachPlugin);

	int AddRef();
	int Release();
	int Start(CommandArgs*);
	void Stop(int flags);
	const char* GetName();
	SdDeviceId GetDeviceId();
	const char* GetVersion();
	const char* GetDlFileName();

	/*
	 * Takes into account the current attitude and comes with
	 * an estimate for the _new_ target attitude Q.
	 */
	int IoCallback(SdIoPacket* ioPacket);

	int IoDispatchThread() { assert(false); return EINVAL; }

private:
	virtual ~Navigator();

private:
	QuaternionD m_TargetAttitude;
	IPluginRuntime* m_Runtime;
	int m_RefCnt;
};

#endif /* NAVIGATOR_H_ */
