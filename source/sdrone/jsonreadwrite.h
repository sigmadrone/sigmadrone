/*
 * jsonreadwrite.h
 *
 *  Created on: Jan 27, 2014
 *      Author: svassilev
 */

#ifndef JSONREADWRITE_H_
#define JSONREADWRITE_H_

#include "commoninc.h"
#include <vector>
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

class JsonObjectReader: public IJsonObjectReader {
public:
	JsonObjectReader(
			JsonObject* obj);
	void AddRef();
	void Release();
	size_t GetMemberCount();
	const char* GetMemberName(size_t index);
	SdJsonValueType GetMemberType(const char* name);
	IJsonValueReader* RefMember(const char* name);
	IJsonObjectReader* RefMemberAsObject(const char* name) {
		return IJsonValueReader::RefAsObjectSafe(RefMember(name));
	}
	IJsonArrayReader* RefMemberAsArray(
			IN const char* name) {
		return IJsonValueReader::RefAsArraySafe(RefMember(name));
	}
	bool GetMemberAsDoubleValue(
			IN const char* name,
			OUT double* value) {
		return IJsonValueReader::AsDoubleSafe(RefMember(name),value);
	}
	bool GetMemberAsIntValue(
			IN const char* name,
			OUT int64_t* value) {
		return IJsonValueReader::AsIntSafe(RefMember(name),value);
	}
	bool GetMemberAsIntValue(
			IN const char* name,
			OUT int32_t* value) {
		return IJsonValueReader::AsIntSafe(RefMember(name),value);
	}
#if 0
	bool GetMemberAsIntValue(
			IN const char* name,
			OUT int16_t* value) {
		return IJsonValueReader::AsIntSafe(RefMember(name),value);
	}
#endif
	bool GetMemberAsBoolValue(
			IN const char* name,
			OUT bool* value) {
		return IJsonValueReader::AsBoolSafe(RefMember(name),value);
	}
	bool GetMemberAsStringValue(
			IN const char* name,
			OUT std::string* value) {
		*value = IJsonValueReader::AsStringSafe(RefMember(name));
		return !!value->length();
	}

	JsonObject* RefGlibJsonObject();

protected:
	~JsonObjectReader();
private:
	JsonObject* m_jobj;
	std::vector<char*> m_members;
	int m_refCnt;
};

class JsonValueReader: public IJsonValueReader
{
public:
	JsonValueReader(
			JsonNode* jnode);
	void AddRef();
	void Release();
	SdJsonValueType GetType();
	const char* GetTypeAsString();
	double AsDouble();
	int64_t AsInt();
	int32_t AsInt32() { return (int32_t)AsInt(); }
	int16_t AsInt16() { return (int16_t)AsInt(); }
	bool AsBool();
	const char* AsString();
	IJsonArrayReader* RefAsArray();
	IJsonObjectReader* RefAsObject();
	JsonNode* DupGlibNode();
protected:
	~JsonValueReader();
private:
	JsonNode* m_jnode;
	SdJsonValueType m_type;
	const char* m_typeAsStr;
	int m_refCnt;
};

class JsonArrayReader: public IJsonArrayReader
{
public:
	JsonArrayReader(
			JsonArray* jarr);
	void AddRef();
	void Release();
	uint32_t ElementCount();
	IJsonValueReader* RefElement(
			uint32_t index);
	IJsonObjectReader* RefElementAsObject(
			IN uint32_t index) {
		return IJsonValueReader::RefAsObjectSafe(RefElement(index));
	}
	IJsonArrayReader* RefElementAsArray(
			IN uint32_t index) {
		return IJsonValueReader::RefAsArraySafe(RefElement(index));
	}
	bool GetElementAsDoubleValue(
			IN uint32_t index,
			OUT double* value) {
		return IJsonValueReader::AsDoubleSafe(RefElement(index),value);
	}

