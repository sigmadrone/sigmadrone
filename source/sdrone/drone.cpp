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
#include "pidpilotsd.h"
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
#include "plugincommandparams.h"
#include "rpcparams.h"
#include "jsonrpcparser.h"

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
		m_targetQ(1,0,0,0),
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

int Drone::Run(CommandLineArgs& args)
{
	SdJsonValue jsonArgs;
	RpcParams::BuildJsonDroneConfigFromCmdLineArgs(&jsonArgs, args);
	RpcParams::ParseJsonDroneConfig(&jsonArgs,&m_droneConfig.m_config);

	if (args.IsDaemon()) {
		daemon_init();
	}

	m_thrustValues = SdThrustValues(
			args.GetDesiredThrust(), args.GetMinThrust(), args.GetMaxThrust());

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
	SdJsonValue paramsSpec;
	SdJsonValue resultSpec;
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_RUN),
			OnRpcCommandRun,
			this);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_RESET),
			OnRpcCommandReset,
			this);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_EXIT),
			OnRpcCommandExit,
			this);
	RpcParams::BuildJsonDroneConfig(&jsonArgs,m_droneConfig.m_config,0);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_GET_CONFIG),
			OnRpcCommandGetConfig,
			this,
			SdJsonValue(),
			jsonArgs);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_SET_CONFIG),
			OnRpcCommandSetConfig,
			this,
			jsonArgs);
	RpcParams::BuildJsonPingParams(&jsonArgs,0.0);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_PING),
			OnRpcCommandPing,
			this,
			jsonArgs,
			jsonArgs);
	RpcParams::BuildJsonThrustParams(&jsonArgs,0.0,0.0,1.0);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_GET_THRUST),
			OnRpcCommandGetThrust,
			this,
			SdJsonValue(),
			jsonArgs);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_SET_THRUST),
			OnRpcCommandSetThrust,
			this,
			jsonArgs);
	RpcParams::BuildJsonTargetQuaternion(&jsonArgs,QuaternionD(1.0,0,0,0));
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_GET_ATTITUDE),
			OnRpcCommandGetAttitude,
			this,
			SdJsonValue(),
			jsonArgs);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_SET_TARGET_ATTITUDE),
			OnRpcCommandSetTargetAttitude,
			this,
			jsonArgs);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_GET_ALTITUDE),
			OnRpcCommandGetAltitude,
			this);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_SET_TARGET_ALTITUDE),
			OnRpcCommandSetTargetAltitude,
			this);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_GET_RPC_SPEC),
			OnRpcCommandGetRpcSpec,
			this,
			SdJsonValue("SD_COMMAND_CODE_XXX"),
			SdJsonValue("Spec"));


	SdJsonArray jarr;
	jarr.AddElement(SdJsonValue("sd_run"));
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_GET_RPC_LIST),
			OnRpcCommandGetRpcList,
			this,
			SdJsonValue(),
			SdJsonValue(jarr));

	//
	// Execute the command that came with the command line
	//
	switch(args.GetCommand()) {
	case SD_COMMAND_RUN: {
		int err;
		RpcParams::BuildJsonDroneConfig(&jsonArgs,m_droneConfig.m_config,0);
		if (SD_ESUCCESS != (err = OnRun(&jsonArgs))) {
			return err;
		}
		OnSetThrust(m_thrustValues);
		break;
	}
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
	PluginCommandParams params(SD_COMMAND_RESET,SdIoData(),0);
	int err = m_pluginChain.ExecuteCommand(&params,SD_FLAG_DISPATCH_DOWN);
	if (err < 0) {
		fprintf(stdout,"Failed to reset the drone, err=%d\n",err);
	} else {
		printf("Drone was successfully reset!\n");
	}
	m_isRunning = false;

	return err;
}

int Drone::OnExit()
{
	int err = OnReset();
	PluginCommandParams params(SD_COMMAND_EXIT,SdIoData(),0);
	m_pluginChain.ExecuteCommand(&params,SD_FLAG_DISPATCH_DOWN);

	assert (m_rpcDispatch);
	m_rpcDispatch->StopServingRequests();

	return err;
}

int Drone::OnRun(const IJsonValue* rpcParams)
{
	int err = 0;
	if (!m_isRunning) {
		SdIoData data(&m_droneConfig.m_config);
		PluginCommandParams params(SD_COMMAND_RUN,data,rpcParams);
		m_droneConfig.PrintConfig();
		if (0 == (err = m_pluginChain.ExecuteCommand(&params, 0))) {
			m_isRunning = true;
		} else {
			OnReset();
		}
	}
	return err;
}

