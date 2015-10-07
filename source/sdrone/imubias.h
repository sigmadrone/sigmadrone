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

#ifndef IMUBIAS_H
#define IMUBIAS_H

#include "commoninc.h"

/*
 * Reads data from all the sensor devices
 * Should be able to read from a file
 */
class ImuBias: public IPlugin
{
public:
	ImuBias();
	int AttachToChain(void*,SdPluginAttachFunc attachPlugin);

public:
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
	~ImuBias();
	int Start(const SdDroneConfig* droneConfig);

private:
	Vector3d m_EarthG;
	Vector3d m_GyroBias;
	uint32_t m_CurrentBiasSamples;
	double m_ElapsedTime;
	double m_TotalBiasCalcTime;
	IPluginRuntime* m_Runtime;
	int m_RefCnt;
};

#endif /* IMUBIAS_H */
