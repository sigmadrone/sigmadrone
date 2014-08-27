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
#include "dronerpcclient.h"
#include "plugincommandparams.h"

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
	if (args.IsDaemon()) {
		daemon_init();
	}

	m_thrustValues.thrust = args.GetDesiredThrust();
	m_thrustValues.minThrust = args.GetMinThrust();
	m_thrustValues.maxThrust = args.GetMaxThrust();

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
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_GET_CONFIG),
			OnRpcCommandGetConfig,
			this);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_SET_CONFIG),
			OnRpcCommandSetConfig,
			this);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_PING),
			OnRpcCommandPing,
			this);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_GET_THRUST),
			OnRpcCommandGetThrust,
			this);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_SET_THRUST),
			OnRpcCommandSetThrust,
			this);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_GET_ATTITUDE),
			OnRpcCommandGetAttitude,
			this);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_SET_TARGET_ATTITUDE),
			OnRpcCommandSetTargetAttitude,
			this);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_GET_ALTITUDE),
			OnRpcCommandGetAltitude,
			this);
	m_rpcDispatch->AddRequestCallback(
			SdCommandCodeToString(SD_COMMAND_SET_TARGET_ALTITUDE),
			OnRpcCommandSetTargetAltitude,
			this);

	//
	// Execute the command that came with the command line
	//
	switch(args.GetCommand()) {
	case SD_COMMAND_RUN: {
		SdJsonValue jsonArgs;
		DroneRpcClient::BuildJsonDroneConfigFromCmdLineArgs(&jsonArgs, args);
		OnRun(&jsonArgs);
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
	int err = m_pluginChain.ExecuteCommand(&params);
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
	m_pluginChain.ExecuteCommand(&params);

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
		if (0 == (err = m_pluginChain.ExecuteCommand(&params))) {
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
	PluginCommandParams params(SD_COMMAND_SET_THRUST,data,0);
	m_thrustValues = thrustVals;
	return m_pluginChain.ExecuteCommand(&params);
}

int Drone::OnSetConfig(
		const SdDroneConfig& config,
		const IJsonValue* rpcParams)
{
	SdIoData data(&config);
	PluginCommandParams params(SD_COMMAND_SET_CONFIG,data,rpcParams);
	m_droneConfig.m_config = config;
	return m_pluginChain.ExecuteCommand(&params);
}

int Drone::OnSetTargetQuaternion(const QuaternionD& targetQ)
{
	SdIoData data(&targetQ);
	PluginCommandParams params(SD_COMMAND_SET_TARGET_ATTITUDE,data,0);
	m_targetQ = targetQ;
	return m_pluginChain.ExecuteCommand(&params);
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

void Drone::OnRpcCommandRun(
		void* context,
		const SdJsonRpcRequest* req,
		SdJsonRpcReply* rep)
{
	assert(context == Only());
	int err;
	rep->ErrorCode = SD_JSONRPC_ERROR_SUCCESS;
	if (!Drone::ParseJsonDroneConfig(&req->Params,&Only()->m_droneConfig.m_config)) {
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
	SdJsonObject jobj;
	rep->ErrorCode = SD_JSONRPC_ERROR_SUCCESS;
	if (!Drone::BuildJsonDroneConfig(&jobj,Only()->m_droneConfig.m_config)) {
		rep->ErrorCode = SD_JSONRPC_ERROR_PARSE;
	} else {
		rep->Results.SetValueAsObject(&jobj);
	}
	rep->Id = req->Id;
}

void Drone::OnRpcCommandPing(
		void* context,
		const SdJsonRpcRequest* req,
		SdJsonRpcReply* rep)
{
	double timestamp = 0;
	assert(context == Only());
	if (req->Params.AsDoubleSafe(&timestamp)) {
		printf("Sending ping reply %f\n",timestamp);
		rep->ErrorCode = SD_JSONRPC_ERROR_SUCCESS;
		rep->Results.SetValueAsString(SD_PING_REPLY_DATA);
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
	if (!Drone::ParseJsonDroneConfig(&req->Params,&config)) {
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
	if (!Drone::ParseJsonThrust(&req->Params, &thrust)) {
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
	SdJsonObject res;
	const SdThrustValues& thrust = Only()->m_thrustValues;
	Drone::BuildJsonThrustParams(&res,thrust.thrust,
			thrust.minThrust,thrust.maxThrust);
	rep->Results.SetValueAsObject(&res);
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
	if (!Drone::ParseJsonTargetQuaternion(&req->Params,&targetQ)) {
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
}

void Drone::OnRpcCommandSetTargetAltitude(void* Context, const SdJsonRpcRequest*,
		SdJsonRpcReply*) {
	//TODO:
}

void Drone::OnRpcCommandGetAltitude(void* Context, const SdJsonRpcRequest*,
		SdJsonRpcReply*) {
	//TODO:
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

bool Drone::ParseJsonImuConfig(
	const IJsonObject* obj,
	SdImuDeviceConfig* imu)
{
	if (obj) {
		obj->GetMember("DeviceName")->AsStringSafe(&imu->DeviceName);
		obj->GetMember("SamplingRate")->AsIntSafe(&imu->SamplingRate);
		obj->GetMember("Scale")->AsIntSafe(&imu->Scale);
		obj->GetMember("MaxReading")->AsIntSafe(&imu->MaxReading);
		obj->GetMember("Watermark")->AsIntSafe(&imu->Watermark);
		obj->GetMember("NumBiasSamples")->AsIntSafe(&imu->NumBiasSamples);
		const IJsonArray* arr;
		if (0 != (arr = obj->GetMember("CoordinateMap")->AsArray())) {
			if (arr->ElementCount() == 9) {
				for (uint32_t i = 0; i < 9; i++) {
					imu->CoordinateMap.at(i/3,i%3)=arr->GetElement(i)->AsInt();
				}
			}
		}
	}
	return !!obj;
}

bool Drone::ParseJsonDroneConfig(
		const IJsonValue* jsonRpcParams,
		SdDroneConfig* droneCfg)
{
	const IJsonObject* config = 0;
	const IJsonObject* obj = 0;
	const IJsonArray* arr = 0;
	const IJsonObject* params = 0;

	if (jsonRpcParams->GetType() != SD_JSONVALUE_OBJECT) {
		return false;
	}
	params = jsonRpcParams->AsObject();

	if (0 == (config = params->GetMember("GlobalConfig")->AsObject())) {
		return false;
	}

	if (config->IsMemberPresent("Gyro")) {
		ParseJsonImuConfig(config->GetMember("Gyro")->AsObject(),
				&droneCfg->Gyro);
	}
	if (config->IsMemberPresent("Accelerometer")) {
		ParseJsonImuConfig(config->GetMember("Accelerometer")->AsObject(),
				&droneCfg->Accel);
	}
	if (config->IsMemberPresent("Magnetometer")) {
		ParseJsonImuConfig(config->GetMember("Magnetometer")->AsObject(),
				&droneCfg->Mag);
	}

	if (0 != (obj = config->GetMember("Servo")->AsObject())) {
		obj->GetMember("DeviceName")->AsStringSafe(&droneCfg->Servo.DeviceName);
		obj->GetMember("SamplingRate")->AsIntSafe(&droneCfg->Servo.Rate);
		obj->GetMember("ChannelMask")->AsUintSafe(&droneCfg->Servo.ChannelMask);
		obj->GetMember("BitCount")->AsUintSafe(&droneCfg->Servo.BitCount);
	}

	if (0 != (obj = config->GetMember("QuadPilot")->AsObject())) {
		obj->GetMember("Kp")->AsDoubleSafe(&droneCfg->Quad.Kp);
		obj->GetMember("Ki")->AsDoubleSafe(&droneCfg->Quad.Ki);
		obj->GetMember("Kd")->AsDoubleSafe(&droneCfg->Quad.Kd);
		if (0 != (arr = obj->GetMember("MotorToServoMap")->AsArray()) &&
			arr->ElementCount() == 4) {
			for (size_t i = 0; i < 4; i++) {
				droneCfg->Quad.Motor[i] = (uint32_t)arr->GetElement(i)->AsInt();
			}
		}

		if (0 != (arr = obj->GetMember("ImuCoordinateAngles")->AsArray()) &&
			arr->ElementCount() == 3) {
			droneCfg->Quad.ImuAngleAxisZ = arr->GetElement(2)->AsInt();
		}
	}

	if (0 != (obj = config->GetMember("ControlChannel")->AsObject())) {
		obj->GetMember("ServerAddress")->AsStringSafe(&droneCfg->ServerAddress);
		obj->GetMember("ServerPort")->AsIntSafe(&droneCfg->ServerPort);
	}
	if (config->GetMember("LogLevel")->GetType() == SD_JSONVALUE_STRING) {
		droneCfg->LogLevel = SdStringToLogLevel(
				config->GetMember("LogLevel")->AsString().c_str());
	}

	config->GetMember("LogFileName")->AsStringSafe(&droneCfg->LogFileName);
	config->GetMember("LogRotationMatrix")->AsIntSafe(&droneCfg->LogRotationMatrix);
	config->GetMember("LogRate")->AsDoubleSafe(&droneCfg->LogRate);
	return true;
}


bool Drone::ParseJsonTargetQuaternion(
		const IJsonValue* args, QuaternionD* targetQ)
{
	bool ret = false;
	if (args->GetType() != SD_JSONVALUE_OBJECT) {
		return ret;
	}
	const IJsonObject* jobj = args->AsObject();

	if (jobj->GetMember("TargetQuaternion")->GetType() == SD_JSONVALUE_ARRAY) {
		const IJsonArray* jarr = jobj->GetMember("TargetQuaternion")->AsArray();
		if (jarr->ElementCount() == 4) {
			targetQ->w = jarr->GetElement(0)->AsDouble();
			targetQ->x = jarr->GetElement(1)->AsDouble();
			targetQ->y = jarr->GetElement(2)->AsDouble();
			targetQ->z = jarr->GetElement(3)->AsDouble();
			ret = true;
		}
	}
	return ret;
}

bool Drone::ParseJsonThrust(
		const IJsonValue* rpcParams,
		SdThrustValues* thrustVals)
{
	if (rpcParams->GetType() != SD_JSONVALUE_OBJECT) {
		return false;
	}
	bool ret = true;
	const IJsonObject* jobj = rpcParams->AsObject();
	ret = ret && jobj->GetMember("Thrust")->AsDoubleSafe(&thrustVals->thrust);
	ret = ret && jobj->GetMember("MaxThrust")->AsDoubleSafe(&thrustVals->maxThrust);
	ret = ret && jobj->GetMember("MinThrust")->AsDoubleSafe(&thrustVals->minThrust);
	return ret;
}

bool Drone::BuildJsonDroneConfig(
		SdJsonObject* jsonDroneConfig,
		const SdDroneConfig& droneCfg)
{
	bool ret = true;
	SdJsonObject tmpObj;
	SdJsonArray tmpArr;

	jsonDroneConfig->Reset();
	ret = ret && BuildJsonImuConfig(&tmpObj,droneCfg.Gyro);
	ret = ret && jsonDroneConfig->AddMember("Gyro",SdJsonValue(tmpObj));
	ret = ret && BuildJsonImuConfig(&tmpObj,droneCfg.Accel);
	ret = ret && jsonDroneConfig->AddMember("Accelerometer",SdJsonValue(tmpObj));
	ret = ret && BuildJsonImuConfig(&tmpObj,droneCfg.Mag);
	ret = ret && jsonDroneConfig->AddMember("Magnetometer",SdJsonValue(tmpObj));

	tmpObj.Reset();
	ret = ret && tmpObj.AddMember("DeviceName",SdJsonValue(droneCfg.Servo.DeviceName));
	ret = ret && tmpObj.AddMember("SamplingRage",SdJsonValue(droneCfg.Servo.Rate));
	ret = ret && tmpObj.AddMember("ChannelMask",SdJsonValue(droneCfg.Servo.ChannelMask));
	ret = ret && tmpObj.AddMember("BitCount",SdJsonValue(droneCfg.Servo.BitCount));
	ret = ret && jsonDroneConfig->AddMember("Servo",SdJsonValue(tmpObj));

	tmpObj.Reset();
	tmpArr.Reset();
	for (size_t i = 0; i < ARRAYSIZE(droneCfg.Quad.Motor); i++) {
		ret = ret && tmpArr.AddElement(SdJsonValue(droneCfg.Quad.Motor[i]));
	}
	ret = ret && tmpObj.AddMember("MotorToServoMap",SdJsonValue(tmpArr));
	ret = ret && tmpArr.AddElement(SdJsonValue((int32_t)0));
	ret = ret && tmpArr.AddElement(SdJsonValue((int32_t)0));
	ret = ret && tmpArr.AddElement(SdJsonValue(droneCfg.Quad.ImuAngleAxisZ));
	ret = ret && tmpObj.AddMember("ImuCoordinateAngles",SdJsonValue(tmpArr));
	ret = ret && tmpObj.AddMember("Kp",SdJsonValue(droneCfg.Quad.Kp));
	ret = ret && tmpObj.AddMember("Ki",SdJsonValue(droneCfg.Quad.Ki));
	ret = ret && tmpObj.AddMember("Kd",SdJsonValue(droneCfg.Quad.Kd));
	ret = ret && jsonDroneConfig->AddMember("QuadPilot",SdJsonValue(tmpObj));

	tmpObj.Reset();
	ret = ret && tmpObj.AddMember("ServerAddress",SdJsonValue(
			droneCfg.ServerAddress));
	ret = ret && tmpObj.AddMember("ServerPort",SdJsonValue(droneCfg.ServerPort));
	ret = ret && jsonDroneConfig->AddMember("ControlChannel",SdJsonValue(tmpObj));

	ret = ret && jsonDroneConfig->AddMember("LogLevel",
			SdLogLevelToString(droneCfg.LogLevel));
	ret = ret && jsonDroneConfig->AddMember("LogRotationMatrix",
			SdJsonValue((bool)droneCfg.LogRotationMatrix));
	ret = ret && jsonDroneConfig->AddMember("LogFileName",SdJsonValue(
			droneCfg.LogFileName));
	ret = ret && jsonDroneConfig->AddMember("LogRate",SdJsonValue(
			droneCfg.LogRate));

	return ret;
}

bool Drone::BuildJsonImuConfig(
		SdJsonObject* jobj,
		const SdImuDeviceConfig& imuCfg)
{
	bool ret = true;
	jobj->Reset();
	ret = ret && jobj->AddMember("DeviceName",SdJsonValue(imuCfg.DeviceName));
	ret = ret && jobj->AddMember("SamplingRage",SdJsonValue(imuCfg.SamplingRate));
	ret = ret && jobj->AddMember("Scale",SdJsonValue(imuCfg.Scale));
	ret = ret && jobj->AddMember("MaxReading",SdJsonValue(imuCfg.MaxReading));
	ret = ret && jobj->AddMember("Watermark",SdJsonValue(imuCfg.Watermark));
	ret = ret && jobj->AddMember("NumBiasSamples",SdJsonValue(imuCfg.NumBiasSamples));
	SdJsonArray jarr;
	for (size_t i = 0; i < 3; i++) {
		for (size_t j = 0; j < 3; j++) {
			ret = ret && jarr.AddElement(SdJsonValue(imuCfg.CoordinateMap.at(i,j)));
		}
	}
	ret = ret && jobj->AddMember("CoordinateMap",SdJsonValue(jarr));
	return ret;
}

bool Drone::BuildJsonThrustParams(
		SdJsonObject* jsonThrustParams,
		double thrust,
		double minThrust,
		double maxThrust)
{
	bool ret = true;
	jsonThrustParams->Reset();
	ret = ret && jsonThrustParams->AddMember("Thrust",SdJsonValue(thrust));
	ret = ret && jsonThrustParams->AddMember("MinThrust",SdJsonValue(minThrust));
	ret = ret && jsonThrustParams->AddMember("MaxThrust",SdJsonValue(maxThrust));
	return ret;
}

bool Drone::BuildJsonPingParams(
		SdJsonValue* jsonPingParams,
		double timestamp)
{
	*jsonPingParams = timestamp;
	return jsonPingParams->SetValueAsDouble(timestamp);
}
