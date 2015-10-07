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

#ifndef TRACELOG_H
#define TRACELOG_H

#include "commoninc.h"

/*
 * Creates a trace log, attaches to the SD_DEVICE_ID_IMU chain
 */
class TraceLogPlugin: public IPlugin
{
public:
	TraceLogPlugin();
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
	~TraceLogPlugin();

private:
	uint32_t m_iteration;
	uint32_t m_logLevel;
	int m_logRotMatrix;
	double m_logPeriod;
	double m_timeAfterLastLog;
	IPluginRuntime* m_runtime;
	int m_refCnt;
};

#endif /* TRACELOG_H */
