/*
 * drone.cpp
 *
 *  Created on: May 19, 2013
 *      Author: svassilev
 */

#include "commoninc.h"
#include "drone.h"
#include "sensorreader.h"
#include "quadrotorpilot.h"
#include "pidpilot.h"
#include "servodevice.h"
#include "imufilter.h"
#include "imureader.h"
#include "navigator.h"
#include "imubias.h"
#include "imuremap.h"

Drone* Drone::s_Only = 0;

extern "C" int SdPluginInitialize(
	IN void* context,
	IN SdPluginAttachFunc attachPlugin,
	IN const char* pluginName
	);

Drone* Drone::Create()
{
	Drone* drone = new Drone();
	if (drone != s_Only) {
		delete drone;
		return 0;
	}
	return s_Only;
}

Drone::Drone()
{
	if (!__sync_bool_compare_and_swap (&s_Only, 0, this)) {
		return;
	}

	memset(&m_config,0,sizeof(m_config));
	m_config.Accel.DeviceName = "/dev/accel0";
	m_config.Accel.Rate = 200;
	m_config.Accel.Scale = 4;
	m_config.Accel.MaxReading = 32768;
	m_config.Gyro.DeviceName = "/dev/gyro0";
	m_config.Gyro.Rate = 200;
	m_config.Gyro.Scale = 2000;
	m_config.Gyro.MaxReading = 32768;
	m_config.Mag.DeviceName = "/dev/mag0";
	m_config.Servo.DeviceName = "/dev/pwm0";
	m_config.Servo.ChannelMask = 0xff;
	m_config.Servo.Rate = 100;
	m_config.Quad.Motor[0] = 0;
	m_config.Quad.Motor[1] = 2;
	m_config.Quad.Motor[2] = 3;
	m_config.Quad.Motor[3] = 4;
	m_config.Quad.ImuAngleAxisZ = 45;
	m_config.Quad.Kp = 0.6;
	m_config.Quad.Ki = 0.00000005;
	m_config.Quad.Kd = 0.0015;

	m_isRunning = true;
	m_pluginsStarted = false;

	InitInternalPlugins();
}

Drone::~Drone()
{
	__sync_bool_compare_and_swap (&s_Only, this, 0);
}

int Drone::Reset(
		const SdDroneConfig* config,
		bool detachPlugins)
{
	int err = ENOMEM;

	if (config) {
		m_config = *config;
	}

	err = m_pluginChain.StopPlugins(detachPlugins);

	if (err < 0) {
		fprintf(stdout,"Failed to reset the drone, err=%d\n",err);
	} else {
		printf("Drone was successfully reset!\n");
	}

	m_pluginsStarted = false;

	return err;
}

int Drone::ExecuteCommand(
	_CommandArgs* cmdArgs)
{
	int err = EINVAL;
	switch (cmdArgs->GetCommand()) {
	case SD_COMMAND_RESET:
		err = Reset(cmdArgs->GetSdDroneConfig(),false);
		break;
	case SD_COMMAND_EXIT:
		err = Reset(0,true);
		m_isRunning = false;
		break;
	case SD_COMMAND_RUN:
		SetConfig(cmdArgs->GetSdDroneConfig());
		//SetThrust(cmdArgs->GetDesiredThrust());
		//SetThrustRange(cmdArgs->GetMinThrust(),cmdArgs->GetMaxThrust());
		err = Run(cmdArgs);
		break;
	case SD_COMMAND_NONE:
	default:
		SetConfig(cmdArgs->GetSdDroneConfig());
		break;
	}
	return err;
}


