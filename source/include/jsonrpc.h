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
static const int SD_JSONRPC_ERROR_PARSE = -32700;
static const int SD_JSONRPC_ERROR_INVALID_XMLRPC = -32600;
static const int SD_JSONRPC_ERROR_UNSUPPORTED_METHOD = -32601;
static const int SD_JSONRPC_ERROR_INVALID_METHOD_PARAMS = -32602;
static const int SD_JSONRPC_ERROR_APP 		= -32500;
static const int SD_JSONRPC_ERROR_SYSTEM 	= -32400;
static const int SD_JSONRPC_ERROR_TRANSPORT = -32300;

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
	const char* asStr[] = {"null","string","int","double","bool","object","array"};
	return t < ARRAYSIZE(asStr) ? asStr[t] : "null";
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
