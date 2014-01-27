/*
 * jsonrpcparser.cpp
 *
 *  Created on: Dec 15, 2013
 *      Author: svassilev
 */

#include "jsonrpcparser.h"
#include <vector>

struct GLibInit {
	GLibInit() { g_type_init(); }
} glibInit;

class ParsedJsonObject: public IJsonObjectReader {
public:
	ParsedJsonObject(
			JsonObject* obj);
	void AddRef();
	void Release();
	size_t GetMemberCount();
	const char* GetMemberName(size_t index);
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
	bool GetMemberAsIntValue(
			IN const char* name,
			OUT int16_t* value) {
		return IJsonValueReader::AsIntSafe(RefMember(name),value);
	}
	bool GetMemberAsBoolValue(
			IN const char* name,
			OUT bool* value) {
		return IJsonValueReader::AsBoolSafe(RefMember(name),value);
	}
	bool GetMemberAsStringValue(
			IN const char* name,
			OUT const char** value) {
		*value = IJsonValueReader::AsStringSafe(RefMember(name));
		return !!(*value);
	}

protected:
	~ParsedJsonObject();
private:
	JsonObject* m_jobj;
	std::vector<char*> m_members;
	int m_refCnt;
};

class ParsedJsonValue: public IJsonValueReader
{
public:
	ParsedJsonValue(
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
protected:
	~ParsedJsonValue();
private:
	JsonNode* m_jnode;
	SdJsonValueType m_type;
	const char* m_typeAsStr;
	int m_refCnt;
};

class ParsedJsonArray: public IJsonArrayReader
{
public:
	ParsedJsonArray(
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
			OUT int32_t* value) {
		return IJsonValueReader::AsIntSafe(RefElement(index),value);
	}
	bool GetElementAsIntValue(
			IN uint32_t index,
			OUT int64_t* value) {
		return IJsonValueReader::AsIntSafe(RefElement(index),value);
	}
	bool GetElementAsIntValue(
			IN uint32_t index,
			OUT int16_t* value) {
		return IJsonValueReader::AsIntSafe(RefElement(index),value);
	}
	virtual bool GetElementAsBoolValue(
			IN uint32_t index,
			OUT bool* value) {
		return IJsonValueReader::AsBoolSafe(RefElement(index),value);
	}
	virtual bool GetElementAsStringValue(
			IN uint32_t index,
			OUT const char** value) {
		*value = IJsonValueReader::AsStringSafe(RefElement(index));
		return !!(*value);
	}
protected:
	~ParsedJsonArray();
private:
	JsonArray* m_jarr;
	int m_refCnt;
};

ParsedJsonObject::ParsedJsonObject(
		JsonObject* obj) :
		m_jobj(obj),m_refCnt(1)
{
	assert(m_jobj);
	json_object_ref(m_jobj);
}

ParsedJsonObject::~ParsedJsonObject()
{
	json_object_unref(m_jobj);
}

void ParsedJsonObject::AddRef() {
	__sync_fetch_and_add(&m_refCnt,1);
}

void ParsedJsonObject::Release() {
	if (0 == __sync_sub_and_fetch(&m_refCnt,1)) {
		delete this;
	}
}

size_t ParsedJsonObject::GetMemberCount()
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

const char* ParsedJsonObject::GetMemberName(size_t index)
{
	return GetMemberCount() > index ? m_members.at(index) : 0;
}

IJsonValueReader* ParsedJsonObject::RefMember(const char* name)
{
	assert(name);
	IJsonValueReader* parsedValue = 0;
	JsonNode* jnode = json_object_get_member(m_jobj,name);
	if (0 != jnode) {
		parsedValue = new ParsedJsonValue(jnode);
	}
	return parsedValue;
}


//
// ParsedJsonArray implementation
//
ParsedJsonArray::ParsedJsonArray(
		JsonArray* jarr) :
		m_jarr(jarr), m_refCnt(1)
{
	assert(m_jarr);
	json_array_ref(m_jarr);
}

ParsedJsonArray::~ParsedJsonArray() {
	json_array_unref(m_jarr);
}

void ParsedJsonArray::AddRef() {
	__sync_fetch_and_add(&m_refCnt,1);
}

void ParsedJsonArray::Release() {
	if (0 == __sync_sub_and_fetch(&m_refCnt,1)) {
		delete this;
	}
}

uint32_t ParsedJsonArray::ElementCount() {
	return json_array_get_length(m_jarr);
}

IJsonValueReader* ParsedJsonArray::RefElement(uint32_t index) {
	IJsonValueReader* parsedJsonVal = 0;
	if (index < ElementCount()) {
		JsonNode* jnode = json_array_get_element(m_jarr,index);
		if (0 != jnode) {
			parsedJsonVal = new ParsedJsonValue(jnode);
		}
	}
	return parsedJsonVal;
}

//
// ParsedJsonValue implementation
//
ParsedJsonValue::ParsedJsonValue(JsonNode* jnode) :
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
				printf("ParsedJsonValue: unrecognized value type: %ld\n", valType);
			}
		} else if (JSON_NODE_ARRAY == nodeType) {
			m_type = SD_JSONVALUE_ARRAY;
			m_typeAsStr = "SD_JSONVALUE_ARRAY";
		}
	}
}

