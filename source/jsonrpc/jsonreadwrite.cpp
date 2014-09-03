/*
 * jsonreadwrite.cpp
 *
 *  Created on: Jan 27, 2014
 *      Author: svassilev
 */

#include "jsonreadwrite.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

struct GTypeInit
{
	GTypeInit() { g_type_init(); }
} gTypeInitOnly;

static const SdJsonValue s_nullJsonValue;
static const SdJsonObject s_nullJsonObject;
static const SdJsonArray s_nullJsonArray;

SdJsonValue::SdJsonValue() :
		m_jnode(0),
		m_type(SD_JSONVALUE_NULL),
		m_typeAsStr("SD_JSONVALUE_NONE"),
		m_asObj(0)
{
}

SdJsonValue::SdJsonValue(JsonNode* jnode) :
		m_jnode(0),m_type(SD_JSONVALUE_NULL),
		m_typeAsStr("SD_JSONVALUE_NONE"),m_asObj(0)
{
	assert(jnode);
	m_jnode = json_node_copy(jnode);
	Init();
}

SdJsonValue::SdJsonValue(const SdJsonValue& val) :
	m_jnode(0), m_type(SD_JSONVALUE_NULL),
	m_typeAsStr("SD_JSONVALUE_NONE"), m_asObj(0)
{
	*this = val;
}

SdJsonValue::SdJsonValue(const IJsonValue* val) :
	m_jnode(0), m_type(SD_JSONVALUE_NULL),
	m_typeAsStr("SD_JSONVALUE_NONE"), m_asObj(0)
{
	assert(val);
	*this = *(SdJsonValue*)val;
}

SdJsonValue::SdJsonValue(const SdJsonArray& arr) :
	m_jnode(0), m_type(SD_JSONVALUE_NULL),
	m_typeAsStr("SD_JSONVALUE_NONE"), m_asObj(0)
{
	SetValueAsArray(&arr);
}

SdJsonValue::SdJsonValue(const SdJsonObject& obj) :
	m_jnode(0), m_type(SD_JSONVALUE_NULL),
	m_typeAsStr("SD_JSONVALUE_NONE"), m_asObj(0)
{
	SetValueAsObject(&obj);
}

SdJsonValue::SdJsonValue(int64_t i) :
		m_jnode(0), m_type(SD_JSONVALUE_NULL),
		m_typeAsStr("SD_JSONVALUE_NONE"), m_asObj(0)
{
	SetValueAsInt(i);
}
SdJsonValue::SdJsonValue(uint64_t i) :
		m_jnode(0), m_type(SD_JSONVALUE_NULL),
		m_typeAsStr("SD_JSONVALUE_NONE"), m_asObj(0)
{
	SetValueAsInt(i);
}
SdJsonValue::SdJsonValue(int32_t i) :
		m_jnode(0), m_type(SD_JSONVALUE_NULL),
		m_typeAsStr("SD_JSONVALUE_NONE"), m_asObj(0)
{
	SetValueAsInt(i);
}
SdJsonValue::SdJsonValue(uint32_t i) :
		m_jnode(0), m_type(SD_JSONVALUE_NULL),
		m_typeAsStr("SD_JSONVALUE_NONE"), m_asObj(0)
{
	SetValueAsInt(i);
}
SdJsonValue::SdJsonValue(int16_t i) :
		m_jnode(0), m_type(SD_JSONVALUE_NULL),
		m_typeAsStr("SD_JSONVALUE_NONE"), m_asObj(0)
{
	SetValueAsInt(i);
}
SdJsonValue::SdJsonValue(uint16_t i) :
		m_jnode(0), m_type(SD_JSONVALUE_NULL),
		m_typeAsStr("SD_JSONVALUE_NONE"), m_asObj(0)
{
	SetValueAsInt(i);
}

