/*
 * drone.cpp
 *
 *  Created on: May 19, 2013
 *      Author: svassilev
 */

#include "commoninc.h"
#include "drone.h"
#include "quadrotorpilot.h"
#include "servodevice.h"
#include "imufilter.h"
#include "imureader.h"
#include "navigator.h"
#include "imubias.h"
#include "imuremap.h"
#include "imufilterplugin.h"
#include "tracelogplugin.h"
#include "imulowpassfilter.h"
#include "kalmanattitudefilter.h"

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
	m_config.Accel.SamplingRate = 200;
	m_config.Accel.Scale = 4;
	m_config.Accel.MaxReading = 32768;
	m_config.Gyro.DeviceName = "/dev/gyro0";
	m_config.Gyro.SamplingRate = 200;
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
		err = Reset(cmdArgs->GetDroneConfig(),false);
		break;
	case SD_COMMAND_EXIT:
		err = Reset(0,true);
		m_isRunning = false;
		break;
	case SD_COMMAND_RUN:
		SetConfig(cmdArgs->GetDroneConfig());
		//SetThrust(cmdArgs->GetDesiredThrust());
		//SetThrustRange(cmdArgs->GetMinThrust(),cmdArgs->GetMaxThrust());
		err = Run(cmdArgs);
		break;
	case SD_COMMAND_NONE:
	default:
		SetConfig(cmdArgs->GetDroneConfig());
		break;
	}
	return err;
}

void Drone::PrintConfig(const SdDroneConfig* config)
{
	printf("Drone configuration:\n");
	printf("Gyro device:    %s\n",config->Gyro.DeviceName);
	printf("Gyro rate:      %d Hz\n",config->Gyro.SamplingRate);
	printf("Gyro scale:     %d DPS\n",config->Gyro.Scale);
	printf("Gyro watermark: %d\n",config->Gyro.Watermark);
	printf("Accel device:   %s\n",config->Accel.DeviceName);
	printf("Accel rate:     %d Hz\n",config->Accel.SamplingRate);
	printf("Accel scale:    %d G\n",config->Accel.Scale);
	printf("Accel watermrk: %d\n",config->Accel.Watermark);
	printf("Compass:        %s\n",config->Mag.DeviceName);
	printf("IMU Angle:      %d\n",config->Quad.ImuAngleAxisZ);
	for (int i = 0; (size_t)i < ARRAYSIZE(config->Quad.Motor); i++) {
		printf("Quad M%d:        Servo %d\n",i,config->Quad.Motor[i]);
	}
	printf("Kp:             %1.8f\n", config->Quad.Kp);
	printf("Ki:             %1.8f\n", config->Quad.Ki);
	printf("Kd:             %1.8f\n", config->Quad.Kd);
}

int Drone::Run(_CommandArgs* cmdArgs)
{
	int err = 0;
	if (m_isRunning) {
		// TEMP
		InitInternalPlugins();
		if (!m_pluginsStarted) {
			PrintConfig(cmdArgs->GetDroneConfig());
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
	static int initialized = 0;
	if (!__sync_bool_compare_and_swap (&initialized, 0, 1)) {
		return;
	}
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_IMU_DEVICE);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_IMU_REMAP);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_IMU_BIAS);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_IMU_LOWPASSFILTER);
	//SdPluginInitialize(this, PluginAttach,SD_PLUGIN_IMU_FILTER);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_IMU_KALMAN_FILTER);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_SERVO_PCA9685);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_NAVIGATOR);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_QUADPILOT);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_TRACELOG);
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
	if (0 == pluginName || 0 == (strcmp(pluginName,SD_PLUGIN_IMU_FILTER))){
		ImuFilterPlugin* imuFilter = new ImuFilterPlugin();
		if (0 != imuFilter) {
			imuFilter->AttachToChain(droneContext,attachPlugin);
			imuFilter->Release();
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
	if (0 == pluginName || 0 == (strcmp(pluginName,SD_PLUGIN_TRACELOG))){
		TraceLogPlugin* tracer = new TraceLogPlugin();
		if (0 != tracer) {
			tracer->AttachToChain(droneContext,attachPlugin);
			tracer->Release();
		}
	}
	if (0 == pluginName || 0 == (strcmp(pluginName,SD_PLUGIN_IMU_LOWPASSFILTER))){
		ImuLowPassFilter* imuLpf = new ImuLowPassFilter();
		if (0 != imuLpf) {
			imuLpf->AttachToChain(droneContext,attachPlugin);
			imuLpf->Release();
		}
	}
//#if 0
	if (0 == pluginName || 0 == (strcmp(pluginName,SD_PLUGIN_IMU_KALMAN_FILTER))){
		KalmanAttitudeFilter* kf = new KalmanAttitudeFilter();
		if (0 != kf) {
			kf->AttachToChain(droneContext,attachPlugin);
			kf->Release();
		}
	}
//#endif
	return 0;
}

