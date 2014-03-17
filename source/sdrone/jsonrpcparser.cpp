/*
 * jsonrpcparser.cpp
 *
 *  Created on: Dec 15, 2013
 *      Author: svassilev
 */

#include "jsonrpcparser.h"
#include "jsonreadwrite.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

struct GLibInit {
	GLibInit() { g_type_init(); }
} glibInit;


SdJsonParser::SdJsonParser() {}

SdJsonParser::~SdJsonParser() {}

bool SdJsonParser::ParseFile(
		const char* fileName)
{
	GError* gerr=0;
	JsonParser* jsonParser = 0;

	m_rootNode = SdJsonValue();
	if (!(jsonParser = json_parser_new())) {
		printf("SdJsonParser error: json_parser_new failed!\n");
		goto __return;
	}

	if (!json_parser_load_from_file(jsonParser,fileName,&gerr)) {
		printf("SdJsonParser error: %s\n", gerr->message);
		goto __return;
	}

	m_rootNode = SdJsonValue(json_parser_get_root(jsonParser));
	if (m_rootNode.GetType() != SD_JSONVALUE_OBJECT) {
		printf("SdJsonParser error: unexpected root type %s\n",
				m_rootNode.GetTypeAsString());
		goto __return;
	}

	__return:
	if (jsonParser) {
		g_object_unref(jsonParser);
	}
	return m_rootNode.AsObject();
}

bool SdJsonParser::ParseBuffer(
		const char* buffer,
		uint32_t len,
		uint32_t* usedLen)
{
	GError* gerr=0;
	JsonParser* jsonParser = 0;

	if (usedLen) {
		*usedLen = 0;
	}

	m_rootNode = SdJsonValue();

	if (!(jsonParser = json_parser_new())) {
		goto __return;
	}

	if (!json_parser_load_from_data(jsonParser, buffer, len, &gerr)) {
		printf("JsonRpcParser error: %s\n", gerr->message);
		goto __return;
	}

	m_rootNode = SdJsonValue(json_parser_get_root(jsonParser));
	if (m_rootNode.GetType() != SD_JSONVALUE_OBJECT) {
		printf("SdJsonParser error: unexpected root type %s\n",
				m_rootNode.GetTypeAsString());
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
	if (jsonParser) {
		g_object_unref(jsonParser);
	}
	return !!m_rootNode.AsObject();
}


void SdJsonParser::PrintJsonObject(const IJsonObject* jobj)
{
	if (jobj) {
		for (size_t i = 0; i < jobj->GetMemberCount(); i++) {
			const char* memberName = jobj->GetMemberName(i);
			printf("%s: ",memberName);
			PrintJsonNode(jobj->GetMember(memberName));
		}
	}
}

void SdJsonParser::PrintJsonNode(const IJsonValue* jnode)
{
	if (!jnode) {
		return;
	}
	SdJsonValueType valType = jnode->GetType();
	//printf("Node type %s\n", json_node_type_name(jnode));
	if (SD_JSONVALUE_OBJECT == valType) {
		printf("\n");
		PrintJsonObject(jnode->AsObject());
	} else if (SD_JSONVALUE_ARRAY == valType) {
		PrintJsonArray(jnode->AsArray());
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
			printf("%s\n",jnode->AsString().c_str());
			break;
		default:
			printf("fatal: unrecognized value type: %d\n", valType);
		}
	}
}

void SdJsonParser::PrintJsonArray(const IJsonArray* jarr)
{
	if (jarr) {
		for (uint32_t i = 0; i < jarr->ElementCount(); i++) {
			PrintJsonNode(jarr->GetElement(i));
		}
	}
}


bool SdJsonRpcParser::IsValidRpcSchema()
{
	if (RootObj() && RootObj()->GetMember("jsonrpc")) {
		return RootObj()->GetMember("jsonrpc")->AsString() == "2.0";
	}
	return false;
}

std::string SdJsonRpcParser::GetRpcMethod()
{
	if (RootObj() && RootObj()->GetMember("method")) {
		return RootObj()->GetMember("method")->AsString();
	}
	return "";
}

const IJsonObject* SdJsonRpcParser::GetRpcParams()
{
	return !!RootObj() ? RootObj()->GetMember("params")->AsObject() : 0;
}

uint64_t SdJsonRpcParser::GetRpcCallId()
{
	uint64_t callId = 0;
	if (RootObj()) {
		RootObj()->GetMember("id")->AsIntSafe((int64_t*)&callId);
	}
	return callId;
}