int Drone::OnSetThrust(const SdThrustValues& thrustVals)
{
	SdIoData data(&thrustVals);
	printf("Setting thrust %.3f, min %.3f, max %.3f\n",
			thrustVals.Thrust(), thrustVals.MinThrust(),
			thrustVals.MaxThrust());
	PluginCommandParams params(SD_COMMAND_SET_THRUST,data,0);
	m_thrustValues = thrustVals;
	return m_pluginChain.ExecuteCommand(&params,SD_FLAG_DISPATCH_DOWN);
}

int Drone::OnSetConfig(
		const SdDroneConfig& config,
		const IJsonValue* rpcParams)
{
	SdIoData data(&config);
	PluginCommandParams params(SD_COMMAND_SET_CONFIG,data,rpcParams);
	m_droneConfig.m_config = config;
	return m_pluginChain.ExecuteCommand(&params,SD_FLAG_DISPATCH_DOWN);
}

int Drone::OnSetTargetQuaternion(const QuaternionD& targetQ)
{
	SdIoData data(&targetQ);
	PluginCommandParams params(SD_COMMAND_SET_TARGET_ATTITUDE,data,0);
	m_targetQ = targetQ;
	return m_pluginChain.ExecuteCommand(&params,SD_FLAG_DISPATCH_DOWN);
}

void Drone::InitInternalPlugins()
{
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_IMU_DEVICE);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_IMU_REMAP);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_IMU_BIAS);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_IMU_LOWPASSFILTER);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_IMU_FILTER);
	//SdPluginInitialize(this, PluginAttach,SD_PLUGIN_IMU_KALMAN_FILTER);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_SERVO_PCA9685);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_NAVIGATOR);
	//SdPluginInitialize(this, PluginAttach,SD_PLUGIN_QUADPILOT);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_PIDPILOT);
	SdPluginInitialize(this, PluginAttach,SD_PLUGIN_TRACELOG);
}

