/*
 * sdroneapi.h
 *
 *  Created on: August 1, 2013
 *      Author: svassilev
 *
 *  SD (SigmaDrone) API
 */

#ifndef SDRONE_API_H_
#define SDRONE_API_H_

#include <stdarg.h>
#include "matrix.h"

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#define SD_DEFAULT_PORT 2222

#define SD_LOG_LEVEL_NONE 0
#define SD_LOG_LEVEL_ERROR 1
#define SD_LOG_LEVEL_WARNING 2
#define SD_LOG_LEVEL_INFO 3
#define SD_LOG_LEVEL_VERBOSE 4
#define SD_LOG_LEVEL_DEBUG 5

typedef enum _SdCommand {
	SD_COMMAND_NONE = 0,
	SD_COMMAND_RUN,
	SD_COMMAND_RESET,
	SD_COMMAND_EXIT,
	SD_COMMAND_LOAD_PLUGIN,
	SD_COMMAND_UNLOAD_PLUGIN
} SdCommand;

typedef enum _SdDroneType
{
	SD_DRONE_TYPE_INVALID,
	SD_DRONE_TYPE_HELLI,
	SD_DRONE_TYPE_AIRPLANE,
	SD_DRONE_TYPE_QUADROTOR,
	SD_DRONE_TYPE_HEXROTOR,
	SD_DRONE_TYPE_OCTOROTOR
} SdDroneType;

typedef enum _SdPilotType
{
	SD_PILOT_TYPE_NONE = 0,
	SD_PILOT_TYPE_QUADROTOR,
	SD_PILOT_TYPE_PID,
} SdPilotType;

typedef struct _SdImuDeviceConfig
{
	const char* DeviceName;

	/*
	 * Sampling rate
	 */
	int SamplingRate;

	/*
	 * Degrees per second for gyro and number of Gs for accelerometer
	 */
	int Scale;

	/*
	 * Max value of a read sample
	 */
	int MaxReading;

	/*
	 * For how many samples are used for gyro bias calculation and earth G
	 * vector
	 */
	int NumBiasSamples;

	/*
	 * How many samples to collect prior to firing an interrupt
	 */
	int Watermark;
} SdImuDeviceConfig;

typedef struct _SdServoConfig
{
	const char* DeviceName;
	int Rate;
	uint32_t BitCount;
	uint32_t ChannelMask;
} SdServoConfig;

typedef struct _SdQuadRotorConfig
{
	/*
	 * Mapping between motor numbers and actual servo channels.
	 * Motors are paired, i.e. Motor[0] and Motor[2] map to the motors
	 * lying on the X axis and Motor[1] and Motor[3] map to the Y axis
	 */
	int Motor[4];
	int ImuAngleAxisZ;  // IMU axes may not be aligned with the quadrotor axes
	double Kp;
	double Ki;
	double Kd;
} SdQuadRotorConfig;

typedef struct _SdDroneConfig
{
	SdImuDeviceConfig Gyro;
	SdImuDeviceConfig Accel;
	SdImuDeviceConfig Mag;
	SdServoConfig Servo;
	SdQuadRotorConfig Quad;
	SdPilotType Pilot;
	Matrix3d GyroMap;
	Matrix3d AccMap;
	Matrix3d MagMap;
	const char* ServerAddress;
	int ServerPort;
	int LogRotationMatrix;
	int LogLevel;
	const char* LogFileName;

	/*
	 * How many times per second the logger should log
	 */
	double LogRate;
} SdDroneConfig;

typedef enum _SdImuDeviceId
{
	SD_IMU_DEVICE_UNKNOWN = 0,
	SD_IMU_DEVICE_GYRO,
	SD_IMU_DEVICE_ACCEL,
	SD_IMU_DEVICE_MAG
} SdImuDeviceId;

typedef struct _SdImuData
{
	short3d_t gyro[64];
	short3d_t acc[64];
	short3d_t mag[64];
	size_t gyro_samples;
	size_t acc_samples;
	size_t mag_samples;
} SdImuData;


typedef double SdPluginAltitude;

