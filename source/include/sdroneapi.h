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

#ifndef SDRONE_API_H_
#define SDRONE_API_H_

#include <stdarg.h>
#include <string>
#include "d3math.h"
#include "jsonrpc.h"
#include "axesdata.h"

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef ARRAYSIZE
#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))
#endif

#define SD_DEFAULT_TCP_PORT 2222

#define SD_LOG_LEVEL_NONE 0
#define SD_LOG_LEVEL_ERROR 1
#define SD_LOG_LEVEL_WARNING 2
#define SD_LOG_LEVEL_INFO 3
#define SD_LOG_LEVEL_VERBOSE 4
#define SD_LOG_LEVEL_DEBUG 5

__inline uint32_t SdStringToLogLevel(const char* _str) {
	if (_str) {
		std::string str(_str);
		if (std::string("SD_LOG_LEVEL_NONE") == str) { return SD_LOG_LEVEL_NONE; }
		if (std::string("SD_LOG_LEVEL_ERROR") == str) { return SD_LOG_LEVEL_ERROR; }
		if (std::string("SD_LOG_LEVEL_WARNING") == str) { return SD_LOG_LEVEL_WARNING; }
		if (std::string("SD_LOG_LEVEL_INFO") == str) { return SD_LOG_LEVEL_INFO; }
		if (std::string("SD_LOG_LEVEL_VERBOSE") == str) { return SD_LOG_LEVEL_VERBOSE; }
		if (std::string("SD_LOG_LEVEL_DEBUG") == str) { return SD_LOG_LEVEL_DEBUG; }
	}
	return SD_LOG_LEVEL_VERBOSE;
}

__inline const char* SdLogLevelToString(uint32_t level) {
	const char* asStr[] = { "SD_LOG_LEVEL_NONE","SD_LOG_LEVEL_ERROR",
			"SD_LOG_LEVEL_WARNING","SD_LOG_LEVEL_INFO","SD_LOG_LEVEL_VERBOSE",
			"SD_LOG_LEVEL_DEBUG"
	};
	return (level < ARRAYSIZE(asStr)) ? asStr[level] : "SD_LOG_LEVEL_NONE";
}

typedef enum _SdDroneType
{
	SD_DRONE_TYPE_INVALID,
	SD_DRONE_TYPE_HELLI,
	SD_DRONE_TYPE_AIRPLANE,
	SD_DRONE_TYPE_QUADROTOR,
	SD_DRONE_TYPE_HEXROTOR,
	SD_DRONE_TYPE_OCTOROTOR
} SdDroneType;

typedef struct _SdImuDeviceConfig
{
	inline _SdImuDeviceConfig() {
		SamplingRate=Scale=MaxReading=NumBiasSamples=Watermark=0;
	}
	std::string DeviceName;

	/*
	 * Sampling rate - how many times per seconds data is read from the sensor
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

	/*
	 * Used to remap coordinates of the read samples; needed due to different
	 * IMU sensors may having unaligned coordinate systems.
	 */
	Matrix3d CoordinateMap;
} SdImuDeviceConfig;

typedef struct _SdServoConfig
{
	inline _SdServoConfig() { Rate=BitCount=ChannelMask=0; }
	std::string DeviceName;
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
	_SdQuadRotorConfig() {
		Motor[0]=Motor[1]=Motor[2]=Motor[3]=-1;
		Kp = Ki = Kd = 0;
		ImuAngleAxisZ = -1;
	}
	int Motor[4];
	int ImuAngleAxisZ;  // IMU axes may not be aligned with the quadrotor axes
	double Kp;
	double Ki;
	double Kd;
} SdQuadRotorConfig;

