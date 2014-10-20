/*
 * jsonrpc.h
 *
 *  Created on: April4, 2014
 *      Author: svassilev
 *
 *  JSON RPC definitions
 */

#ifndef SDJSONRPC_H_
#define SDJSONRPC_H_

#include <string>

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef ARRAYSIZE
#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))
#endif

static const uint32_t SD_JSONRPC_INVALID_ID = 0;

static const int SD_JSONRPC_ERROR_SUCCESS 	= 0;
static const int SD_JSONRPC_ERROR_FIRST = -32700;
static const int SD_JSONRPC_ERROR_PARSE = SD_JSONRPC_ERROR_FIRST;
static const int SD_JSONRPC_ERROR_INVALID_XMLRPC = -32600;
static const int SD_JSONRPC_ERROR_UNSUPPORTED_METHOD = -32601;
static const int SD_JSONRPC_ERROR_INVALID_METHOD_PARAMS = -32602;
static const int SD_JSONRPC_ERROR_APP 		= -32500;
static const int SD_JSONRPC_ERROR_SYSTEM 	= -32400;
static const int SD_JSONRPC_ERROR_TRANSPORT = -32300;
static const int SD_JSONRPC_ERROR_LAST 		= SD_JSONRPC_ERROR_TRANSPORT;

__inline bool SdIsJsonRpcError(int err) {
	return err >= SD_JSONRPC_ERROR_FIRST && err <= SD_JSONRPC_ERROR_LAST;
}

__inline const char* SdJsonRpcErrorToString(int err) {
	switch (err) {
	case SD_JSONRPC_ERROR_PARSE: return "SD_JSONRPC_ERROR_PARSE";
	case SD_JSONRPC_ERROR_INVALID_XMLRPC: return "SD_JSONRPC_ERROR_INVALID_XMLRPC";
	case SD_JSONRPC_ERROR_UNSUPPORTED_METHOD: return "SD_JSONRPC_ERROR_UNSUPPORTED_METHOD";
	case SD_JSONRPC_ERROR_INVALID_METHOD_PARAMS: return "SD_JSONRPC_ERROR_INVALID_METHOD_PARAMS";
	case SD_JSONRPC_ERROR_APP: return "SD_JSONRPC_ERROR_APP";
	case SD_JSONRPC_ERROR_SYSTEM: return "SD_JSONRPC_ERROR_SYSTEM";
	case SD_JSONRPC_ERROR_TRANSPORT: return "SD_JSONRPC_ERROR_TRANSPORT";
	default: return "SD_JSONRPC_ERROR_UNKNOWN";
	}
}

enum SdJsonValueType
{
	SD_JSONVALUE_NULL,
	SD_JSONVALUE_STRING,
	SD_JSONVALUE_INT,
	SD_JSONVALUE_DOUBLE,
	SD_JSONVALUE_BOOL,
	SD_JSONVALUE_OBJECT,
	SD_JSONVALUE_ARRAY
};

__inline const char* SdJsonValueTypeAsString(SdJsonValueType t) {
	static const char* asStr[] = {"null","string","int","double","bool","object","array"};
	return t < ARRAYSIZE(asStr) ? asStr[t] : "null";
}

__inline SdJsonValueType SdJsonValueTypeFromString(std::string val) {
	static const char* asStr[] = {"null","string","int","double","bool","object","array"};
	for (size_t i = 0; i < ARRAYSIZE(asStr); ++i) {
		if (val == std::string(asStr[i])) {
			return static_cast<SdJsonValueType>(i);
		}
	}
	return SD_JSONVALUE_NULL;
}

typedef enum _SdCommandCode {
	SD_COMMAND_NONE = 0,
	SD_COMMAND_RUN,
	SD_COMMAND_RESET,
	SD_COMMAND_EXIT,
	SD_COMMAND_PING,
	SD_COMMAND_SET_CONFIG,
	SD_COMMAND_GET_CONFIG,
	SD_COMMAND_SET_THRUST,
	SD_COMMAND_GET_THRUST,
	SD_COMMAND_SET_ATTITUDE,
	SD_COMMAND_GET_ATTITUDE,
	SD_COMMAND_SET_TARGET_ALTITUDE,
	SD_COMMAND_GET_ALTITUDE,
	SD_COMMAND_GET_RPC_SPEC,
	SD_COMMAND_GET_RPC_LIST,
	SD_COMMAND_LOAD_PLUGIN,
	SD_COMMAND_UNLOAD_PLUGIN,
	SD_COMMAND_GET_PLUGINS,
	SD_COMMAND_SET_EARTH_G_VECTOR,
	SD_COMMAND_GET_EARTH_G_VECTOR,
	SD_COMMAND_IS_RUNNING,
	SD_COMMAND_GET_MOTORS,
	SD_COMMAND_SET_MOTORS,
	SD_COMMAND_SET_SAFE_THRUST
} SdCommandCode;

