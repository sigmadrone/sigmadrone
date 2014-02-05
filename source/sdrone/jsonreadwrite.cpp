/*
 * jsonreadwrite.cpp
 *
 *  Created on: Jan 27, 2014
 *      Author: svassilev
 */

#include "jsonreadwrite.h"

/*
 *  Readers implementation
 */

JsonObjectReader::JsonObjectReader(
		JsonObject* obj) :
		m_jobj(obj),m_refCnt(1)
{
	assert(m_jobj);
	json_object_ref(m_jobj);
}

JsonObject* JsonObjectReader::RefGlibJsonObject() {
	json_object_ref(m_jobj);
	return m_jobj;
}

JsonObjectReader::~JsonObjectReader() {
	json_object_unref(m_jobj);
}

void JsonObjectReader::AddRef() {
	__sync_fetch_and_add(&m_refCnt,1);
}

void JsonObjectReader::Release() {
	if (0 == __sync_sub_and_fetch(&m_refCnt,1)) {
		delete this;
	}
}

size_t JsonObjectReader::GetMemberCount()
{
	if (0 == m_members.size()) {
		GList* memberNames = json_object_get_members(m_jobj);
		if (0 != memberNames) {
			m_members.reserve(8);
			GList* it = g_list_first(memberNames);
			while (0 != it) {
				m_members.push_back((char*)it->data);
				it = g_list_next(it);
			}
			g_list_free(memberNames);
		}
	}
	return m_members.size();
}

const char* JsonObjectReader::GetMemberName(size_t index)
{
	return GetMemberCount() > index ? m_members.at(index) : 0;
}

IJsonValueReader* JsonObjectReader::RefMember(const char* name)
{
	assert(name);
	IJsonValueReader* parsedValue = 0;
	JsonNode* jnode = json_object_get_member(m_jobj,name);
	if (0 != jnode) {
		parsedValue = new JsonValueReader(jnode);
	}
	return parsedValue;
}

SdJsonValueType JsonObjectReader::GetMemberType(
		const char* name)
{
	SdJsonValueType type = SD_JSONVALUE_NULL;
	IJsonValueReader* val = RefMember(name);
	if (val) {
		type = val->GetType();
		val->Release();
	}
	return type;
}


/*
 * JsonArrayReader implementation
 */
JsonArrayReader::JsonArrayReader(
		JsonArray* jarr) :
		m_jarr(jarr), m_refCnt(1)
{
	assert(m_jarr);
	json_array_ref(m_jarr);
}

JsonArray* JsonArrayReader::RefGlibJsonArray() {
	json_array_ref(m_jarr);
	return m_jarr;
}

JsonArrayReader::~JsonArrayReader() {
	json_array_unref(m_jarr);
}

void JsonArrayReader::AddRef() {
	__sync_fetch_and_add(&m_refCnt,1);
}

void JsonArrayReader::Release() {
	if (0 == __sync_sub_and_fetch(&m_refCnt,1)) {
		delete this;
	}
}

uint32_t JsonArrayReader::ElementCount() {
	return json_array_get_length(m_jarr);
}

IJsonValueReader* JsonArrayReader::RefElement(uint32_t index) {
	IJsonValueReader* parsedJsonVal = 0;
	if (index < ElementCount()) {
		JsonNode* jnode = json_array_get_element(m_jarr,index);
		if (0 != jnode) {
			parsedJsonVal = new JsonValueReader(jnode);
		}
	}
	return parsedJsonVal;
}

/*
 * JsonValueReader implementation
 */