SdJsonValue::SdJsonValue(bool b) :
		m_jnode(0), m_type(SD_JSONVALUE_NULL),
		m_typeAsStr("SD_JSONVALUE_NONE"), m_asObj(0)
{
	SetValueAsBool(b);
}

SdJsonValue::SdJsonValue(const std::string& str) :
		m_jnode(0), m_type(SD_JSONVALUE_NULL),
		m_typeAsStr("SD_JSONVALUE_NONE"), m_asObj(0)
{
	SetValueAsString(str.c_str());
}
SdJsonValue::SdJsonValue(const char* str) :
		m_jnode(0), m_type(SD_JSONVALUE_NULL),
		m_typeAsStr("SD_JSONVALUE_NONE"), m_asObj(0)
{
	SetValueAsString(str);
}

SdJsonValue::SdJsonValue(double d) :
		m_jnode(0), m_type(SD_JSONVALUE_NULL),
		m_typeAsStr("SD_JSONVALUE_NONE"), m_asObj(0)
{
	SetValueAsDouble(d);
}

SdJsonValue::~SdJsonValue() {
	Reset();
}

void SdJsonValue::Init()
{
	if (0 != m_jnode) {
		JsonNodeType nodeType = json_node_get_node_type(m_jnode);
		if (JSON_NODE_OBJECT == nodeType) {
			m_type = SD_JSONVALUE_OBJECT;
			m_typeAsStr = "SD_JSONVALUE_OBJECT";
		} else if (JSON_NODE_VALUE == nodeType) {
			GType valType = json_node_get_value_type(m_jnode);
			switch (valType) {
			case G_TYPE_BOOLEAN:
				m_type = SD_JSONVALUE_BOOL;
				m_typeAsStr = "SD_JSONVALUE_BOOL";
				break;
			case G_TYPE_CHAR:
			case G_TYPE_UCHAR:
			case G_TYPE_INT:
			case G_TYPE_UINT:
			case G_TYPE_INT64:
			case G_TYPE_UINT64:
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
				printf("SdJsonValue: unrecognized value type: %ld\n",
						valType);
			}
		} else if (JSON_NODE_ARRAY == nodeType) {
			m_type = SD_JSONVALUE_ARRAY;
			m_typeAsStr = "SD_JSONVALUE_ARRAY";
		}
	} else {
		Reset();
	}
}

void SdJsonValue::Reset()
{
	if (SD_JSONVALUE_ARRAY == GetType()) {
		if (m_asArr) {
			delete m_asArr;
		}
	} else if (SD_JSONVALUE_OBJECT == GetType()) {
		if (m_asObj) {
			delete m_asObj;
		}
	}
	m_asObj = 0;
	m_asStr.clear();
	if (!!m_jnode) {
		json_node_free(m_jnode);
		m_jnode = 0;
	}
	m_type = SD_JSONVALUE_NULL;
	m_typeAsStr = "SD_JSONVALUE_NULL";
}

SdJsonValueType SdJsonValue::GetType() const {
	return m_type;
}

const char* SdJsonValue::GetTypeAsString() const {
	return m_typeAsStr;
}

double SdJsonValue::AsDouble() const {
	if (GetType() == SD_JSONVALUE_DOUBLE) {
		return json_node_get_double(const_cast<JsonNode*>(m_jnode));
	}
	return AsInt();
}

int64_t SdJsonValue::AsInt() const {
	if (GetType() == SD_JSONVALUE_INT) {
		return json_node_get_int(const_cast<JsonNode*>(m_jnode));
	}
	return 0;
}

bool SdJsonValue::AsBool() const {
	if (GetType() == SD_JSONVALUE_BOOL) {
		return json_node_get_boolean(const_cast<JsonNode*>(m_jnode));
	}
	return 0;
}

bool SdJsonValue::AsDoubleSafe(double* val) const
{
	if (GetType() == SD_JSONVALUE_DOUBLE ||
		GetType() == SD_JSONVALUE_INT) {
		*val = AsDouble();
		return true;
	}
	return false;
}

