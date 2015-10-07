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