#define SD_ALTITUDE_FIRST 10000
#define SD_ALTITUDE_LAST  60000-1


/*
 * Internal use
 */
#define SD_ALTITUDE_GROUP_INVALID 0
#define SD_ALTITUDE_INVALID SD_ALTITUDE_GROUP_INVALID

/*
 * All the plugins directly interfacing with devices (sensors/servos/etc)
 * belong to this group. There should be only one such plugin per device
 */
#define SD_ALTITUDE_GROUP_DEVICE 1*SD_ALTITUDE_FIRST //10000

/*
 * Sensor data pre-filters - this may be axis re-mapping or
 * subtracting the bias or a low-pass filter on the input data
 */
#define SD_ALTITUDE_GROUP_LOWER_FILTER 2*SD_ALTITUDE_FIRST //20000

/*
 * Plugins that process, i.e. make sense/transform the device data
 * belong to this group, e.g. 1) IMU filter - it takes data from
 * accel/gyro/mag and produces an attitude estimate
 */
#define SD_ALTITUDE_GROUP_FILTER 3*SD_ALTITUDE_FIRST //30000

/*
 * Plugins that may desire to filter the converted data will occupy
 * this altitude. Navigator will be such plugin, its responsibilities will
 * be to issue commands to the pilot group, by taking the input
 * from all the sensor chains and combining it with the
 * commands from the CCC and sending to the pilot the desired thrust vector
 * and desired attitude.
 */
#define SD_ALTITUDE_GROUP_UPPER_FILTER 4*SD_ALTITUDE_FIRST //40000

/*
 * Application specific plugins, e.g. save data from camera/image processing/etc.
 * The pilot plugin will be part of this group. Note: the pilot must be above the
 * navigator
 */
#define SD_ALTITUDE_GROUP_APP 		5*SD_ALTITUDE_FIRST //50000

inline  SdPluginAltitude SD_ALTITUDE_FIRST_IN_GROUP(SdPluginAltitude alt) {
	return (int)(alt)/(int)SD_ALTITUDE_FIRST*(int)SD_ALTITUDE_FIRST;
}

inline  SdPluginAltitude SD_ALTITUDE_LAST_IN_GROUP(SdPluginAltitude alt) {
	return SD_ALTITUDE_FIRST_IN_GROUP(alt) + SD_ALTITUDE_FIRST - 1;
}

/*
 * Internal use
 */
#define SD_ALTITUDE_GROUP_RESERVED_UPPER 10000000


/*
 * SD_IOCODE_RECEIVE
 * Used by plugins directly talking to devices to notify the chain
 * about the newly received data from the device/sen
 */
#define SD_IOCODE_RECEIVE 	0

/*
 * SD_IOCODE_SEND
 * Plugins can send data to each other using this code. The pilot
 * plugin sends data to the servo plugin using this IO code
 */
#define SD_IOCODE_SEND 		1

/*
 * SD_IOCODE_GET_STATE
 * Plugins can obtain state from other plugins using this IO code
 */
#define SD_IOCODE_GET_STATE 2

/*
 * SD_IOCODE_COMMAND
 * Used by the framework to notify all the plugins about newly received
 * command
 */
#define SD_IOCODE_COMMAND	3

#define SD_IOCODE_TO_FLAG(iocode) (1<<(iocode))

typedef uint64_t SdDeviceId;
#define SD_DEVICEID_IMU 		0
#define SD_DEVICEID_SERVO 		1
#define	SD_DEVICEID_BAR			2
#define SD_DEVICEID_GPS			3
#define SD_DEVICEID_CAMERA		4
#define SD_DEVICEID_SONAR		5
#define SD_DEVICEID_FILTER		32
#define SD_DEVICEID_LAST		63
#define SD_DEVICEID_ALL		((uint64_t)-1)

#define SD_DEVICEID_TO_FLAG(dev) ((SdDeviceId)1<<(dev))

/*
 * Internal plugins. Plugin names must not contain space
 */