bool SdJsonValue::AsIntSafe(int64_t* val) const
{
	if (GetType() == SD_JSONVALUE_INT) {
		*val = AsInt();
		return true;
	}
	return false;
}

bool SdJsonValue::AsIntSafe(int32_t* val) const
{
	if (GetType() == SD_JSONVALUE_INT) {
		*val = (int32_t)AsInt();
		return true;
	}
	return false;
}

bool SdJsonValue::AsUintSafe(uint64_t* val) const
{
	return AsIntSafe((int64_t*)val);
}

bool SdJsonValue::AsUintSafe(uint32_t* val) const
{
	return AsIntSafe((int32_t*)val);
}

bool SdJsonValue::AsBoolSafe(bool* val) const
{
	if (GetType() == SD_JSONVALUE_BOOL) {
		*val = AsBool();
		return true;
	}
	return false;
}

bool SdJsonValue::AsStringSafe(std::string* val) const
{
	if (GetType() == SD_JSONVALUE_STRING) {
		*val = AsString();
		return true;
	}
	return false;
}

std::string SdJsonValue::AsString() const {
	assert(GetType() == SD_JSONVALUE_STRING);
	if (!m_asStr.size()) {
		*const_cast<std::string*>(&m_asStr) =
				json_node_get_string(const_cast<JsonNode*>(m_jnode));
	}
	return m_asStr;
}

const IJsonArray* SdJsonValue::AsArray() const
{
	if (GetType()==SD_JSONVALUE_ARRAY) {
		if (0 == m_asArr) {
			SdJsonValue* _this = const_cast<SdJsonValue*>(this);
			_this->m_asArr = new SdJsonArray(json_node_get_array(_this->m_jnode));
		}
		return m_asArr;
	}
	return 0;
}

const IJsonObject* SdJsonValue::AsObject() const
{
	if (GetType()==SD_JSONVALUE_OBJECT) {
		if (0 == m_asObj) {
			SdJsonValue* _this = const_cast<SdJsonValue*>(this);
			_this->m_asObj = new SdJsonObject(json_node_get_object(
					_this->m_jnode));
		}
		return m_asObj;
	}
	return 0;
}

IJsonValue* SdJsonValue::Clone() const
{
	return new SdJsonValue(*this);
}

JsonNode* SdJsonValue::DupGlibNode() const
{
	JsonNode* thisNode = const_cast<JsonNode*>(m_jnode);
	if (GetType() == SD_JSONVALUE_NULL) {
		return NULL;
	}
	if (GetType() != SD_JSONVALUE_ARRAY && GetType() != SD_JSONVALUE_OBJECT) {
		return json_node_copy(thisNode);
	}
	JsonNode* jnode = json_node_new(json_node_get_node_type(thisNode));
	if (jnode) {
		JsonArray* jarr = 0;
		JsonObject* jobj = 0;
		if (GetType() == SD_JSONVALUE_ARRAY) {
			if (0 != (jarr = json_node_dup_array(thisNode))) {
				json_node_set_array(jnode, jarr);
			}
		} else {
			assert(GetType() == SD_JSONVALUE_OBJECT);
			if (0 != (jobj = json_node_dup_object(thisNode))) {
				json_node_set_object(jnode, jobj);
			}
		}
		if (!jobj && !jarr) {
			json_node_free(jnode);
			jnode = 0;
		}
	}
	return jnode;
}

bool SdJsonValue::SetValueAsObject(const IJsonObject* _obj)
{
	bool retVal = false;
	SdJsonObject* obj = dynamic_cast<SdJsonObject*>(_obj->Clone());
	Reset();
	if (0 != obj) {
		if (0 != (m_jnode = json_node_new(JSON_NODE_OBJECT))) {
			json_node_set_object(m_jnode,obj->PeekGlibObj());
			Init();
			retVal = true;
		}
		delete obj;
	}
	return retVal;
}