ParsedJsonValue::~ParsedJsonValue() {
	if (!!m_jnode) {
		json_node_free(m_jnode);
	}
}

void ParsedJsonValue::AddRef() {
	__sync_fetch_and_add(&m_refCnt,1);
}

void ParsedJsonValue::Release() {
	if (0 == __sync_sub_and_fetch(&m_refCnt,1)) {
		delete this;
	}
}

SdJsonValueType ParsedJsonValue::GetType() {
	return m_type;
}

const char* ParsedJsonValue::GetTypeAsString() {
	return m_typeAsStr;
}

double ParsedJsonValue::AsDouble() {
	assert(GetType() == SD_JSONVALUE_DOUBLE);
	return json_node_get_double(m_jnode);
}

int64_t ParsedJsonValue::AsInt() {
	assert(GetType() == SD_JSONVALUE_INT);
	return json_node_get_int(m_jnode);
}

bool ParsedJsonValue::AsBool() {
	assert(GetType() == SD_JSONVALUE_BOOL);
	return json_node_get_boolean(m_jnode);
}

const char* ParsedJsonValue::AsString() {
	assert(GetType() == SD_JSONVALUE_STRING);
	return json_node_get_string(m_jnode);
}

IJsonArrayReader* ParsedJsonValue::RefAsArray() {
	assert(GetType() == SD_JSONVALUE_ARRAY);
	IJsonArrayReader* parsedJar = 0;
	JsonArray* jar = json_node_get_array(m_jnode);
	if (0 != jar) {
		parsedJar = new ParsedJsonArray(jar);
	}
	return parsedJar;
}

IJsonObjectReader* ParsedJsonValue::RefAsObject() {
	assert(GetType() == SD_JSONVALUE_OBJECT);
	IJsonObjectReader* parsedJobj = 0;
	JsonObject* jobj = json_node_get_object(m_jnode);
	if (0 != jobj) {
		parsedJobj = new ParsedJsonObject(jobj);
	}
	return parsedJobj;
}


JsonRpcParser::JsonRpcParser() {
	m_jsonParser = 0;
	m_rootObj = 0;
}

JsonRpcParser::~JsonRpcParser() {
	Reset();
}

void JsonRpcParser::Reset()
{
	if (m_jsonParser) {
		g_object_unref(m_jsonParser);
		m_jsonParser = 0;
	}
	if (m_rootObj) {
		m_rootObj->Release();
		m_rootObj = 0;
	}

}

