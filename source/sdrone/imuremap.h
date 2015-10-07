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

#ifndef IMUREMAP_H_
#define IMUREMAP_H_

#include "commoninc.h"

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
	int ExecuteCommand(SdCommandParams* params);
const char* GetName();
	SdDeviceId GetDeviceId();
	const char* GetVersion();
	const char* GetDlFileName();
	int IoCallback(SdIoPacket* ioPacket);
	int IoDispatchThread();
	int Start(const SdDroneConfig* droneConfig);

private:
	~ImuRemap();

private:
	Matrix3d m_AccelMap;
	Matrix3d m_GyroMap;
	Matrix3d m_MagMap;
	IPluginRuntime* m_Runtime;
	int m_RefCnt;
};

#endif /* IMUREMAP_H_ */