typedef struct _SdDroneConfig
{
	inline _SdDroneConfig() {
		ServerPort=SD_DEFAULT_TCP_PORT;
		LogRotationMatrix=LogLevel=LogRate=0;
	}
	SdImuDeviceConfig Gyro;
	SdImuDeviceConfig Accel;
	SdImuDeviceConfig Mag;
	SdServoConfig Servo;
	SdQuadRotorConfig Quad;
	std::string ServerAddress;
	int ServerPort;
	int LogRotationMatrix;
	int LogLevel;
	std::string LogFileName;

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
#define SD_PLUGIN_IMU_KALMAN_FILTER "sd.imukalmanfilter"
#define SD_PLUGIN_QUADPILOT  "sd.quadpilot"
#define SD_PLUGIN_PIDPILOT  "sd.pidpilot"
#define SD_PLUGIN_NAVIGATOR "sd.navigator"
#define SD_PLUGIN_SERVO_PCA9685 "sd.servo_pca9685"
#define SD_PLUGIN_TRACELOG "sd.tracelog"
#define SD_PLUGIN_THRUST_CORRECT "sd.thrustcorrect"

#define SD_PLUGIN_VERSION "SigmaDronePlugin"


/*
 * SigmaDrone specific error codes
 */
#define SD_ESUCCESS 	0
#define SD_EFIRST 		20000
#define SD_ESTOP_DISPATCH  	SD_EFIRST+0
#define SD_EINVALID_DEPENDENCY SD_EFIRST+1
#define SD_INVALID_ATTRIBUTE SD_EFIRST+2


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

typedef struct _SdImuData {
	Vector3f gyro3d;
	Vector3f acc3d;
	Vector3f mag3d;
	bool gyro3d_upd;
	bool acc3d_upd;
	bool mag3d_upd;
} SdImuData;

typedef struct _SdThrustValues {
	static constexpr double MIN = 0.0;
	static constexpr double MAX = 1.0;
	_SdThrustValues(double thr=MIN, double minThr=MIN, double maxThr=MIN) {
		minThrust = fmin(fmax(MIN, minThr), maxThr);
		maxThrust = fmax(fmin(MAX, maxThr), minThrust);
		thrust = fmin(fmax(minThrust,thr),maxThrust);
	}
	double Thrust() const { return thrust; }
	double MinThrust() const { return minThrust; }
	double MaxThrust() const { return maxThrust; }
private:
	double thrust;
	double minThrust;
	double maxThrust;
} SdThrustValues;

#include "boost/variant.hpp"
struct SdIoData
{
	static const uint32_t TYPE_INVALID 		= 0;
	static const uint32_t TYPE_INT32 		= 1;
	static const uint32_t TYPE_DOUBLE 		= 2;
	static const uint32_t TYPE_STRING 		= 3;
	static const uint32_t TYPE_VECTOR3D 	= 4;
	static const uint32_t TYPE_VECTOR4D 	= 5;
	static const uint32_t TYPE_QUATERNION 	= 6;
	static const uint32_t TYPE_IMU 			= 7;
	static const uint32_t TYPE_DRONE_CONFIG = 8;
	static const uint32_t TYPE_SERVO 		= 9;
	static const uint32_t TYPE_THRUST 		= 10;

	typedef boost::variant<int32_t,double,Vector3f,Vector4d,QuaternionF,SdDroneConfig,
			SdServoIoData,SdThrustValues,SdImuData,std::string> Data;

	SdIoData() { dataType_ = TYPE_INVALID; data_ = 0; }
	SdIoData(int32_t i) { dataType_ = TYPE_INT32; data_ = asInt32(); }
	SdIoData(double d) { dataType_ = TYPE_DOUBLE; data_ = d; }
	SdIoData(const Vector3f& v3d) { dataType_ = TYPE_VECTOR3D; data_ = v3d; }
	SdIoData(const Vector4d& v4d) { dataType_ = TYPE_VECTOR4D; data_ = v4d; }
	SdIoData(const QuaternionF& qt) { dataType_ = TYPE_QUATERNION; data_ = qt; }
	SdIoData(const SdServoIoData& siod) { dataType_ = TYPE_SERVO; data_ = siod; }
	SdIoData(const SdDroneConfig& config) {
		dataType_ = TYPE_DRONE_CONFIG; data_ = config;
	}
	SdIoData(const SdThrustValues& thr) { dataType_ = TYPE_THRUST; data_ = thr; }
	SdIoData(const SdImuData& imuData) { dataType_ = TYPE_IMU; data_ = imuData; }

	uint32_t dataType() const { return dataType_; }
	int32_t asInt32() const { assert(dataType_ == TYPE_INT32); return boost::get<int32_t>(data_); }
	double asDouble() const { assert(dataType_ == TYPE_DOUBLE);
		return boost::get<double>(data_);
	}
	const QuaternionF& asQuaternion()  const {
		assert(dataType_ == TYPE_QUATERNION); return boost::get<QuaternionF>(data_);
	}
	const Vector4d& asVector4d()  const {
		assert(dataType_ == TYPE_VECTOR4D); return boost::get<Vector4d>(data_);
	}
	const Vector3f& asVector3f()  const {
		assert(dataType_ == TYPE_VECTOR3D); return boost::get<Vector3f>(data_);
	}
	const SdDroneConfig& asDroneConfig()  const {
		assert(dataType_ == TYPE_DRONE_CONFIG); return boost::get<SdDroneConfig>(data_);
	}
	const SdServoIoData& asServoData() const {
		assert(dataType_ == TYPE_SERVO); return boost::get<SdServoIoData>(data_);
	}
	const SdThrustValues& asThrust() const {
		assert(dataType_ == TYPE_THRUST); return boost::get<SdThrustValues>(data_);
	}
	const SdImuData& asImuData() const {
		assert(dataType_ == TYPE_IMU); return boost::get<SdImuData>(data_);
	}

private:
	uint32_t dataType_; /*one of the above types*/
	Data data_;

};



static const uint32_t SDIO_ATTR_ATTITUDE_Q 	= 0;
static const uint32_t SDIO_ATTR_TARGET_Q 	= 1;
static const uint32_t SDIO_ATTR_GYRO 		= 2;
static const uint32_t SDIO_ATTR_ACCEL 		= 3;
static const uint32_t SDIO_ATTR_MAG 		= 4;
static const uint32_t SDIO_ATTR_EARTH_G 	= 5;
static const uint32_t SDIO_ATTR_MOTORS 		= 6;
static const uint32_t SDIO_ATTR_ERR_PID 	= 7;
static const uint32_t SDIO_ATTR_ERR_P 		= 8;
static const uint32_t SDIO_ATTR_ERR_I 		= 9;
static const uint32_t SDIO_ATTR_ERR_D 		= 10;
static const uint32_t SDIO_ATTR_ERR_ANGLE	= 11;
static const uint32_t SDIO_ATTR_ALTITUDE	= 12;
static const uint32_t SDIO_ATTR_DELTA_TIME	= 13;
static const uint32_t SDIO_ATTR_CORR_VELOCITY = 14;
static const uint32_t SDIO_ATTR_TIME_TO_READ_SENSORS = 15;
static const uint32_t SDIO_ATTR_TEMPERATURE = 16;
static const uint32_t SDIO_ATTR_COORDINATES = 17;
static const uint32_t SDIO_ATTR_ERR_OMEGA 	= 18;

/*
 * SdIoPacket describes the IO structure that travels up and down the plugin
 * chain. SdIoPacket has the following main categories of fields:
 * - fields that do not change for the life of the object - returned from
 * IoCode(), PluginName(), DeviceId(), StartingAltitude()
 * - input/output data - the equivalent of input/output parameters in a function
 * call. Input data is populated by the issuer and output data is set by the
 * target plugin, used to return information to the caller/issuer of the SdIoPacket
 * - drone state attributes - filled-in by different plugins as the packet moves
 * thru the chain. Attributes are not guaranteed to be always present
 */
struct SdIoPacket
{
	virtual uint32_t IoCode() = 0;
	virtual uint32_t Flags() = 0;

	/*
	 * Returns the target plugin name, may be NULL for IOs with
	 * SD_IOCODE_RECEIVE
	 */
	virtual const char* PluginName() = 0;

	/*
	 * Returns the target device ID, this will be the issuer's ID
	 * for SD_IOCODE_RECEIVE and the target ID for the rest of the
	 * SD_IOCODE_XXX.
	 */
	virtual SdDeviceId DeviceId() = 0;

	/*
	 * Returns the altitude at which the IO was inserted in the chain;
	 * for the most part it will equal the altitude of the plugin that
	 * issued the IO
	 */
	virtual SdPluginAltitude StartingAltitude() = 0;

	/*
	 * Set/GetIoData. Input data conveys data across the chain, think of
	 * it as an input parameter of a function call. Output data is used
	 * only to return data back to the issuer of the IO operation; think
	 * of it as an output parameter of the function call
	 */
	virtual const SdIoData& GetIoData(
			bool input
			) = 0;
	virtual void SetIoData(
			IN const SdIoData&,
			IN bool input
			) = 0;
	/*
	 * Returns the specified attribute type. If the attribute type has not been
	 * set prior to the call, then the returned SdIoData will carry data type
	 * SdIoData::TYPE_INVALID
	 */
	virtual SdIoData GetAttribute(
			IN uint32_t attributeType
			) = 0;

	/*
	 * Sets IO attribute.
	 */
	virtual int SetAttribute(
			IN uint32_t attributeType,
			IN const SdIoData& attribute
			) = 0;

	/*
	 * Shortcut getters. These APIs achieve the same functionality
	 * as GetAttribute/SetAttribute and are provided for easy access to
	 * the most commonly accessed fields
	 */
	virtual const QuaternionF& Attitude() = 0;
	virtual const Vector3f& GyroData() = 0;
	virtual const Vector3f& AccelData() = 0;
	virtual const Vector3f& MagData() = 0;
	virtual const QuaternionF& TargetAttitude() = 0;
	virtual double DeltaTime() = 0;


protected:
	virtual ~SdIoPacket() {}
};

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

	virtual SdIoPacket* AllocIoPacket(
			uint32_t ioCode,
			SdDeviceId deviceType,
			const char* pluginName
			) = 0;

	virtual void FreeIoPacket(
			SdIoPacket*
			) = 0;

protected:
	virtual ~IPluginRuntime() {}
};