bool SdJsonValue::SetValueAsArray(const IJsonArray* _arr)
{
	bool retVal = false;
	SdJsonArray* arr = dynamic_cast<SdJsonArray*>(_arr->Clone());
	Reset();
	if (0 != arr) {
		if (0 != (m_jnode = json_node_new(JSON_NODE_ARRAY))) {
			json_node_set_array(m_jnode,arr->PeekGlibArr());
			Init();
			retVal = true;
		}
		delete arr;
	}
	return retVal;
}

bool SdJsonValue::SetValueAsDouble(double value)
{
	Reset();
	if (0 != (m_jnode = json_node_new(JSON_NODE_VALUE))) {
		json_node_set_double(m_jnode,value);
		Init();
	}
	return !!m_jnode;
}

bool SdJsonValue::SetValueAsInt(int64_t value)
{
	Reset();
	if (0 != (m_jnode = json_node_new(JSON_NODE_VALUE))) {
		json_node_set_int(m_jnode,value);
		Init();
	}
	return !!m_jnode;

}

bool SdJsonValue::SetValueAsBool(bool value)
{
	Reset();
	if (0 != (m_jnode = json_node_new(JSON_NODE_VALUE))) {
		json_node_set_boolean(m_jnode,value);
		Init();
	}
	return !!m_jnode;
}

bool SdJsonValue::SetValueAsString(const char* value)
{
	Reset();
	if (0 != (m_jnode = json_node_new(JSON_NODE_VALUE))) {
		json_node_set_string(m_jnode,value);
		Init();
	}
	return !!m_jnode;
}

const SdJsonValue& SdJsonValue::operator=(const SdJsonValue& rhs)
{
	Reset();
	m_jnode = const_cast<SdJsonValue*>(&rhs)->DupGlibNode();
	Init();
	return *this;
}

SdJsonObject::SdJsonObject() : m_jobj(0) {
}

SdJsonObject::SdJsonObject(JsonObject* obj) : m_jobj(obj) {
	assert(m_jobj);
	json_object_ref(m_jobj);
}

SdJsonObject::SdJsonObject(const SdJsonObject& obj) : m_jobj(0) {
	*this = obj;
}

SdJsonObject::~SdJsonObject() {
	Reset();
}

void SdJsonObject::Reset()
{
	if (m_jobj) {
		json_object_unref(m_jobj);
		m_jobj = 0;
	}
	MemberMapIt it;
	while ((it = m_members.begin()) != m_members.end()) {
		m_members.erase(it);
		delete it->second;
	}
	m_names.clear();
}

size_t SdJsonObject::GetMemberCount() const
{
	const_cast<SdJsonObject*>(this)->InitMemberNames();
	return m_names.size();
}

void SdJsonObject::InitMemberNames()
{
	if (0 != m_jobj && 0 == m_names.size()) {
		GList* memberNames = json_object_get_members(m_jobj);
		if (0 != memberNames) {
			m_names.reserve(8);
			GList* it = g_list_first(memberNames);
			while (0 != it) {
				m_names.push_back((char*)it->data);
				it = g_list_next(it);
			}
			g_list_free(memberNames);
		}
	}
}

const char* SdJsonObject::GetMemberName(size_t index) const
{
	return (index < GetMemberCount()) ? m_names[index].c_str() : 0;
}

const IJsonValue* SdJsonObject::GetMember(
	const char* memberName) const
{
	ConstMemberMapIt it = m_members.find(memberName);
	if (it == m_members.end() && m_jobj) {
		JsonNode* jnode = json_object_get_member(m_jobj, memberName);
		if (jnode) {
			SdJsonValue* sdNode = new SdJsonValue(jnode);
			if (sdNode) {
				const_cast<SdJsonObject*>(this)->m_members[memberName] = sdNode;
			}
			it = m_members.find(memberName);
		}
	}
	return it != m_members.end() ? it->second : &s_nullJsonValue;
}