#define SD_PLUGIN_IMU_DEVICE "sd.imudevice"
#define SD_PLUGIN_IMU_BIAS "sd.imubias"
#define SD_PLUGIN_IMU_REMAP "sd.imuremap"
#define SD_PLUGIN_IMU_LOWPASSFILTER "sd.imulowpassfilter"
#define SD_PLUGIN_IMU_FILTER "sd.imufilter"
#define SD_PLUGIN_QUADPILOT  "sd.quadpilot"
#define SD_PLUGIN_PIDPILOT  "sd.pidpilot"
#define SD_PLUGIN_NAVIGATOR "sd.navigator"
#define SD_PLUGIN_SERVO_PCA9685 "sd.servo_pca9685"
#define SD_PLUGIN_TRACELOG "sd.tracelog"

#define SD_PLUGIN_VERSION "SigmaDronePlugin"


/*
 * SigmaDrone specific error codes
 */
#define SD_ESUCCESS 	0
#define SD_EFIRST 		20000
#define SD_ESTOP_DISPATCH  	SD_EFIRST+0
#define SD_EINVALID_DEPENDENCY SD_EFIRST+1


typedef int (*SdPluginAttachFunc)(
		IN void* droneContext,
		IN struct IPlugin* plugin,
		IN SdPluginAltitude desiredAltitude,
		IN const char* const attachAbove[],
		IN const char* const attachBelow[],
		OUT struct IPluginRuntime** pluginRuntime
		);

/*
 * This is the entry point for a plugin life time. Very first API
 * invoked by the SigmaDrone framework to tell the shared object to
 * initialize its plugins. If plugin name is not specified, then
 * the plugin module/.so should load all the plugins that are hosted
 * by the module.
 */
extern "C" int SdPluginInitialize(
		IN void* droneContext,
		IN SdPluginAttachFunc attachPlugin,
		IN const char* pluginName
		);

typedef struct _SdServoIoData
{
	uint32_t numChannels;
	int channels[16];
	double value [16];
} SdServoIoData;

typedef struct _SdIoData
{
	static const uint32_t TYPE_INVALID = 0;
	static const uint32_t TYPE_IMU = 1;
	static const uint32_t TYPE_VOID_PTR = 2; /*any struct*/
	static const uint32_t TYPE_VECTOR4D = 3;
	static const uint32_t TYPE_BLOB = 4;
	static const uint32_t TYPE_FILE = 5;
	static const uint32_t TYPE_QUATERNION = 6;
	static const uint32_t TYPE_COMMAND_ARGS = 7;
	static const uint32_t TYPE_SERVO = 8;

	uint32_t dataType; /*one of the above*/
	union
	{
		const SdImuData* asImuData;
		struct CommandArgs* asCommandArgs;
		const SdServoIoData* asServoData;
		const void* asVoidPtr;
		const Vector4d* asVector4d;
		FILE* asFile;
		const QuaternionD* asQuaternion;
		struct
		{
			uint32_t blobSize;
			const void* blob;
		} asBlob;
	};
	IPlugin* dataOwner;
	//SdIoData* next;
} SdIoData;

typedef struct _SdIoPacket
{
	static void Init(
		_SdIoPacket* io,
		uint32_t ioCode,
		SdDeviceId deviceType,
		const char* pluginName)
	{
		memset(io,0,sizeof(*io));
		io->size = sizeof(*io);
		io->ioCode = ioCode;
		io->deviceId = deviceType;
		io->pluginName = pluginName;
	}

	static const uint32_t FLAG_DISPATCH_UP = 0x0001;

	uint32_t size;
	uint32_t flags;
	SdPluginAltitude startPluginAltitude;
	uint32_t ioCode;
	uint32_t reserved[8];

	const char* pluginName;
	SdDeviceId deviceId;

	/*
	 * inData is always used to convey data across the chain, think of
	 * it as an input parameter of a function call.
	 */
	SdIoData inData;

	/*
	 * outData is used only to return back data to the issuer
	 * of the IO operation; think of it as an output parameter
	 * of the function call
	 */
	SdIoData outData;

	/*
	 * Drone state
	 */
	const QuaternionD* attitudeQ;
	const QuaternionD* targetQ;
	const QuaternionD* motorAxisQ;
	const Vector3d* velocity;
	const Vector3d* gyroDataDps;
	const Vector3d* accelData;
	const Vector3d* magData;
	const Vector3d* earthG;
	const Vector4d* motors;
	const Vector3d* errAxisPid;
	const Vector3d* errAxis;
	const Vector3d* errAxisP;
	const Vector3d* errAxisI;
	const Vector3d* errAxisD;
	double errAngle;
	double droneAltitude;
	double deltaTime; /*elapsed time since the last sample*/
	double timeToReadSensors;
	// TODO: Gps/Temperature
} SdIoPacket;