JsonValueReader::JsonValueReader(JsonNode* jnode) :
		m_jnode(0),
		m_type(SD_JSONVALUE_NULL),
		m_typeAsStr("SD_JSONVALUE_NULL"),
		m_refCnt(1)
{
	assert(jnode);
	m_jnode = json_node_copy(jnode);
	if (!!m_jnode) {
		JsonNodeType nodeType = json_node_get_node_type(jnode);
		if (JSON_NODE_OBJECT == nodeType) {
			m_type = SD_JSONVALUE_OBJECT;
			m_typeAsStr = "SD_JSONVALUE_OBJECT";
		} else if (JSON_NODE_VALUE == nodeType) {
			GType valType = json_node_get_value_type(jnode);
			switch(valType) {
			case G_TYPE_BOOLEAN:
				m_type = SD_JSONVALUE_BOOL;
				m_typeAsStr = "SD_JSONVALUE_BOOL";
				break;
			case G_TYPE_CHAR:case G_TYPE_UCHAR:case G_TYPE_INT:
			case G_TYPE_UINT:case G_TYPE_INT64:case G_TYPE_UINT64:
				m_type = SD_JSONVALUE_INT;
				m_typeAsStr = "SD_JSONVALUE_INT";
				break;
			case G_TYPE_DOUBLE:
				m_type = SD_JSONVALUE_DOUBLE;
				m_typeAsStr = "SD_JSONVALUE_DOUBLE";
				break;
			case 64:
				m_type = SD_JSONVALUE_STRING;
				m_typeAsStr = "SD_JSONVALUE_STRING";
				break;
			default:
				printf("JsonValueReader: unrecognized value type: %ld\n", valType);
			}
		} else if (JSON_NODE_ARRAY == nodeType) {
			m_type = SD_JSONVALUE_ARRAY;
			m_typeAsStr = "SD_JSONVALUE_ARRAY";
		}
	}
}

JsonValueReader::~JsonValueReader() {
	if (!!m_jnode) {
		json_node_free(m_jnode);
	}
}

void JsonValueReader::AddRef() {
	__sync_fetch_and_add(&m_refCnt,1);
}

void JsonValueReader::Release() {
	if (0 == __sync_sub_and_fetch(&m_refCnt,1)) {
		delete this;
	}
}

SdJsonValueType JsonValueReader::GetType() {
	return m_type;
}

const char* JsonValueReader::GetTypeAsString() {
	return m_typeAsStr;
}

double JsonValueReader::AsDouble() {
	assert(GetType() == SD_JSONVALUE_DOUBLE);
	return json_node_get_double(m_jnode);
}

int64_t JsonValueReader::AsInt() {
	assert(GetType() == SD_JSONVALUE_INT);
	return json_node_get_int(m_jnode);
}

bool JsonValueReader::AsBool() {
	assert(GetType() == SD_JSONVALUE_BOOL);
	return json_node_get_boolean(m_jnode);
}

const char* JsonValueReader::AsString() {
	assert(GetType() == SD_JSONVALUE_STRING);
	return json_node_get_string(m_jnode);
}

IJsonArrayReader* JsonValueReader::RefAsArray() {
	assert(GetType() == SD_JSONVALUE_ARRAY);
	IJsonArrayReader* parsedJar = 0;
	JsonArray* jar = json_node_get_array(m_jnode);
	if (0 != jar) {
		parsedJar = new JsonArrayReader(jar);
	}
	return parsedJar;
}

IJsonObjectReader* JsonValueReader::RefAsObject() {
	assert(GetType() == SD_JSONVALUE_OBJECT);
	IJsonObjectReader* parsedJobj = 0;
	JsonObject* jobj = json_node_get_object(m_jnode);
	if (0 != jobj) {
		parsedJobj = new JsonObjectReader(jobj);
	}
	return parsedJobj;
}

JsonNode* JsonValueReader::DupGlibNode()
{
	return json_node_copy(m_jnode);
}

/*
 * Writers implementation
 */

JsonValueWriter::JsonValueWriter() : m_jnode(0), m_refCnt(1)
{
}

JsonValueWriter::~JsonValueWriter()
{
	if (!!m_jnode) {
		json_node_free(m_jnode);
	}
}

void JsonValueWriter::AddRef() {
	__sync_fetch_and_add(&m_refCnt,1);
}

void JsonValueWriter::Release() {
	if (0 == __sync_sub_and_fetch(&m_refCnt,1)) {
		delete this;
	}
}

void JsonValueWriter::Reset() {
}

bool JsonValueWriter::AllocGlibNode(JsonNodeType nodeType) {
	if (!!m_jnode) {
		json_node_free(m_jnode);
	}
	return !!(m_jnode = json_node_new(nodeType));
}

bool JsonValueWriter::SetValueAsObject(IJsonObjectReader* _reader) {
	if (_reader && AllocGlibNode(JSON_NODE_OBJECT)) {
		JsonObjectReader* reader = dynamic_cast<JsonObjectReader*>(_reader);
		if (reader) {
			JsonObject* glibObj = reader->RefGlibJsonObject();
			if (glibObj) {
				json_node_set_object(m_jnode, glibObj);
				json_object_unref(glibObj);
			}
		} //else oops
	}
	return !!m_jnode;
}