bool SdJsonObject::IsMemberPresent(
	IN const char* memberName) const
{
	return GetMember(memberName)->GetType() != SD_JSONVALUE_NULL;
}

bool SdJsonObject::AddMember(
		const char* name,
		const IJsonValue* _member)
{
	const SdJsonValue* member = dynamic_cast<const SdJsonValue*>(_member);
	RemoveMember(name);
	if (!m_jobj) {
		m_jobj = json_object_new();
	}
	if (member && m_jobj) {
		JsonNode* dup = member->DupGlibNode();
		if (dup) {
			InitMemberNames();
			json_object_set_member(m_jobj,name,dup);
			m_names.push_back(name);
			return true;
		}
	}
	return false;
}

void SdJsonObject::RemoveMember(const char* name)
{
	if (m_jobj && IsMemberPresent(name)) {
		MemberMapIt it = m_members.find(name);
		assert(it != m_members.end());
		m_members.erase(it);
		delete it->second;

		std::string strName(name);
		for (size_t i = 0; i < m_names.size(); i++) {
			if (m_names[i] == strName) {
				m_names.erase(m_names.begin() + i);
				break;
			}
		}
		json_object_remove_member(m_jobj,name);
	}
}

IJsonObject* SdJsonObject::Clone() const
{
	return new SdJsonObject(*this);
}

const SdJsonObject& SdJsonObject::operator=(const SdJsonObject& rhs)
{
	Reset();
	for (size_t i = 0; i < rhs.GetMemberCount(); i++) {
		const char* memberName = rhs.GetMemberName(i);
		assert(memberName);
		AddMember(memberName,rhs.GetMember(memberName));
	}
	return *this;
}

SdJsonArray::SdJsonArray() : m_jarr(0) {}

SdJsonArray::SdJsonArray(JsonArray* jarr) : m_jarr(jarr) {
	assert(m_jarr);
	json_array_ref(m_jarr);
}

SdJsonArray::SdJsonArray(const SdJsonArray& arr) : m_jarr(0) {
	*this = arr;
}

SdJsonArray::~SdJsonArray() {
	Reset();
}

uint32_t SdJsonArray::ElementCount() const {
	return !!m_jarr ? json_array_get_length(m_jarr) : 0;
}

const IJsonValue* SdJsonArray::GetElement(uint32_t index) const
{
	SdJsonValue* ret = 0;
	if (index < ElementCount()) {
		const_cast<SdJsonArray*>(this)->m_elements.resize(ElementCount(),0);
		if (m_elements.size() == ElementCount()) {
			if (0 == (ret = m_elements[index])) {
				ret = new SdJsonValue(json_array_get_element(m_jarr,index));
				const_cast<SdJsonArray*>(this)->m_elements[index] = ret;
			}
		}
	}
	return ret;
}

bool SdJsonArray::AddElement(const IJsonValue* _val)
{
	const SdJsonValue* val = dynamic_cast<const SdJsonValue*>(_val);
	JsonNode* dup = 0;
	if (!m_jarr) {
		m_jarr = json_array_new();
	}
	if (val && m_jarr) {
		if (0 != (dup = val->DupGlibNode())) {
			json_array_add_element(m_jarr,dup);
		}
	}
	return !!dup;
}

IJsonArray* SdJsonArray::Clone() const {
	return new SdJsonArray(*this);
}

void SdJsonArray::Reset()
{
	for (size_t i = 0; i < m_elements.size(); i++) {
		if (m_elements[i]) {
			delete m_elements[i];
		}
	}
	m_elements.clear();
	if (m_jarr) {
		json_array_unref(m_jarr);
		m_jarr = 0;
	}
}

const SdJsonArray& SdJsonArray::operator=(const SdJsonArray& rhs)
{
	Reset();
	for (size_t i = 0; i < rhs.ElementCount(); i++) {
		AddElement(rhs.GetElement(i));
	}
	return *this;
}
