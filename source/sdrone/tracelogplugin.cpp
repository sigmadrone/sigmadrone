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

#include "commoninc.h"
#include "tracelogplugin.h"

TraceLogPlugin::TraceLogPlugin()
{
	m_runtime = 0;
	m_refCnt = 1;
	m_iteration = 0;
	m_logRotMatrix = 0;
	m_logLevel = SD_LOG_LEVEL_NONE;
	m_logPeriod = 0.1;
	m_timeAfterLastLog = 0;
}

TraceLogPlugin::~TraceLogPlugin()
{
	assert(0 == m_refCnt);
}

int TraceLogPlugin::AttachToChain(
		void* droneContext,
		SdPluginAttachFunc attachFunc)
{
	int err = attachFunc(
			droneContext,
			this,
			SD_ALTITUDE_LAST,
			0,
			0,
			&m_runtime);
	return err;
}

int TraceLogPlugin::ExecuteCommand(
		SdCommandParams* params)
{
	switch (params->CommandCode()) {
	case SD_COMMAND_RUN:
		m_iteration = 0;
		m_logLevel = params->Params().asDroneConfig().LogLevel;
		m_logRotMatrix = params->Params().asDroneConfig().LogRotationMatrix;
		m_logPeriod = 1.0/(double)params->Params().asDroneConfig().LogRate;
		m_runtime->SetIoFilters(
			SD_DEVICEID_TO_FLAG(SD_DEVICEID_IMU),
			SD_IOCODE_TO_FLAG(SD_IOCODE_RECEIVE));
		break;
	case SD_COMMAND_EXIT:
		m_runtime->DetachPlugin();
		break;
	case SD_COMMAND_SET_CONFIG:
		m_logLevel = params->Params().asDroneConfig().LogLevel;
		m_logRotMatrix = params->Params().asDroneConfig().LogRotationMatrix;
		m_logPeriod = 1.0/(double)params->Params().asDroneConfig().LogRate;
		break;
	default:break;
	}
	return SD_ESUCCESS;
}

int TraceLogPlugin::AddRef()
{
	return __sync_fetch_and_add(&m_refCnt,1);
}

int TraceLogPlugin::Release()
{
	int refCnt = __sync_sub_and_fetch(&m_refCnt,1);
	if (0 == refCnt) {
		delete this;
	}
	return refCnt;
}

const char* TraceLogPlugin::GetName()
{
	return SD_PLUGIN_TRACELOG;
}

SdDeviceId TraceLogPlugin::GetDeviceId()
{
	return SD_DEVICEID_FILTER;
}

const char* TraceLogPlugin::GetVersion()
{
	return SD_PLUGIN_VERSION;
}

extern char **g_Argv;
const char* TraceLogPlugin::GetDlFileName()
{
	return g_Argv[0];
}

int TraceLogPlugin::IoCallback(
	SdIoPacket* ioPacket)
{
	Vector3d x(1, 0, 0), y(0, 1, 0), z(0, 0, 1);
	SdIoData ioData;

	++m_iteration;
	m_timeAfterLastLog += ioPacket->DeltaTime();

	if (m_timeAfterLastLog < m_logPeriod) {
		return SD_ESUCCESS;
	}

	m_timeAfterLastLog = 0;

	if (m_logLevel < SD_LOG_LEVEL_VERBOSE) {
		return SD_ESUCCESS;
	}

	const QuaternionD& attQ = ioPacket->Attitude();
	x = attQ.rotate(Vector3d(1, 0, 0));
	y = attQ.rotate(Vector3d(0, 1, 0));
	z = attQ.rotate(Vector3d(0, 0, 1));
	m_runtime->Log(SD_LOG_LEVEL_VERBOSE,
			"--> X     : %1.3lf %1.3lf %1.3lf\n",x.at(0,0),x.at(1,0),x.at(2,0));
	m_runtime->Log(SD_LOG_LEVEL_VERBOSE,
			"--> Y     : %1.3lf %1.3lf %1.3lf\n",y.at(0,0),y.at(1,0),y.at(2,0));
	m_runtime->Log(SD_LOG_LEVEL_VERBOSE,
			"--> Z     : %1.3lf %1.3lf %1.3lf\n",z.at(0,0),z.at(1,0),z.at(2,0));
	m_runtime->Log(SD_LOG_LEVEL_VERBOSE,
			"--> Q     : %1.3lf %1.3lf %1.3lf %1.3lf\n",
			attQ.w,attQ.x,attQ.y,attQ.z);

	m_runtime->Log(SD_LOG_LEVEL_VERBOSE,
			"--> dT,s  : %1.6lf  %1.6f (sensor read)\n",
			ioPacket->DeltaTime(),
			ioPacket->GetAttribute(SDIO_ATTR_TIME_TO_READ_SENSORS).asDouble());

	m_runtime->Log(SD_LOG_LEVEL_VERBOSE,"--> Accel : %1.3lf %1.3lf %1.3lf\n",
			ioPacket->AccelData().at(0,0),
			ioPacket->AccelData().at(1,0),
			ioPacket->AccelData().at(2,0));

	ioData = ioPacket->GetAttribute(SDIO_ATTR_EARTH_G);
	if (ioData.dataType()== SdIoData::TYPE_VECTOR3D) {
		m_runtime->Log(SD_LOG_LEVEL_VERBOSE,
				"--> EarthG: %1.3lf %1.3lf %1.3lf\n",
				ioData.asVector3d().at(0,0),ioData.asVector3d().at(1,0),
				ioData.asVector3d().at(2,0));
	}

	m_runtime->Log(SD_LOG_LEVEL_VERBOSE,"--> Gyro  : %4.3lf %4.3lf %4.3lf\n",
				ioPacket->GyroData().at(0,0),
				ioPacket->GyroData().at(1,0),
				ioPacket->GyroData().at(2,0));

	m_runtime->Log(SD_LOG_LEVEL_VERBOSE,"--> Mag   : %1.3lf %1.3lf %1.3lf\n",
			ioPacket->MagData().at(0,0),
			ioPacket->MagData().at(1,0),
			ioPacket->MagData().at(2,0));


	const Vector4d& mot = ioPacket->GetAttribute(SDIO_ATTR_MOTORS).asVector4d();
	m_runtime->Log(SD_LOG_LEVEL_VERBOSE,"--> Motor : %1.3lf %1.3lf %1.3lf %1.3lf\n",
			mot.at(0,0),mot.at(1,0),mot.at(2,0),mot.at(3,0));

	ioData = ioPacket->GetAttribute(SDIO_ATTR_ERR_PID);
	if (ioData.dataType()== SdIoData::TYPE_VECTOR3D) {
		m_runtime->Log(SD_LOG_LEVEL_VERBOSE,
				"--> ErrPID: %1.3lf %1.3lf %1.3lf\n",
				ioData.asVector3d().at(0,0),ioData.asVector3d().at(1,0),
				ioData.asVector3d().at(2,0));
	}

	if (m_logRotMatrix) {
		m_runtime->Log(SD_LOG_LEVEL_VERBOSE, "%5.9lf %5.9lf %5.9lf %5.9lf \n",
				attQ.w,attQ.x,attQ.y,attQ.z);
	}

	m_runtime->Log(SD_LOG_LEVEL_VERBOSE,
			"--> ================ End Iteration %lu ===================\n\n",
			(unsigned long)m_iteration);

	return SD_ESUCCESS;
}

int TraceLogPlugin::IoDispatchThread()
{
	assert(false);
	return EINVAL;
}
