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
	SdJsonValue(const IJsonValue*);
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
	const SdJsonArray& Array() const;
	const SdJsonObject& Object() const;
	bool AsDoubleSafe(double*) const;
	bool AsIntSafe(int64_t*) const;
	bool AsIntSafe(int32_t*) const;
	bool AsUintSafe(uint64_t*) const;
	bool AsUintSafe(uint32_t*) const;
	bool AsStringSafe(std::string*) const;
	bool AsBoolSafe(bool*) const;

	IJsonValue* Clone() const;

	bool SetValueAsObject( IN const IJsonObject*);
	bool SetValueAsArray( IN const IJsonArray*);
	bool SetValueAsDouble( IN double value);
	bool SetValueAsInt( IN int64_t value);
	bool SetValueAsBool( IN bool value);
	bool SetValueAsString( IN const char* value);

	const SdJsonValue& operator=(const SdJsonValue&);
	bool operator==(const SdJsonValue&) const;
	bool operator!=(const SdJsonValue& rhs) const { return !operator==(rhs); }
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
	bool IsMemberPresent(
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
	void RemoveMember(const char* name);
	IJsonObject* Clone() const;
	const SdJsonObject& operator=(const SdJsonObject& rhs);
	bool operator==(const SdJsonObject& rhs) const;
	bool operator!=(const SdJsonObject& rhs) const { return !operator==(rhs); }
	_JsonObject* PeekGlibObj() { return m_jobj; }
	const SdJsonValue& Member(const char* name) const {
		return *static_cast<const SdJsonValue*>(GetMember(name));
	}
	const SdJsonValue& operator[](const char* name) const {
		return Member(name);
	}
	SdJsonValue& operator[](const char* name) {
		if (!IsMemberPresent(name)) {
			AddMember(name,SdJsonValue(true));
		}
		return *const_cast<SdJsonValue*>(&(Member(name)));
	}

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
	SdJsonArray(const SdJsonValue&);
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
	bool operator==(const SdJsonArray& rhs) const;
	bool operator!=(const SdJsonArray& rhs) const { return !operator==(rhs); }

	_JsonArray* PeekGlibArr() { return m_jarr; }

	const SdJsonValue& Element(size_t index) const {
		return *static_cast<const SdJsonValue*>(GetElement(index));
	}
	const SdJsonValue& operator[](size_t i)const {
		return Element(i);
	}

private:
	_JsonArray* m_jarr;
	std::vector<SdJsonValue*> m_elements;
};

static const SdJsonValue s_nullJsonValue;
static const SdJsonObject s_nullJsonObject;
static const SdJsonArray s_nullJsonArray;

class SdJsonValueSpec
{
public:
	SdJsonValueSpec(const SdJsonValue&);
	~SdJsonValueSpec() {}
	const SdJsonValue& Get();
private:
	void OnJsonValue(const SdJsonValue& val, SdJsonValue* spec);
	const SdJsonValue& m_value;
	SdJsonValue m_spec;
};

class SdJsonValueFromSpec
{
public:
	SdJsonValueFromSpec(
			const SdJsonValue& valueSpec,
			const std::vector<std::string>& data);
	const SdJsonValue& Get();
private:
	void OnJsonValueSpec(const SdJsonValue& spec, SdJsonValue* value);
private:
	const SdJsonValue& m_spec;
	const std::vector<std::string>& m_data;
	size_t m_dataIndex;
	SdJsonValue m_value;
};


std::string SdJsonValueToText(const SdJsonValue& val);

#endif /* JSONREADWRITE_H_ */

