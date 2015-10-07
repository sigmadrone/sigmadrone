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

#ifndef IMULPF_H
#define IMULPF_H

#include "commoninc.h"
#include "firfilt.h"

/*
 * Reads data from all the sensor devices
 * Should be able to read from a file
 */
class ImuLowPassFilter: public IPlugin
{
public:
	ImuLowPassFilter();
	int AttachToChain(void*,SdPluginAttachFunc attachPlugin);

public:
	int AddRef();
	int Release();
	int ExecuteCommand(SdCommandParams* commandArgs);
	void Stop(int flags);
	const char* GetName();
	SdDeviceId GetDeviceId();
	const char* GetVersion();
	const char* GetDlFileName();
	int IoCallback(SdIoPacket* ioPacket);
	int IoDispatchThread();

private:
	~ImuLowPassFilter();

private:
	LpPreFilter3d gyrofilt_;
	FirFilter3d<38>::type accelfilt_38_;
	FirFilter3d<199>::type accelfilt_200_;
	IPluginRuntime* runtime_;
	Vector3d filtered_accel3d_;
	int ref_cnt_;
};

#endif /* IMULPF_H */
