/*
 * jsonrpcparser.cpp
 *
 *  Created on: Dec 15, 2013
 *      Author: svassilev
 */

#include "jsonrpcparser.h"
#include "jsonreadwrite.h"

struct GLibInit {
	GLibInit() { g_type_init(); }
} glibInit;



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

	if (0 == (jvalue = new JsonValueReader(rootNode))) {
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

	if (0 == (jvalue = new JsonValueReader(
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
