/*
 * drone.cpp
 *
 *  Created on: May 19, 2013
 *      Author: svassilev
 */

#include "commoninc.h"
#include <sys/stat.h>
#include <sys/signal.h>
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
#include "jsonrpcdispatch.h"
#include "daemon.h"

Drone* Drone::s_Only = 0;

extern "C" int SdPluginInitialize(
	IN void* context,
	IN SdPluginAttachFunc attachPlugin,
	IN const char* pluginName
	);

Drone* Drone::Create() {
	Drone* drone = new Drone();
	if (!!drone) {
		if (!drone->m_globalLock.IsLocked()) {
			delete drone;
			drone = 0;
		} else {
			s_Only = drone;
		}
	}
	return s_Only;
}

void Drone::Destroy() {
	if (s_Only) {
		delete s_Only;
		s_Only = 0;
	}
}

void Drone::fatal_error_signal (int sig)
{
	static int fatal_error_in_progress = 0;
	/* Since this handler is established for more than one kind of signal,
          it might still get invoked recursively by delivery of some other kind
          of signal.  Use a static variable to keep track of that. */
	if (fatal_error_in_progress) {
		raise (sig);
	}
	fatal_error_in_progress = 1;

	/*
	 * Now do the clean up actions:
	 * 		- reset terminal modes
	 * 		- kill child processes
	 * 		- remove lock files
     */

	printf("FATAL: error signal %d raised, shutting down the drone!\n",
			sig);
	Drone::Only()->OnExit();

	/*
	 * Now reraise the signal.  We reactivate the signal's
	 * default handling, which is to terminate the process.
	 * We could just call exit or abort,
	 * but reraising the signal sets the return status
	 * from the process correctly.
	 */
	signal (sig, SIG_DFL);
	raise (sig);
}

Drone::Drone() :
		m_rpcDispatch(0),
		m_rpcTransport(0),
		m_globalLock("sigmadrone",true),
		m_isRunning(false)
{
	/*
	 * Register error signal handlers
	 */
	signal(SIGFPE,fatal_error_signal);
	signal(SIGILL,fatal_error_signal);
	signal(SIGSEGV,fatal_error_signal);
	signal(SIGBUS,fatal_error_signal);
	signal(SIGABRT,fatal_error_signal);

	if (!m_globalLock.IsLocked()) {
		printf("Failed to obtain global lock: %s\n",strerror(errno));
	}
}

Drone::~Drone()
{
	if (m_rpcDispatch) {
		delete m_rpcDispatch;
	}
	if (m_rpcTransport) {
		delete m_rpcTransport;
	}
}

int Drone::Run(const _CommandArgs& args)
{
	if (args.IsDaemon()) {
		daemon_init();
	}

	m_commandArgs = args;

	InitInternalPlugins();

	if (0 != m_rpcDispatch) {
		delete m_rpcDispatch;
		m_rpcDispatch = 0;
	}
	if (0 != m_rpcTransport) {
		delete m_rpcTransport;
		m_rpcTransport = 0;
	}

	m_rpcTransport = IRpcTransport::Create(IRpcTransport::TRANSPORT_HTTP);
	if (0 == m_rpcTransport) {
		return ENOMEM;
	}

	m_rpcDispatch = new SdJsonRpcDispatcher(m_rpcTransport);
	if (0 == m_rpcDispatch) {
		return ENOMEM;
	}
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_FLY),
			OnRpcCommandFly,
			this);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_RESET),
			OnRpcCommandReset,
			this);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_EXIT),
			OnRpcCommandExit,
			this);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_GET_STATE),
			OnRpcCommandGetState,
			this);

	//
	// Execute the command that came with the command line
	//
	switch(args.GetCommand()) {
	case SD_COMMAND_FLY:
		OnFly();
		break;
	case SD_COMMAND_RESET:
	case SD_COMMAND_NONE:
		OnReset();
		break;
	default:
	case SD_COMMAND_EXIT:
		// OK, we will just return
		return 0;
	}

	//
	// Start serving requests. Note this call will block the thread and return
	// only after an exit command was received
	//
	return m_rpcDispatch->StartServingRequests(std::string("0.0.0.0"),
			args.GetServerPort());
}

int Drone::OnReset()
{
	int err = m_pluginChain.StopPlugins(false);
	if (err < 0) {
		fprintf(stdout,"Failed to reset the drone, err=%d\n",err);
	} else {
		printf("Drone was successfully reset!\n");
	}

	return err;
}