bool JsonRpcParser::ParseFile(
		const char* fileName)
{
	GError* gerr=0;
	JsonNode* rootNode = 0;
	IJsonValueReader* jvalue = 0;

	Reset();

	if (!(m_jsonParser = json_parser_new())) {
		printf("JsonRpcParser error: json_parser_new failed!\n");
		goto __return;
	}

	if (!json_parser_load_from_file(m_jsonParser,fileName,&gerr)) {
		printf("JsonRpcParser error: %s\n", gerr->message);
		return false;
	}

	if (!(rootNode = json_parser_get_root(m_jsonParser))) {
		goto __return;
	}

	if (0 == (jvalue = new ParsedJsonValue(rootNode))) {
		goto __return;
	}

	if (jvalue->GetType() != SD_JSONVALUE_OBJECT) {
		printf("JsonRpcParser error: unexpected root type %s\n",
				jvalue->GetTypeAsString());
		goto __return;
	}

	if (0 == (m_rootObj = jvalue->RefAsObject())) {
		printf("JsonRpcParser error: failed to ref root object!\n");
	}

	__return:
	if (jvalue) {
		jvalue->Release();
	}

	return !!m_rootObj;
}

bool JsonRpcParser::ParseBuffer(
		const char* buffer,
		uint32_t len,
		uint32_t* usedLen)
{
	GError* gerr=0;
	IJsonValueReader* jvalue = 0;

	if (usedLen) {
		*usedLen = 0;
	}

	Reset();

	if (!(m_jsonParser = json_parser_new())) {
		goto __return;
	}

	if (!json_parser_load_from_data(m_jsonParser, buffer, len, &gerr)) {
		printf("JsonRpcParser error: %s\n", gerr->message);
		goto __return;
	}

	if (!json_parser_get_root(m_jsonParser)) {
		goto __return;
	}

	if (0 == (jvalue = new ParsedJsonValue(
			json_parser_get_root(m_jsonParser)))) {
		goto __return;
	}

	if (jvalue->GetType() != SD_JSONVALUE_OBJECT) {
		printf("JsonRpcParser error: unexpected root type %s\n",
				jvalue->GetTypeAsString());
		goto __return;
	}

	if (0 == (m_rootObj = jvalue->RefAsObject())) {
		printf("JsonRpcParser error: failed to ref root object!\n");
		goto __return;
	}

	if (usedLen) {
		/*
		 * Could not find a way to figure out how much of the buffer
		 * json_parser_load_from_data has used, so will try to figure it out
		 * by iterating over the '{' and '}'
		 */
		const char* cursor = buffer;
		int sum = 1;
		bool inStr = false;
		bool foundBegin = false;
		while ((uint32_t)(cursor - buffer) < len) {
			if (0 == sum) {
				/*
				 * We are past the schema that was just parsed and found a new
				 * opening bracket, so break out.
				 */
				if (*cursor == '{') {
					break;
				}
			}
			else if (!foundBegin) {
				if ('{' == *cursor) {
					foundBegin = true;
				}
			}
			else if (inStr) {
				if (*cursor == '"') {
					inStr = false;
				}
			} else {
				switch(*cursor) {
				case '"': inStr = true; break;
				case '{': ++sum; break;
				case '}': --sum; break;
				}
			}
			++cursor;
		}
		*usedLen = cursor-buffer;
	}

	__return:
	if (jvalue) {
		jvalue->Release();
	}
	return !!m_rootObj;
}

bool JsonRpcParser::IsValidRpcSchema()
{
	const char* strVal = 0;
	strVal = IJsonValueReader::AsStringSafe(m_rootObj->RefMember("jsonrpc"));
	if (0 != strVal && 0 == strcmp(strVal,"2.0")) {
		return true;
	}
	return false;
}