/*
 * If SD_FLAG_DISPATCH_DOWN set the IO is dispatched from higher to lower
 * altitude. The IO is dispatched starting with caller's  altitude unless
 * SD_FLAG_DISPATCH_END_TO_END is set, in which case all the plugins get
 * to look at the IO packet
 */
#define SD_FLAG_DISPATCH_DOWN				0x0001

/*
 * If SD_FLAG_DISPATCH_TO_ALL is set the IO is dispatched to all the
 * plugins. The direction is controlled with SD_FLAG_DISPATCH_DOWN
 */
#define SD_FLAG_DISPATCH_TO_ALL		0x0002

/*
 * If SD_FLAG_DISPATCH_SKIP_CALLER is set the IO will not be dispatched to
 * the calling plugin. This flag is ignored if SD_FLAG_DISPATCH_TO_ALL is
 * not set
 */
#define SD_FLAG_DISPATCH_SKIP_CALLER	0x0004

struct IPluginRuntime
{
	/*
	 * Detaches plugin from the chain. After this call returns, the plugin
	 * will not receive any further calls from the framework. If the plugin
	 * wants to play again, then it must call SdPluginAttachFunc (note:
	 * SdPluginAttachFunc will fail if the Drone framework is in the process
	 * of tearing down).
	 */
	virtual void DetachPlugin() = 0;

	/*
	 * Instructs the runtime to schedule an IO dispatch thread; as a
	 * result of this call IPlugin::IoDispatchThread will be invoked in
	 * a tight loop, which should be used by the plugin to pull data from the
	 * sensor device. Normally this API will be invoked only by plugins
	 * interfacing directly with devices - SD_ALTITUDE_GROUP_DEVICE. Dispatch
	 * thread can be started/stopped as many times as the plugin desires. Only one
	 * thread per plugin will be started.
	 */
	virtual int StartStopIoDispatchThread(
			IN bool start
			) = 0;

	/*
	 * Must be called by the plugin in order to become part of the IO
	 * processing. Plugin will normally setup masks using SD_DEVICEID_TO_FLAG
	 * and SD_IOCODE_TO_FLAG. This API plays the role of "soft" attach/detach
	 * to/from the chain, and it can be called any times after the plugin
	 * attaches to the chain.
	 */
	virtual void SetIoFilters(
			IN SdDeviceId deviceTypeMask,
			IN uint32_t ioCodeMask
			) = 0;

	/*
	 * Dispatches SdIoPacket "up/down" the plugin chain.
	 */
	virtual int DispatchIo(
			IN OUT SdIoPacket* iop,
			IN uint32_t dispatchFlags
			) = 0;

	virtual SdDroneType GetSdDroneType(
			) = 0;

	/*
	 * Returns the actual plugin altitude assigned by the framework
	 */
	virtual SdPluginAltitude GetMyAltitude() = 0;

	virtual void Log(
			int logLevel,
			const char* format,
			...
			) = 0;

	virtual void Logv(
			int logLevel,
			const char* format,
			va_list args
			) = 0;

#if 0
	/*
	 * Returns altitude for any plugin
	 */
	virtual SdPluginAltitude GetPluginAltitude(
			IN IPlugin*
			) = 0;

	virtual struct IPluginIterator* GetPluginSnapshot() = 0;
#endif

protected:
	virtual ~IPluginRuntime() {}
};

