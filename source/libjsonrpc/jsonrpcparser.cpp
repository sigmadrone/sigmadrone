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
		assert(gerr && gerr->message);
		printf("JsonRpcParser error: %s\n", gerr->message);
		goto __return;
	}

	if (!json_parser_get_root(jsonParser)) {
		printf("JsonRpcParser error - no root element!\n");
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


bool SdJsonRpcParser::IsValidRpcSchema() const
{
	if (RootObj() && RootObj()->IsMemberPresent("jsonrpc")) {
		return RootObj()->GetMember("jsonrpc")->AsString() == "2.0" &&
				(IsRequest() || IsReply());
	}
	return false;
}

std::string SdJsonRpcParser::GetRpcMethod() const
{
	if (RootObj() && RootObj()->IsMemberPresent("method")) {
		return RootObj()->GetMember("method")->AsString();
	}
	return "";
}

bool SdJsonRpcParser::IsRequest() const {
	return RootObj() && RootObj()->IsMemberPresent("method");
}

bool SdJsonRpcParser::IsReply() const {
	return RootObj() && (RootObj()->IsMemberPresent("error") ||
			RootObj()->IsMemberPresent("result"));
}

bool SdJsonRpcParser::IsErrorReply() const {
	return RootObj() && RootObj()->IsMemberPresent("error");
}

const IJsonValue* SdJsonRpcParser::GetRpcParams() const
{
	return !!RootObj() ? RootObj()->GetMember("params") : 0;
}

uint64_t SdJsonRpcParser::GetRpcCallId() const
{
	uint64_t callId = 0;
	if (RootObj()) {
		RootObj()->GetMember("id")->AsUintSafe(&callId);
	}
	return callId;
}

const IJsonValue* SdJsonRpcParser::GetResult() const {
	if (RootObj() && IsReply() && !IsErrorReply()) {
		return RootObj()->GetMember("result");
	}
	return 0;
}

const IJsonObject* SdJsonRpcParser::GetError() const {
	if (RootObj() && IsErrorReply()) {
		return RootObj()->GetMember("error")->AsObject();
	}
	return 0;
}

int SdJsonRpcParser::GetErrorCode() const {
	if (RootObj() && IsErrorReply()) {
		return (int)GetError()->GetMember("code")->AsInt();
	}
	return 0;
}

std::string SdJsonRpcParser::GetErrorMessage() const {
	if (RootObj() && IsErrorReply()) {
		return GetError()->GetMember("message")->AsString();
	}
	return "";
}
