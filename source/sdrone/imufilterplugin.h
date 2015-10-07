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

#ifndef IMUFILTERPLUGIN_H_
#define IMUFILTERPLUGIN_H_

#include "commoninc.h"
#include "imufilter.h"
#include "attitudetracker.h"

class ImuFilterPlugin : public IPlugin{
public:
	ImuFilterPlugin();
	int AttachToChain(void*,SdPluginAttachFunc attachPlugin);

	int AddRef();
	int Release();
	int ExecuteCommand(SdCommandParams* commandArgs);
	const char* GetName();
	SdDeviceId GetDeviceId();
	const char* GetVersion();
	const char* GetDlFileName();

	/*
	 * Takes into account the IMU readings and comes with an estimate
	 * of the current attite
	 */
	int IoCallback(SdIoPacket* ioPacket);

	int IoDispatchThread() { assert(false); return EINVAL; }

private:
	virtual ~ImuFilterPlugin();
private:
	attitudetracker m_attitude;
	IPluginRuntime* m_Runtime;
	int m_RefCnt;
};

#endif /* IMUFILTERPLUGIN_H_ */