SdCommandCode JsonRpcParser::GetRpcMethod()
{
	SdCommandCode cmdCode = SD_COMMAND_NONE;
	IJsonValueReader* jval = 0;
	if (!m_rootObj) {
		goto __return;
	}

	if (0 == (jval = m_rootObj->RefMember("method"))) {
		printf("JsonRpcParser::GetRpcMethod: member method not found!\n");
		goto __return;
	}
	if (jval->GetType() != SD_JSONVALUE_STRING) {
		printf("JsonRpcParser::GetRpcMethod: unexpected method type %s\n",
				jval->GetTypeAsString());
		goto __return;
	}

	cmdCode = SdStringToCommandCode(jval->AsString());

	__return:
	if (jval) {
		jval->Release();
	}
	return cmdCode;
}

IJsonObjectReader* JsonRpcParser::GetRpcParams()
{
	return !!m_rootObj ? m_rootObj->RefMemberAsObject("params") : 0;
}

uint64_t JsonRpcParser::GetRpcCallId()
{
	uint64_t callId = -1;
	if (m_rootObj) {
		m_rootObj->GetMemberAsIntValue("id",(int64_t*)&callId);
	}
	return callId;
}

void JsonRpcParser::ParseImuConfig(
	IJsonObjectReader* obj,
	SdImuDeviceConfig* imu)
{
	if (obj) {
		obj->GetMemberAsStringValue("DeviceName",&imu->DeviceName);
		obj->GetMemberAsIntValue("SamplingRate",&imu->SamplingRate);
		obj->GetMemberAsIntValue("Scale",&imu->Scale);
		obj->GetMemberAsIntValue("MaxReading",&imu->MaxReading);
		obj->GetMemberAsIntValue("Watermark",&imu->Watermark);
		obj->GetMemberAsIntValue("NumBiasSamples",&imu->NumBiasSamples);
		IJsonArrayReader* arr;
		if (0 != (arr = obj->RefMemberAsArray("CoordinateMap"))) {
			if (arr->ElementCount() == 9) {
				for (uint32_t i = 0; i < 9; i++) {
					int32_t intVal;
					if (arr->GetElementAsIntValue(i,&intVal)) {
						imu->CoordinateMap.at(i/3,i%3)=intVal;
					}
				}
			}
			arr->Release();
		}
		obj->Release();
	}
}

bool JsonRpcParser::GetDroneConfig(SdDroneConfig* cfg)
{
	IJsonObjectReader* config = 0;
	IJsonObjectReader* obj = 0;
	IJsonArrayReader* arr = 0;
	const char* strVal = 0;
	IJsonObjectReader* params = GetRpcParams();

	if (!params) {
		goto __return;
	}
	if (0 == (config = params->RefMemberAsObject("GlobalConfig"))) {
		goto __return;
	}

	ParseImuConfig(config->RefMemberAsObject("Gyro"),&cfg->Gyro);
	ParseImuConfig(config->RefMemberAsObject("Accelerometer"),&cfg->Accel);
	ParseImuConfig(config->RefMemberAsObject("Magnetometer"),&cfg->Mag);

	if (0 != (obj = config->RefMemberAsObject("Servo"))) {
		obj->GetMemberAsStringValue("DeviceName",&cfg->Servo.DeviceName);
		obj->GetMemberAsIntValue("SamplingRate",&cfg->Servo.Rate);
		obj->GetMemberAsIntValue("ChannelMask",(int32_t*)&cfg->Servo.ChannelMask);
		obj->GetMemberAsIntValue("BitCount",(int32_t*)&cfg->Servo.BitCount);
		obj->Release();
	}

	if (0 != (obj = config->RefMemberAsObject("QuadPilot"))) {
		obj->GetMemberAsDoubleValue("Kp",&cfg->Quad.Kp);
		obj->GetMemberAsDoubleValue("Ki",&cfg->Quad.Ki);
		obj->GetMemberAsDoubleValue("Kd",&cfg->Quad.Kd);
		if (0 != (arr = obj->RefMemberAsArray("MotorToServoMap"))) {
			for (size_t i = 0; i < 4; i++) {
				arr->GetElementAsIntValue(i,&cfg->Quad.Motor[i]);
			}
			arr->Release();
		}
		if (0 != (arr = obj->RefMemberAsArray("ImuCoordinateAngles"))) {
			arr->GetElementAsIntValue(2,&cfg->Quad.ImuAngleAxisZ);
		}
		obj->Release();
	}

	if (0 != (obj = config->RefMemberAsObject("ControlChannel"))) {
		obj->GetMemberAsStringValue("ServerAddress",&cfg->ServerAddress);
		obj->GetMemberAsIntValue("ServerPort",&cfg->ServerPort);
		obj->Release();
	}

	if (config->GetMemberAsStringValue("LogLevel",&strVal)) {
		cfg->LogLevel = SdStringToLogLevel(strVal);
	}
	config->GetMemberAsStringValue("LogFileName",&cfg->LogFileName);
	config->GetMemberAsIntValue("LogRotationMatrix", &cfg->LogRotationMatrix);
	config->GetMemberAsDoubleValue("LogRate", &cfg->LogRate);

	__return:
	if (config) {
		config->Release();
	}
	if (params) {
		params->Release();
	}
	return !!config;
}