void Drone::PrintState(FILE* out)
{
#if 0
	fprintf(out,"-->================ Iteration %lu ===================\n",
			(unsigned long)m_Iteration);
	const QuaternionD& attQ = m_ImuFilter->GetAttitude();
	const Vector4d* mot = m_Pilot->GetMotors();
	Vector3d x(1, 0, 0), y(0, 1, 0), z(0, 0, 1);
	Vector3d gyroDps = m_ImuFilter->GetGyroData();
	x = attQ.rotate(Vector3d(1, 0, 0));
	y = attQ.rotate(Vector3d(0, 1, 0));
	z = attQ.rotate(Vector3d(0, 0, 1));
	fprintf(out,"--> X     : %1.3lf %1.3lf %1.3lf\n",x.at(0,0),x.at(1,0),x.at(2,0));
	fprintf(out,"--> Y     : %1.3lf %1.3lf %1.3lf\n",y.at(0,0),y.at(1,0),y.at(2,0));
	fprintf(out,"--> Z     : %1.3lf %1.3lf %1.3lf\n",z.at(0,0),z.at(1,0),z.at(2,0));
	fprintf(out,"--> Q     : %1.3lf %1.3lf %1.3lf %1.3lf\n",
			attQ.w,attQ.x,attQ.y,attQ.z);
	fprintf(out,"--> Accel : %1.3lf %1.3lf %1.3lf\n",
			m_ImuFilter->GetAccelData().at(0,0),
			m_ImuFilter->GetAccelData().at(1,0),
			m_ImuFilter->GetAccelData().at(2,0));
	fprintf(out,"--> Gyro  : %4.3lf %4.3lf %4.3lf\n",
			gyroDps.at(0,0), gyroDps.at(1,0), gyroDps.at(2,0));

	fprintf(out,"--> Motor : %1.3lf %1.3lf %1.3lf %1.3lf\n",
			mot->at(0,0),mot->at(1,0),mot->at(2,0),mot->at(3,0));
	const Vector3d& err = m_Pilot->GetErrorAxis();
	gyroDps = m_Pilot->GetOmega();
	fprintf(out,"--> AngV  : %4.3lf %4.3lf %4.3lf\n",
			gyroDps.at(0,0), gyroDps.at(1,0), gyroDps.at(2,0));
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
			gyroDps.at(0,0), gyroDps.at(1,0), gyroDps.at(2,0),
			m_ImuFilter->GetMagData().at(0,0),
			m_ImuFilter->GetMagData().at(1,0),
			m_ImuFilter->GetMagData().at(2,0));

	if (m_config.PrintRotationMatrix) {
		QuaternionD tgtQ = m_MissionControl->GetDesiredAttitude();
		fprintf(out,"--> Tgt Q : %1.3lf %1.3lf %1.3lf %1.3lf\n",
				tgtQ.w,tgtQ.w,tgtQ.y,tgtQ.z);
		fprintf(out,"--> Vg    : %1.3lf %1.3lf %1.3lf\n",
					m_ImuFilter->GetVg().at(0,0),
					m_ImuFilter->GetVg().at(1,0),
					m_ImuFilter->GetVg().at(2,0));
		Matrix4d rotMx = attQ.rotMatrix4();
		fprintf(out, "%5.9lf %5.9lf %5.9lf %5.9lf %5.9lf %5.9lf %5.9lf %5.9lf %5.9lf \n",
				rotMx.at(0, 0), rotMx.at(0, 1), rotMx.at(0, 2),
				rotMx.at(1, 0), rotMx.at(1, 1), rotMx.at(1, 2),
				rotMx.at(2, 0), rotMx.at(2, 1), rotMx.at(2, 2));
	}

	fprintf(out,"-->=====================================================\n");
#endif
}

void Drone::PrintConfig(const SdDroneConfig* config)
{
	printf("Drone configuration:\n");
	printf("Gyro device:  %s\n",config->Gyro.DeviceName);
	printf("Gyro rate:    %d Hz\n",config->Gyro.Rate);
	printf("Gyro scale:   %d DPS\n",config->Gyro.Scale);
	printf("Accel device: %s\n",config->Accel.DeviceName);
	printf("Accel rate:   %d Hz\n",config->Accel.Rate);
	printf("Accel scale:  %d G\n",config->Accel.Scale);
	printf("Compass:      %s\n",config->Mag.DeviceName);
	printf("IMU Angle:    %d\n",config->Quad.ImuAngleAxisZ);
	for (int i = 0; (size_t)i < ARRAYSIZE(config->Quad.Motor); i++) {
		printf("Quad M%d:      Servo %d\n",i,config->Quad.Motor[i]);
	}
	printf("Kp:           %1.8f\n", config->Quad.Kp);
	printf("Ki:           %1.8f\n", config->Quad.Ki);
	printf("Kd:           %1.8f\n", config->Quad.Kd);
}