	bool GetElementAsIntValue(
			IN uint32_t index,
			OUT int64_t* value) {
		return IJsonValueReader::AsIntSafe(RefElement(index),value);
	}
	bool GetElementAsIntValue(
			IN uint32_t index,
			OUT int32_t* value) {
		return IJsonValueReader::AsIntSafe(RefElement(index),value);
	}
#if 0
	bool GetElementAsIntValue(
			IN uint32_t index,
			OUT int16_t* value) {
		return IJsonValueReader::AsIntSafe(RefElement(index),value);
	}
#endif
	virtual bool GetElementAsBoolValue(
			IN uint32_t index,
			OUT bool* value) {
		return IJsonValueReader::AsBoolSafe(RefElement(index),value);
	}
	virtual bool GetElementAsStringValue(
			IN uint32_t index,
			OUT std::string* value) {
		*value = IJsonValueReader::AsStringSafe(RefElement(index));
		return !!value->length();
	}
	JsonArray* RefGlibJsonArray();
protected:
	~JsonArrayReader();
private:
	JsonArray* m_jarr;
	int m_refCnt;
};

class JsonArrayWriter;
class JsonObjectWriter;

class JsonValueWriter: public IJsonValueWriter
{
public:
	JsonValueWriter();
	void AddRef();
	void Release();
	void Reset();
	bool SetValueAsObject(
			IN IJsonObjectReader*);
	bool SetValueAsArray(
			IN IJsonArrayReader*);
	bool SetValueAsDouble(
			IN double value);
	bool SetValueAsInt(
			IN int64_t value);
	bool SetValueAsBool(
			IN bool value);
	bool SetValueAsString(
			IN const char* value
			);
	IJsonValueReader* RefReader();

	bool SetValueAsObject(JsonObjectWriter* obj);
	bool SetValueAsArray(JsonArrayWriter* arr);

	JsonNode* GetJNode() { return m_jnode; }

protected:
	~JsonValueWriter();

	bool AllocGlibNode(JsonNodeType nodeType);

private:
	JsonNode* m_jnode;
	int m_refCnt;
};


class JsonArrayWriter: public IJsonArrayWriter
{
public:
	JsonArrayWriter();
	void AddRef();
	void Release();
	void Reset();
	bool AddElement(
			IN IJsonValueReader*
			);
	IJsonArrayReader* RefArray();
	JsonArray* PeekGlibArray() { return m_jarr; }
protected:
	JsonArray* AllocGlibArray();
	~JsonArrayWriter();
private:
	JsonArray* m_jarr;
	int m_refCnt;
};

class JsonObjectWriter: public IJsonObjectWriter
{
public:
	JsonObjectWriter();
	void AddRef();
	void Release();
	void Reset();
	bool AddMember(
			IN const char* name,
			IN IJsonValueReader* member);
	bool AddObjectMember(
			IN const char* name,
			IN IJsonObjectReader*);
	bool AddArrayMember(
			IN const char* name,
			IN IJsonArrayReader*);
	bool AddDoubleMember(
			IN const char* name,
			IN double value);
	bool AddIntMember(
			IN const char* name,
			IN int64_t value);
	bool AddBoolMember(
			IN const char* name,
			IN bool value);
	bool AddStringMember(
			IN const char* name,
			IN const char* value);
	IJsonObjectReader* RefObject();
	bool AddMember(
			IN const char* name,
			IN JsonValueWriter* member);
	bool AddObjectMember(
			IN const char* name,
			IN JsonObjectWriter*);
	bool AddArrayMember(
			IN const char* name,
			IN JsonArrayWriter*);
	JsonObject* PeekGlibJobj() { return m_jobj; }
protected:
	JsonObject* AllocGlibObject();
	~JsonObjectWriter();
	bool AddMemberAndDeref(
		IN const char* name,
		IN IJsonValueReader* member);
private:
	JsonObject* m_jobj;
	int m_refCnt;
};


#endif /* JSONREADWRITE_H_ */
