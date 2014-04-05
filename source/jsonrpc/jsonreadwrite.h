/*
 * jsonreadwrite.h
 *
 *  Created on: Jan 27, 2014
 *      Author: svassilev
 */

#ifndef JSONREADWRITE_H_
#define JSONREADWRITE_H_

#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <assert.h>
#include <errno.h>
#include <vector>
#include <map>
#include "jsonrpc.h"

struct _JsonNode;
struct _JsonArray;
struct _JsonObject;

class SdJsonArray;
class SdJsonObject;

class SdJsonValue: public IJsonValue {
public:
	SdJsonValue();
	SdJsonValue(_JsonNode* jnode);
	SdJsonValue(const SdJsonValue&);
	SdJsonValue(const SdJsonArray& arr);
	SdJsonValue(const SdJsonObject& obj);
	SdJsonValue(int64_t i);
	SdJsonValue(int32_t i);
	SdJsonValue(int16_t i);
	SdJsonValue(uint64_t i);
	SdJsonValue(uint32_t i);
	SdJsonValue(uint16_t i);
	SdJsonValue(char i);
	SdJsonValue(bool b);
	SdJsonValue(const std::string&);
	SdJsonValue(const char*);
	SdJsonValue(double d);
	~SdJsonValue();
	SdJsonValueType GetType() const;
	const char* GetTypeAsString() const;
	double AsDouble() const;
	int64_t AsInt() const;
	bool AsBool() const;
	std::string AsString() const;
	const IJsonArray* AsArray() const;
	const IJsonObject* AsObject() const;
	bool AsDoubleSafe(double*) const;
	bool AsIntSafe(int64_t*) const;
	bool AsBoolSafe(bool*) const;

	IJsonValue* Clone() const;

	bool SetValueAsObject( IN const IJsonObject*);
	bool SetValueAsArray( IN const IJsonArray*);
	bool SetValueAsDouble( IN double value);
	bool SetValueAsInt( IN int64_t value);
	bool SetValueAsBool( IN bool value);
	bool SetValueAsString( IN const char* value);

	const SdJsonValue& operator=(const SdJsonValue&);
	_JsonNode* DupGlibNode() const;
	_JsonNode* PeekGlibNode() { return m_jnode; }

protected:
	void Init();
	void Reset();

protected:
	_JsonNode* m_jnode;
	SdJsonValueType m_type;
	const char* m_typeAsStr;
	std::string m_asStr;
	union {
		SdJsonArray* m_asArr;
		SdJsonObject* m_asObj;
	};
};

class SdJsonObject: public IJsonObject
{
public:
	SdJsonObject();
	SdJsonObject(_JsonObject*);
	SdJsonObject(const SdJsonObject&);
	~SdJsonObject();
	void Reset();
	size_t GetMemberCount() const;
	const char* GetMemberName(
			IN size_t index
			) const;
	const IJsonValue* GetMember(
			IN const char* memberName
			) const;
	bool IsMemberPreset(
			IN const char* memberName
			) const;
	bool AddMember(
			IN const char* name,
			IN const IJsonValue* member);
	bool AddMember(
			IN const char* name,
			IN const SdJsonValue& member) {
		return AddMember(name,&member);
	}

	IJsonObject* Clone() const;
	const SdJsonObject& operator=(const SdJsonObject& rhs);
	_JsonObject* PeekGlibObj() { return m_jobj; }

private:

	void InitMemberNames();

	typedef std::map<std::string,SdJsonValue*> MemberMap;
	typedef MemberMap::iterator MemberMapIt;
	typedef MemberMap::const_iterator ConstMemberMapIt;

	MemberMap m_members;
	std::vector<std::string> m_names;
	_JsonObject* m_jobj;
};

class SdJsonArray: public IJsonArray
{
public:
	SdJsonArray();
	SdJsonArray(_JsonArray*);
	SdJsonArray(const SdJsonArray&);
	~SdJsonArray();

	uint32_t ElementCount() const;
	const IJsonValue* GetElement(
			uint32_t index
			) const;
	bool AddElement(
			const IJsonValue*
			);
	bool AddElement(const SdJsonValue& val) { return AddElement(&val); }
	IJsonArray* Clone() const;
	void Reset();
	const SdJsonArray& operator=(const SdJsonArray&);

	_JsonArray* PeekGlibArr() { return m_jarr; }

private:
	_JsonArray* m_jarr;
	std::vector<SdJsonValue*> m_elements;
};

#endif /* JSONREADWRITE_H_ */