bool JsonRpcParser::GetThrust(
		double* thrust,
		double* minThrust,
		double* maxThrust)
{
	IJsonObjectReader* flight = 0;
	IJsonObjectReader* params = 0;

	if (0 == (params=GetRpcParams())) {
		goto __return;
	}
	if (0 == (flight = params->RefMemberAsObject("Flight"))) {
		goto __return;
	}

	if (thrust) {
		flight->GetMemberAsDoubleValue("Thrust",thrust);
	}
	if (maxThrust) {
		flight->GetMemberAsDoubleValue("MaxThrust",maxThrust);
	}
	if (minThrust) {
		flight->GetMemberAsDoubleValue("MinThrust",minThrust);
	}

	__return:
	if (flight) {
		flight->Release();
	}
	if (params) {
		params->Release();
	}
	return !!flight;
}

void JsonRpcParser::ParseObject(IJsonObjectReader* jobj)
{
	if (!jobj) {
		return;
	}
	for (size_t i = 0; i < jobj->GetMemberCount(); i++) {
		const char* memberName = jobj->GetMemberName(i);
		IJsonValueReader* jval = jobj->RefMember(memberName);
		printf("%s: ",memberName);
		ParseNode(jval);
	}
	jobj->Release();
}

void JsonRpcParser::ParseNode(IJsonValueReader* jnode)
{
	if (!jnode) {
		return;
	}
	SdJsonValueType valType = jnode->GetType();
	//printf("Node type %s\n", json_node_type_name(jnode));
	if (SD_JSONVALUE_OBJECT == valType) {
		printf("\n");
		ParseObject(jnode->RefAsObject());
	} else if (SD_JSONVALUE_ARRAY == valType) {
		ParseArray(jnode->RefAsArray());
	} else {
		switch(valType) {
		case SD_JSONVALUE_BOOL:
			printf("%d\n",jnode->AsBool());
			break;
		case SD_JSONVALUE_INT:
			printf("%lld\n",(long long)jnode->AsInt());
			break;
		case SD_JSONVALUE_DOUBLE:
			printf("%f\n",jnode->AsDouble());
			break;
		case SD_JSONVALUE_STRING:
			printf("%s\n",jnode->AsString());
			break;
		default:
			printf("fatal: unrecognized value type: %d\n", valType);
		}
	}
	jnode->Release();
}

void JsonRpcParser::ParseArray(IJsonArrayReader* jarr)
{
	if (!jarr) {
		return;
	}
	uint32_t len = jarr->ElementCount();
	for (uint32_t i = 0; i < len; i++) {
		ParseNode(jarr->RefElement(i));
	}
}