struct IPlugin
{
	virtual int AddRef() = 0;
	virtual int Release() = 0;

	/*
	 * Invoked to tell the plugin to carry out the specified command. The plugin must
	 * process at least the following:
	 * - SD_COMMAND_RUN - perform any post init steps and update its configuration
	 *   and subscribe for IOs; when this command completes the plugin must be ready
	 *   to process IOs
	 * - SD_COMMAND_RESET - IO processing has stopped on the chain and the drone
	 *   hardware must be reset, most importantly the plugin controlling the servo PWM,
	 *   must shutdown the motors
	 * - SD_COMMAND_EXIT - plugin must perform SD_COMMAND_RESET, followed by detaching
	 *   itself from the chain
	 * Plugins may choose to ignore the rest of the commands, but they can't opt out
	 * to receive them.
	 */
	virtual int ExecuteCommand(
			struct SdCommandParams* commandArgs
			) = 0;

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
	virtual int IoDispatchThread() { return SD_ESUCCESS; }

protected:
	virtual ~IPlugin() {}
};

struct SdCommandParams
{
	virtual SdCommandCode CommandCode() = 0;
	virtual const SdIoData& Params() = 0;
	virtual const IJsonValue* RpcParams() = 0;
	virtual void SetOutParams(const SdIoData&) = 0;
	virtual const SdIoData& OutParams() = 0;

protected:
	virtual ~SdCommandParams() {}
};

#endif // SDRONE_API_H_