int Drone::Run(_CommandArgs* cmdArgs)
{
	int err = 0;
	if (m_isRunning) {
		if (!m_pluginsStarted) {
			if (0 == (err = m_pluginChain.StartPlugins(cmdArgs))) {
				m_pluginsStarted = true;
			}
		} else {
			err = m_pluginChain.ExecuteCommand(cmdArgs);
		}
	}
	return err;
}

void Drone::InitInternalPlugins()
{
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_IMU_DEVICE);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_IMU_REMAP);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_IMU_BIAS);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_NAVIGATOR);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_QUADPILOT);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_SERVO_PCA9685);
}

bool Drone::IsRunning(void)
{
	return m_isRunning;
}

int Drone::PluginAttach(
	IN void* droneContext,
	IN IPlugin* plugin,
	IN SdPluginAltitude desiredAltitude,
	IN const char* const attachAbove[],
	IN const char* const attachBelow[],
	OUT IPluginRuntime** pluginRuntime)
{
	assert(s_Only);
	if (0 == plugin->GetName() || 0 == plugin->GetVersion() ||
		0 != strcmp(plugin->GetVersion(),SD_PLUGIN_VERSION)) {
		return EINVAL;
	}
	return s_Only->m_pluginChain.PluginAttach(
			plugin,
			desiredAltitude,
			attachAbove,
			attachBelow,
			pluginRuntime);
}

int SdPluginInitialize(
	IN void* droneContext,
	IN SdPluginAttachFunc attachPlugin,
	IN const char* pluginName)
{
	if (0 == pluginName || 0 == (strcmp(pluginName,SD_PLUGIN_IMU_DEVICE))){
		ImuReader* imuReader = new ImuReader();
		if (0 != imuReader) {
			imuReader->AttachToChain(droneContext,attachPlugin);
			imuReader->Release();
		}
	}
	if (0 == pluginName || 0 == (strcmp(pluginName,SD_PLUGIN_QUADPILOT))){
		QuadRotorPilot* pilot = new QuadRotorPilot();
		if (0 != pilot) {
			pilot->AttachToChain(droneContext,attachPlugin);
			pilot->Release();
		}
	}
	if (0 == pluginName || 0 == (strcmp(pluginName,SD_PLUGIN_NAVIGATOR))){
		Navigator* navigator = new Navigator();
		if (0 != navigator) {
			navigator->AttachToChain(droneContext,attachPlugin);
			navigator->Release();
		}
	}
	if (0 == pluginName || 0 == (strcmp(pluginName,SD_PLUGIN_IMU_BIAS))){
		ImuBias* imuBias = new ImuBias();
		if (0 != imuBias) {
			imuBias->AttachToChain(droneContext,attachPlugin);
			imuBias->Release();
		}
	}
	if (0 == pluginName || 0 == (strcmp(pluginName,SD_PLUGIN_IMU_REMAP))){
		ImuRemap* imuRemap = new ImuRemap();
		if (0 != imuRemap) {
			imuRemap->AttachToChain(droneContext,attachPlugin);
			imuRemap->Release();
		}
	}
	if (0 == pluginName || 0 == (strcmp(pluginName,SD_PLUGIN_SERVO_PCA9685))){
		ServoDevice* servoDev = new ServoDevice();
		if (0 != servoDev) {
			servoDev->AttachToChain(droneContext,attachPlugin);
			servoDev->Release();
		}
	}
	return 0;
}