// TODO: The standalone parse routine will have to be moved to the corresponding
// rpc callback routines
bool SdJsonParseImuConfig(
	const IJsonObject* obj,
	SdImuDeviceConfig* imu)
{
	if (obj) {
		imu->DeviceName = obj->GetMember("DeviceName")->AsString();
		imu->SamplingRate = obj->GetMember("SamplingRate")->AsInt();
		imu->Scale = obj->GetMember("Scale")->AsInt();
		imu->MaxReading = obj->GetMember("MaxReading")->AsInt();
		imu->Watermark = obj->GetMember("Watermark")->AsInt();
		imu->NumBiasSamples = obj->GetMember("NumBiasSamples")->AsInt();
		const IJsonArray* arr;
		if (0 != (arr = obj->GetMember("CoordinateMap")->AsArray())) {
			if (arr->ElementCount() == 9) {
				for (uint32_t i = 0; i < 9; i++) {
					imu->CoordinateMap.at(i/3,i%3)=arr->GetElement(i)->AsInt();
				}
			}
		}
	}
	return !!obj;
}

bool SdJsonParseDroneConfig(
		const IJsonObject* params,
		SdDroneConfig* droneCfg)
{
	const IJsonObject* config = 0;
	const IJsonObject* obj = 0;
	const IJsonArray* arr = 0;

	if (0 == (config = params->GetMember("GlobalConfig")->AsObject())) {
		return false;
	}

	SdJsonParseImuConfig(config->GetMember("Gyro")->AsObject(),
			&droneCfg->Gyro);
	SdJsonParseImuConfig(config->GetMember("Accelerometer")->AsObject(),
			&droneCfg->Accel);
	SdJsonParseImuConfig(config->GetMember("Magnetometer")->AsObject(),
			&droneCfg->Mag);

	if (0 != (obj = config->GetMember("Servo")->AsObject())) {
		droneCfg->Servo.DeviceName = obj->GetMember("DeviceName")->AsString();
		droneCfg->Servo.Rate = (uint32_t)obj->GetMember("SamplingRate")->AsInt();
		droneCfg->Servo.ChannelMask = (uint32_t)obj->GetMember("ChannelMask")->AsInt();
		droneCfg->Servo.BitCount = (uint32_t)obj->GetMember("BitCount")->AsInt();
	}

	if (0 != (obj = config->GetMember("QuadPilot")->AsObject())) {
		droneCfg->Quad.Kp = obj->GetMember("Kp")->AsDouble();
		droneCfg->Quad.Ki = obj->GetMember("Ki")->AsDouble();
		droneCfg->Quad.Kd = obj->GetMember("Kd")->AsDouble();
		if (0 != (arr = obj->GetMember("MotorToServoMap")->AsArray()) &&
			arr->ElementCount() == 4) {
			for (size_t i = 0; i < 4; i++) {
				droneCfg->Quad.Motor[i] = (uint32_t)arr->GetElement(i)->AsInt();
			}
		}

		if (0 != (arr = obj->GetMember("ImuCoordinateAngles")->AsArray()) &&
			arr->ElementCount() == 3) {
			droneCfg->Quad.ImuAngleAxisZ = arr->GetElement(2)->AsInt();
		}
	}

	if (0 != (obj = config->GetMember("ControlChannel")->AsObject())) {
		droneCfg->ServerAddress = obj->GetMember("ServerAddress")->AsString();
		droneCfg->ServerPort = (int32_t)obj->GetMember("ServerPort")->AsInt();
	}
	if (config->GetMember("LogLevel")->GetType() == SD_JSONVALUE_STRING) {
		droneCfg->LogLevel = SdStringToLogLevel(
				config->GetMember("LogLevel")->AsString().c_str());
	}

	droneCfg->LogFileName = config->GetMember("LogFileName")->AsString();
	droneCfg->LogRotationMatrix = (int32_t)
			config->GetMember("LogRotationMatrix")->AsInt();
	droneCfg->LogRate = config->GetMember("LogRate")->AsDouble();
	return true;
}

bool SdJsonParseParseThrust(
		const IJsonObject* rpcParams,
		double* thrust,
		double* minThrust,
		double* maxThrust)
{
	const IJsonObject* flight = 0;
	if (0 != (flight = rpcParams->GetMember("Flight")->AsObject())) {
		if (thrust) {
			*thrust = flight->GetMember("Thrust")->AsDouble();
		}
		if (maxThrust) {
			*maxThrust = flight->GetMember("MaxThrust")->AsDouble();
		}
		if (minThrust) {
			*minThrust = flight->GetMember("MinThrust")->AsDouble();
		}
	}
	return !!flight;
}