void Drone::OnRpcCommandRun(
		void* context,
		const SdJsonRpcRequest* req,
		SdJsonRpcReply* rep)
{
	assert(context == Only());
	int err;
	rep->ErrorCode = SD_JSONRPC_ERROR_SUCCESS;
	if (!RpcParams::ParseJsonDroneConfig(&req->Params,&Only()->m_droneConfig.m_config)) {
		rep->ErrorCode = SD_JSONRPC_ERROR_PARSE;
	} else if (0 != (err = Only()->OnRun(&req->Params))) {
		rep->ErrorCode = SD_JSONRPC_ERROR_APP;
		rep->ErrorMessage = strerror(err);
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
	if (0 != Only()->OnReset()) {
		rep->ErrorCode = SD_JSONRPC_ERROR_APP;
	} else {
		rep->Results.SetValueAsInt(0);
	}
	rep->Id = req->Id;
}

void Drone::OnRpcCommandGetConfig(
		void* context,
		const SdJsonRpcRequest* req,
		SdJsonRpcReply* rep)
{

	assert(context == Only());
	rep->ErrorCode = SD_JSONRPC_ERROR_SUCCESS;
	if (!RpcParams::BuildJsonDroneConfig(&rep->Results,Only()->m_droneConfig.m_config,0)) {
		rep->ErrorCode = SD_JSONRPC_ERROR_PARSE;
	}
	rep->Id = req->Id;
}

void Drone::OnRpcCommandPing(
		void* context,
		const SdJsonRpcRequest* req,
		SdJsonRpcReply* rep)
{
	int64_t timestamp = 0;
	assert(context == Only());
	if (req->Params.AsIntSafe(&timestamp)) {
		printf("Sending ping reply %lu\n",timestamp);
		rep->ErrorCode = SD_JSONRPC_ERROR_SUCCESS;
		rep->Results.SetValueAsInt(timestamp);
	} else {
		rep->ErrorMessage = "Ping request carries wrong timestamp value type ";
		rep->ErrorMessage += req->Params.GetTypeAsString();
		printf("WARN: %s\n", rep->ErrorMessage.c_str());
		rep->ErrorCode = SD_JSONRPC_ERROR_INVALID_METHOD_PARAMS;
	}
	rep->Id = req->Id;
}

void Drone::OnRpcCommandSetConfig(
		void* context,
		const SdJsonRpcRequest* req,
		SdJsonRpcReply* rep)
{
	assert(context == Only());
	SdDroneConfig config = Only()->m_droneConfig.m_config;
	rep->ErrorCode = SD_JSONRPC_ERROR_SUCCESS;
	if (!RpcParams::ParseJsonDroneConfig(&req->Params,&config)) {
		rep->ErrorCode = SD_JSONRPC_ERROR_PARSE;
	} else {
		int err = Only()->OnSetConfig(config,&req->Params);
		if (0 != err) {
			rep->ErrorCode = SD_JSONRPC_ERROR_APP;
			rep->ErrorMessage = strerror(err);
		}
		rep->Results.SetValueAsInt(0);
	}
	rep->Id = req->Id;
}

void Drone::OnRpcCommandSetThrust(
		void* context,
		const SdJsonRpcRequest* req,
		SdJsonRpcReply* rep)
{
	SdThrustValues thrust = Only()->m_thrustValues;
	assert(context == Only());
	rep->ErrorCode = SD_JSONRPC_ERROR_SUCCESS;
	if (!RpcParams::ParseJsonThrust(&req->Params, &thrust)) {
		rep->ErrorCode = SD_JSONRPC_ERROR_PARSE;
	} else {
		int err;
		if (0 != (err = Only()->OnSetThrust(thrust))) {
			rep->ErrorCode = SD_JSONRPC_ERROR_APP;
			rep->ErrorMessage = strerror(err);
		} else {
			rep->Results.SetValueAsInt(0);
		}
	}
	rep->Id = req->Id;
}

void Drone::OnRpcCommandGetThrust(
		void* Context,
		const SdJsonRpcRequest* req,
		SdJsonRpcReply* rep)
{
	const SdThrustValues& thrust = Only()->m_thrustValues;
	RpcParams::BuildJsonThrustParams(&rep->Results,thrust.Thrust(),
			thrust.MinThrust(),thrust.MaxThrust());
	rep->ErrorCode = SD_JSONRPC_ERROR_SUCCESS;
	rep->Id = req->Id;
}

void Drone::OnRpcCommandSetTargetAttitude(
		void* Context,
		const SdJsonRpcRequest* req,
		SdJsonRpcReply* rep)

{
	QuaternionD targetQ;
	rep->ErrorCode = SD_JSONRPC_ERROR_SUCCESS;
	if (!RpcParams::ParseJsonTargetQuaternion(&req->Params,&targetQ)) {
		rep->ErrorCode = SD_JSONRPC_ERROR_PARSE;
	} else {
		int err;
		if (0 != (err = Only()->OnSetTargetQuaternion(targetQ))) {
			rep->ErrorCode = SD_JSONRPC_ERROR_APP;
			rep->ErrorMessage = strerror(err);
		} else {
			rep->Results.SetValueAsInt(0);
		}
	}
	rep->Id = req->Id;
}

void Drone::OnRpcCommandGetAttitude(void* Context, const SdJsonRpcRequest*,
		SdJsonRpcReply*) {
	//TODO:
}

void Drone::OnRpcCommandSetTargetAltitude(void* Context, const SdJsonRpcRequest*,
		SdJsonRpcReply*) {
	//TODO:
}

void Drone::OnRpcCommandGetAltitude(void* Context, const SdJsonRpcRequest*,
		SdJsonRpcReply*) {
	//TODO:
}

void Drone::OnRpcCommandGetRpcSpec(
	void* context,
	const SdJsonRpcRequest* req,
	SdJsonRpcReply* rep)
{
	std::string methodName;
	SdJsonValue paramsSpec;
	SdJsonValue resultSpec;
	req->Params.AsStringSafe(&methodName);
	if (!Only()->m_rpcDispatch->GetParamsSpec(methodName,&paramsSpec) ||
			!Only()->m_rpcDispatch->GetResultSpec(methodName,&resultSpec)) {
		rep->ErrorCode = SD_JSONRPC_ERROR_INVALID_METHOD_PARAMS;
		rep->ErrorMessage = "Requested spec for unsupported method";
	} else {
		SdJsonObject obj;
		obj.AddMember("params",paramsSpec);
		obj.AddMember("result",resultSpec);
		rep->Results.SetValueAsObject(&obj);
		rep->ErrorCode = SD_JSONRPC_ERROR_SUCCESS;
	}
	rep->Id = req->Id;
}

void Drone::OnRpcCommandGetRpcList(
	void* context,
	const SdJsonRpcRequest* req,
	SdJsonRpcReply* rep)
{
	std::vector<std::string> rpcList;
	SdJsonArray jarr;
	Only()->m_rpcDispatch->GetRegisteredMethods(rpcList);
	for (size_t i = 0; i < rpcList.size(); i++) {
		jarr.AddElement(SdJsonValue(rpcList[i]));
	}
	rep->Results.SetValueAsArray(&jarr);
	rep->ErrorCode = SD_JSONRPC_ERROR_SUCCESS;
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
	if (0 == pluginName || 0 == (strcmp(pluginName,SD_PLUGIN_PIDPILOT))){
		PidPilot* pilot = new PidPilot();
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
#if 0
	if (0 == pluginName || 0 == (strcmp(pluginName,SD_PLUGIN_IMU_KALMAN_FILTER))){
		KalmanAttitudeFilter* kf = new KalmanAttitudeFilter();
		if (0 != kf) {
			kf->AttachToChain(droneContext,attachPlugin);
			kf->Release();
		}
	}
#endif
	return 0;
}