static const char* SdCommandCodeAsStr[] = {
		"sd_none","sd_run","sd_reset","sd_exit","sd_ping",
		"sd_set_config","sd_get_config",
		"sd_set_thrust", "sd_get_thrust", "sd_set_attitude",
		"sd_get_attitude","sd_set_altitude", "sd_get_altitude",
		"sd_get_rpc_spec", "sd_get_rpc_list", "sd_load_plugin",
		"sd_unload_plugin",	"sd_get_plugins", "sd_set_earth_g_vector",
		"sd_get_earth_g_vector", "sd_get_running", "sd_get_motors",
		"sd_set_motors", "sd_set_safe_thrust"
};

__inline SdCommandCode SdStringToCommandCode(const char* _str) {
	std::string str(_str);
	for (size_t i = 0; i < ARRAYSIZE(SdCommandCodeAsStr); i++) {
		if (std::string(SdCommandCodeAsStr[i]) == str) {
			return static_cast<SdCommandCode>(i);
		}
	}
	return SD_COMMAND_NONE;
}

__inline const char* SdCommandCodeToString(SdCommandCode cmd) {
	return (cmd < ARRAYSIZE(SdCommandCodeAsStr)) ?
			SdCommandCodeAsStr[cmd] : SdCommandCodeAsStr[0];
}


struct IJsonArray;
struct IJsonObject;

struct IJsonValue
{
	virtual SdJsonValueType GetType() const = 0;
	virtual const char* GetTypeAsString() const = 0;

	/*
	 * The AsDouble/Int/Bool virtual members below assume that the caller is
	 * asking for the correct value type; if the value is of different type
	 * then 0 is returned. AsString/Array/Object will return
	 * NULL if the value is of different type than the asked type.
	 */
	virtual double AsDouble() const = 0;
	virtual int64_t AsInt() const = 0;
	virtual bool AsBool() const = 0;
	virtual std::string AsString() const = 0;
	virtual const IJsonArray* AsArray() const = 0;
	virtual const IJsonObject* AsObject() const = 0;

	/*
	 * Safe routines for extracting ordinal types. The output parameter will
	 * not be set if, the value contains different value
	 */
	virtual bool AsDoubleSafe(OUT double*) const = 0;
	virtual bool AsIntSafe(OUT int64_t*) const = 0;
	virtual bool AsIntSafe(OUT int32_t*) const = 0;
	virtual bool AsUintSafe(OUT uint64_t*) const = 0;
	virtual bool AsUintSafe(OUT uint32_t*) const = 0;
	virtual bool AsBoolSafe(OUT bool*) const = 0;
	virtual bool AsStringSafe(OUT std::string*) const = 0;

	virtual bool SetValueAsObject(
			IN const IJsonObject*) = 0;
	virtual bool SetValueAsArray(
			IN const IJsonArray*) = 0;
	virtual bool SetValueAsDouble(
			IN double value) = 0;
	virtual bool SetValueAsInt(
			IN int64_t value) = 0;
	virtual bool SetValueAsBool(
			IN bool value) = 0;
	virtual bool SetValueAsString(
			IN const char*) = 0;

	virtual IJsonValue* Clone() const = 0;
protected:
	virtual ~IJsonValue() {}
};

struct IJsonArray
{
	virtual uint32_t ElementCount() const = 0;
	virtual const IJsonValue* GetElement(
			uint32_t index
			) const = 0;
	virtual bool AddElement(
			const IJsonValue*
			) = 0;
	virtual void Reset() = 0;
	virtual IJsonArray* Clone() const = 0;
protected:
	virtual ~IJsonArray() {}
};

struct IJsonObject
{
	virtual size_t GetMemberCount() const = 0;
	virtual const char* GetMemberName(
				IN size_t index
				) const = 0;
	/*
	 * If member is not present will return IJsonValue of type SD_JSONVALUE_NULL
	 */
	virtual const IJsonValue* GetMember(
			IN const char* memberName
			) const = 0;
	virtual bool IsMemberPresent(
			IN const char* memberName
			) const = 0;
	virtual bool AddMember(
			IN const char* name,
			IN const IJsonValue* member) = 0;

	virtual void Reset() = 0;
	virtual IJsonObject* Clone() const = 0;

protected:
	virtual ~IJsonObject() {}
};

#endif // SDJSONRPC_H_
