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

#ifndef IMUREADER_H_
#define IMUREADER_H_

#include <boost/scoped_ptr.hpp>
#include "commoninc.h"
#include "sampler.h"
#include "filesampler.h"

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
	int Run(const SdDroneConfig* config);
	const char* GetName();
	SdDeviceId GetDeviceId();
	const char* GetVersion();
	const char* GetDlFileName();
	int IoCallback(SdIoPacket* ioPacket);
	int IoDispatchThread();
	int ExecuteCommand(SdCommandParams*);

private:
	void CloseSensorLog();
	~ImuReader();

private:
	IPluginRuntime* m_RunTime;
	int m_refCnt;
	FILE* m_sensorLog;
	deltatime m_dt;
	uint32_t m_counter;
	boost::scoped_ptr<file_sampler> m_fileSampler;
	boost::scoped_ptr<sampler> m_sampler;
};

#endif /* IMUREADER_H_ */