bool JsonValueWriter::SetValueAsArray(IJsonArrayReader* _reader) {
	if (_reader && AllocGlibNode(JSON_NODE_ARRAY)) {
		JsonArrayReader* reader = dynamic_cast<JsonArrayReader*>(_reader);
		if (reader) {
			JsonArray* glibArr = reader->RefGlibJsonArray();
			if (glibArr) {
				json_node_set_array(m_jnode, glibArr);
				json_array_unref(glibArr);
			}
		}
	}
	return !!m_jnode;
}

bool JsonValueWriter::SetValueAsDouble(double value) {
	if (AllocGlibNode(JSON_NODE_VALUE)) {
		json_node_set_double(m_jnode,value);
	}
	return !!m_jnode;
}

bool JsonValueWriter::SetValueAsInt(int32_t value) {
	return SetValueAsInt64(value);
}

bool JsonValueWriter::SetValueAsInt64(int64_t value) {
	if (AllocGlibNode(JSON_NODE_VALUE)) {
		json_node_set_int(m_jnode,value);
	}
	return !!m_jnode;
}

bool JsonValueWriter::SetValueAsBool(bool value) {
	if (AllocGlibNode(JSON_NODE_VALUE)) {
		json_node_set_boolean(m_jnode,value);
	}
	return !!m_jnode;
}

bool JsonValueWriter::SetValueAsString(const char* value) {
	if (AllocGlibNode(JSON_NODE_VALUE)) {
		json_node_set_string(m_jnode,value);
	}
	return !!m_jnode;
}

IJsonValueReader* JsonValueWriter::RefValue() {
	return new JsonValueReader(m_jnode);
}

JsonArrayWriter::JsonArrayWriter() : m_jarr(0), m_refCnt(1) {
}

void JsonArrayWriter::AddRef() {
	__sync_fetch_and_add(&m_refCnt,1);
}

void JsonArrayWriter::Release() {
	if (0 == __sync_sub_and_fetch(&m_refCnt,1)) {
		delete this;
	}
}

void JsonArrayWriter::Reset() {
	if (!!m_jarr) {
		json_array_unref(m_jarr);
		m_jarr=0;
	}
}

bool JsonArrayWriter::AddElement(IJsonValueReader* _jvalReader) {
	bool ret = false;
	JsonValueReader* jvalReader = dynamic_cast<JsonValueReader*>(_jvalReader);
	if (AllocGlibArray() && !!jvalReader) {
		JsonNode* glibJnode = jvalReader->DupGlibNode();
		if (glibJnode) {
			json_array_add_element(m_jarr,glibJnode);
			ret = true;
		}
	}
	return ret;
}

IJsonArrayReader* JsonArrayWriter::RefArray() {
	return new JsonArrayReader(m_jarr);
}

JsonArrayWriter::~JsonArrayWriter() {
	Reset();
}

JsonArray* JsonArrayWriter::AllocGlibArray()
{
	if (0 == m_jarr) {
		m_jarr = json_array_new();
	}
	return m_jarr;
}

JsonObjectWriter::JsonObjectWriter() : m_jobj(0), m_refCnt(1)
{
}

void JsonObjectWriter::AddRef() {
	__sync_fetch_and_add(&m_refCnt,1);
}

void JsonObjectWriter::Release() {
	if (0 == __sync_sub_and_fetch(&m_refCnt,1)) {
		delete this;
	}
}

void JsonObjectWriter::Reset() {
	if (m_jobj) {
		json_object_unref(m_jobj);
		m_jobj = 0;
	}
}

bool JsonObjectWriter::AddMember(
		const char* memberName,
		IJsonValueReader* member)
{
	bool ret = false;
	JsonValueReader* jvalReader = dynamic_cast<JsonValueReader*>(member);
	if (AllocGlibObject() && !!jvalReader) {
		JsonNode* glibJnode = jvalReader->DupGlibNode();
		if (glibJnode) {
			json_object_set_member(m_jobj,memberName,glibJnode);
			ret = true;
		}
	}
	return ret;
}

IJsonObjectReader* JsonObjectWriter::RefObject() {
	return new JsonObjectReader(m_jobj);
}

JsonObjectWriter::~JsonObjectWriter()
{
	Reset();
}

JsonObject* JsonObjectWriter::AllocGlibObject()
{
	if (!m_jobj) {
		m_jobj = json_object_new();
	}
	return m_jobj;
}