int Drone::OnExit()
{
	int err = m_pluginChain.StopPlugins(true);
	if (err < 0) {
		fprintf(stdout,"Failed to reset the drone, err=%d\n",err);
	} else {
		printf("Drone was successfully reset!\n");
	}

	assert (m_rpcDispatch);
	m_rpcDispatch->StopServingRequests();

	return err;
}

int Drone::OnFly()
{
	int err = 0;
	if (!m_pluginChain.ArePluginsStarted()) {
		PrintConfig(m_commandArgs.GetDroneConfig());
		err = m_pluginChain.StartPlugins(&m_commandArgs);
	} else {
		err = m_pluginChain.ExecuteCommand(&m_commandArgs);
	}
	return err;
}

void Drone::PrintConfig(const SdDroneConfig* config)
{
	printf("Drone configuration:\n");
	printf("Gyro device:    %s\n",config->Gyro.DeviceName.c_str());
	printf("Gyro rate:      %d Hz\n",config->Gyro.SamplingRate);
	printf("Gyro scale:     %d DPS\n",config->Gyro.Scale);
	printf("Gyro watermark: %d\n",config->Gyro.Watermark);
	printf("Accel device:   %s\n",config->Accel.DeviceName.c_str());
	printf("Accel rate:     %d Hz\n",config->Accel.SamplingRate);
	printf("Accel scale:    %d G\n",config->Accel.Scale);
	printf("Accel watermrk: %d\n",config->Accel.Watermark);
	printf("Compass:        %s\n",config->Mag.DeviceName.c_str());
	printf("IMU Angle:      %d\n",config->Quad.ImuAngleAxisZ);
	for (int i = 0; (size_t)i < ARRAYSIZE(config->Quad.Motor); i++) {
		printf("Quad M%d:        Servo %d\n",i,config->Quad.Motor[i]);
	}
	printf("Kp:             %1.8f\n", config->Quad.Kp);
	printf("Ki:             %1.8f\n", config->Quad.Ki);
	printf("Kd:             %1.8f\n", config->Quad.Kd);
}

void Drone::InitInternalPlugins()
{
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

void Drone::OnRpcCommandFly(
		void* context,
		const SdJsonRpcRequest* req,
		SdJsonRpcReply* rep)
{
	assert(context == Only());
	rep->ErrorCode = SD_JSONRPC_ERROR_SUCCESS;
	Only()->m_commandArgs.ParseJsonRpcArgs(req->Params);
	if (0 != Only()->OnFly()) {
		rep->ErrorCode = SD_JSONRPC_ERROR_APP;
	} else {
		rep->Results.SetValueAsInt(0);
	}
	rep->Id = req->Id;
}

void Drone::OnRpcCommandExit(
		void* context,
		const SdJsonRpcRequest* req,
		SdJsonRpcReply* rep)
{
	assert(context == Only());
	rep->ErrorCode = SD_JSONRPC_ERROR_SUCCESS;
	if (0 != Only()->OnExit()) {
		rep->ErrorCode = SD_JSONRPC_ERROR_APP;
	} else {
		rep->Results.SetValueAsInt(0);
	}
	rep->Id = req->Id;
}

void Drone::OnRpcCommandReset(
		void* context,
		const SdJsonRpcRequest* req,
		SdJsonRpcReply* rep)
{
	assert(context == Only());
	rep->ErrorCode = SD_JSONRPC_ERROR_SUCCESS;
	Only()->m_commandArgs.ParseJsonRpcArgs(req->Params);
	if (0 != Only()->OnReset()) {
		rep->ErrorCode = SD_JSONRPC_ERROR_APP;
	} else {
		rep->Results.SetValueAsInt(0);
	}
	rep->Id = req->Id;
}

void Drone::OnRpcCommandGetState(
		void* context,
		const SdJsonRpcRequest* req,
		SdJsonRpcReply* rep)
{
	assert(context == Only());
	rep->ErrorCode = SD_JSONRPC_ERROR_SUCCESS;
	if (!Only()->m_commandArgs.GetCommandArgsAsJobj()) {
		rep->ErrorCode = SD_JSONRPC_ERROR_APP;
	} else {
		rep->Results.SetValueAsObject(
			Only()->m_commandArgs.GetCommandArgsAsJobj());
	}
	rep->Id = req->Id;
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