struct IPluginIterator
{
	virtual int AddRef() = 0;
	virtual int Release() = 0;

	virtual void Reset() = 0;
	virtual IPlugin* GetNextPlugin() = 0;

protected:
	virtual ~IPluginIterator() {}
};

struct IPlugin
{
	virtual int AddRef() = 0;
	virtual int Release() = 0;

	/*
	 * Called to tell the plugin to perform any post init steps and update
	 * its configuration; when this API completes, the plugin must be ready
	 * to process IOs, provided the plugin has called SetIoFilters.
	 */
	virtual int Start(
			CommandArgs*
			)  = 0;
	/*
	 * IO processing has stopped on the chain and the drone hardware is being
	 * reset.
	 */
	static const int FLAG_STOP_AND_DETACH = 0x01;
	virtual void Stop(
			int flags
			)  = 0;

	/*
	 * Returns the name of the plugin - should be unique and contain only
	 * printable characters; please avoid using ' '.
	 */
	virtual const char* GetName(
			) = 0;

	/*
	 * Returns the device type
	 */
	virtual SdDeviceId GetDeviceId(
			) = 0;

	/*
	 * Returns the defined const SD_PLUGIN_VERSION. Used by the runtime to
	 * figure out whether the plugin compiled against same or older header.
	 * The runtime will support old plugins, but newer plugins will not
	 * be able to run against an older runtime.
	 */
	virtual const char* GetVersion(
			) = 0;

	/*
	 * Returns the name of the .so file that implements the plugin code
	 */
	virtual const char* GetDlFileName(
			) = 0;

#if 0
	/*
	 * Returns a list of plugins that must have lower altitude than _this_
	 * plugin. If there are no dependencies NULL should be returned.
	 */
	virtual const char* const* GetLowerPluginDependencies(
			) = 0;

	/*
	 * Returns a list of plugins that must have higher altitude than _this_
	 * plugin. If there are no dependencies NULL should be returned.
	 */
	virtual const char* const* GetUpperPluginDependencies(
			) = 0;
#endif

	/*
	 * IO processing callback. The plugin is free to modify any of the ioPacket
	 * fields.
	 * Return code:
	 *  - SD_ESUCCESS - the IO will be dispatched to the next plugin
	 *  - Any of the system error codes - see errno.h Note: if code 1..SD_EFIRST is
	 *  returned the IO dispatch will stop, but an error will be logged.
	 *  - SD_ESTOP_DISPATCH - the framework stops the dispatch operation for the IO.
	 */
	virtual int IoCallback(
			SdIoPacket* ioPacket
			) = 0;

	/*
	 * Invoked by the runtime only if the plugin has previously called
	 * IPluginRuntime::StartIoDispatchThread(true). It is perfectly normal
	 * IoDispatchthread() to block until data is available from the device.
	 * Plugins should not implement "while (pluginRunning) {}" loop inside of
	 * this routine as the latter will be invoked in a tight loop by the runtime,
	 * until non-zero error code is returned or StartStopIoDispatchThread(false) is
	 * called
	 */
	virtual int IoDispatchThread() = 0;

protected:
	virtual ~IPlugin() {}
};

struct CommandArgs
{
	virtual SdCommand GetCommand() = 0;
	virtual const char* GetArgAsString(
			int index
			) = 0;
	virtual int GetArgAsInt(
			int index
			) = 0;
	virtual int64_t GetArgAsInt64(
			int index
			) = 0;
	virtual double GetArgAsDouble(
			int index
			) = 0;
	virtual int /*-1 or index*/ IsArgPresent(
			const char* argName
			) = 0;
	virtual const char** GetArgv(
			) = 0;
	virtual int GetArgc(
			) = 0;

	virtual double GetMinThrust() = 0;
	virtual double GetMaxThrust() = 0;
	virtual double GetDesiredThrust() = 0;
	virtual const QuaternionD* GetTargetAttitude() = 0;
	virtual const SdDroneConfig* GetDroneConfig() = 0;
protected:
	virtual ~CommandArgs() {}
};

#endif // SDRONE_API_H_
