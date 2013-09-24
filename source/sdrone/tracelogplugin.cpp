/*
 * tracelogplugin.cpp
 *
 *  Created on: September 20, 2013
 *      Author: svassilev
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

int TraceLogPlugin::Start(
	CommandArgs* cmdArgs)
{
	m_iteration = 0;
	m_logLevel = cmdArgs->GetDroneConfig()->LogLevel;
	m_logRotMatrix = cmdArgs->GetDroneConfig()->LogRotationMatrix;
	m_logPeriod = 1.0/(double)cmdArgs->GetDroneConfig()->LogRate;
	m_runtime->SetIoFilters(
		SD_DEVICEID_TO_FLAG(SD_DEVICEID_IMU),
		SD_IOCODE_TO_FLAG(SD_IOCODE_RECEIVE));
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

void TraceLogPlugin::Stop(int flags)
{
	if (!!(flags&FLAG_STOP_AND_DETACH)) {
		m_runtime->DetachPlugin();
	}
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
	const QuaternionD* attQ;
	const Vector4d* mot;
	Vector3d x(1, 0, 0), y(0, 1, 0), z(0, 0, 1);

	++m_iteration;
	m_timeAfterLastLog += ioPacket->deltaTime;

	if (m_timeAfterLastLog < m_logPeriod) {
		return SD_ESUCCESS;
	}

	m_timeAfterLastLog = 0;

	if (m_logLevel < SD_LOG_LEVEL_VERBOSE) {
		return SD_ESUCCESS;
	}
	m_runtime->Log(SD_LOG_LEVEL_VERBOSE,
			"--> dT,s  : %1.6lf  %1.6f (sensor read)\n",
			ioPacket->deltaTime, ioPacket->timeToReadSensors);

	if (ioPacket->accelData) {
		m_runtime->Log(SD_LOG_LEVEL_VERBOSE,"--> Accel : %1.3lf %1.3lf %1.3lf\n",
				ioPacket->accelData->at(0,0),
				ioPacket->accelData->at(1,0),
				ioPacket->accelData->at(2,0));
	}

	const Vector3d* gyroDps = ioPacket->gyroDataDps;
	if (gyroDps) {
		m_runtime->Log(SD_LOG_LEVEL_VERBOSE,"--> Gyro  : %4.3lf %4.3lf %4.3lf\n",
				gyroDps->at(0,0), gyroDps->at(1,0), gyroDps->at(2,0));
	}

	if (ioPacket->magData) {
		m_runtime->Log(SD_LOG_LEVEL_VERBOSE,"--> Mag   : %1.3lf %1.3lf %1.3lf\n",
				ioPacket->magData->at(0,0),
				ioPacket->magData->at(1,0),
				ioPacket->magData->at(2,0));
	}

	if (!ioPacket->attitudeQ) {
		goto __return;
	}

	attQ = ioPacket->attitudeQ;
	mot = ioPacket->motors;
	x = attQ->rotate(Vector3d(1, 0, 0));
	y = attQ->rotate(Vector3d(0, 1, 0));
	z = attQ->rotate(Vector3d(0, 0, 1));
	m_runtime->Log(SD_LOG_LEVEL_VERBOSE,
			"--> X     : %1.3lf %1.3lf %1.3lf\n",x.at(0,0),x.at(1,0),x.at(2,0));
	m_runtime->Log(SD_LOG_LEVEL_VERBOSE,
			"--> Y     : %1.3lf %1.3lf %1.3lf\n",y.at(0,0),y.at(1,0),y.at(2,0));
	m_runtime->Log(SD_LOG_LEVEL_VERBOSE,
			"--> Z     : %1.3lf %1.3lf %1.3lf\n",z.at(0,0),z.at(1,0),z.at(2,0));
	m_runtime->Log(SD_LOG_LEVEL_VERBOSE,
			"--> Q     : %1.3lf %1.3lf %1.3lf %1.3lf\n",
			attQ->w,attQ->x,attQ->y,attQ->z);

	if (0 != mot) {
		m_runtime->Log(SD_LOG_LEVEL_VERBOSE,"--> Motor : %1.3lf %1.3lf %1.3lf %1.3lf\n",
			mot->at(0,0),mot->at(1,0),mot->at(2,0),mot->at(3,0));
	}
#if 0
	const Vector3d& err = m_Pilot->GetErrorAxis();
	fprintf(out,"--> AngAcc: %4.3lf %4.3lf %4.3lf\n",
			m_Pilot->GetAngularAccel().at(0,0),
			m_Pilot->GetAngularAccel().at(1,0),
			m_Pilot->GetAngularAccel().at(2,0));
	fprintf(out,"--> Torque: %4.3lf %4.3lf %4.3lf Tc: %4.3f %4.3f %4.3f ErrAng: %4.3f\n",
			err.at(0,0),err.at(1,0),err.at(2,0),
			m_Pilot->GetTorqueCorrection().at(0,0),
			m_Pilot->GetTorqueCorrection().at(1,0),
			m_Pilot->GetTorqueCorrection().at(2,0),
			m_Pilot->GetErrorAngle());

	fprintf(out, "--> IMU %5.9lf %5.9lf %5.9lf %5.9lf %5.9lf %5.9lf %5.9lf %5.9lf %5.9lf \n",
			m_ImuFilter->GetAccelData().at(0,0),
			m_ImuFilter->GetAccelData().at(1,0),
			m_ImuFilter->GetAccelData().at(2,0),
			gyroDps->at(0,0), gyroDps->at(1,0), gyroDps->at(2,0),
			m_ImuFilter->GetMagData().at(0,0),
			m_ImuFilter->GetMagData().at(1,0),
			m_ImuFilter->GetMagData().at(2,0));
#endif

	if (m_logRotMatrix) {
		const QuaternionD* tgtQ = ioPacket->targetQ;
		m_runtime->Log(SD_LOG_LEVEL_VERBOSE,"--> Tgt Q : %1.3lf %1.3lf %1.3lf %1.3lf\n",
				tgtQ->w,tgtQ->w,tgtQ->y,tgtQ->z);
		Matrix4d rotMx = attQ->rotMatrix4();
		m_runtime->Log(SD_LOG_LEVEL_VERBOSE,
				"%5.9lf %5.9lf %5.9lf %5.9lf %5.9lf %5.9lf %5.9lf %5.9lf %5.9lf \n",
				rotMx.at(0, 0), rotMx.at(0, 1), rotMx.at(0, 2),
				rotMx.at(1, 0), rotMx.at(1, 1), rotMx.at(1, 2),
				rotMx.at(2, 0), rotMx.at(2, 1), rotMx.at(2, 2));
	}

	__return:
	m_runtime->Log(SD_LOG_LEVEL_VERBOSE,
			"<-- ================ End Iteration %lu ===================\n\n",
			(unsigned long)m_iteration);

	return SD_ESUCCESS;
}

int TraceLogPlugin::IoDispatchThread()
{
	assert(false);
	return EINVAL;
}
