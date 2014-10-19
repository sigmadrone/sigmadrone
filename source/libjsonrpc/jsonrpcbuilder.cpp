/*
 * jsonrpcbuilder.cpp
 *
 *  Created on: Jan 27, 2014
 *      Author: svassilev
 */

#include "jsonrpcbuilder.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>


uint32_t SdJsonRpcBuilder::s_rpcId = 1;

SdJsonRpcBuilder::SdJsonRpcBuilder() :
		m_jsonGenerator(0),
		m_jsonStream(0),
		m_jsonStreamSize(0)
{
}

SdJsonRpcBuilder::~SdJsonRpcBuilder() {
	Reset();
}

bool SdJsonRpcBuilder::BuildRequest(
	const char* methodName,
	const IJsonValue* params,
	const SdJsonValue& _rpcId)
{
	SdJsonValue root;
	SdJsonObject rootObj;
	SdJsonValue rpcId = _rpcId;

	Reset();

	if (0 == (m_jsonGenerator = json_generator_new())) {
		return false;
	}
	if (rpcId.GetType() == SD_JSONVALUE_NULL) {
		rpcId.SetValueAsInt(GenerateRpcId());
	}

	rootObj.AddMember("jsonrpc",SdJsonValue("2.0"));
	rootObj.AddMember("method",SdJsonValue(methodName));
	if (params && params->GetType() != SD_JSONVALUE_NULL) {
		rootObj.AddMember("params",params);
	}
	rootObj.AddMember("id",rpcId);
	root.SetValueAsObject(&rootObj);

	json_generator_set_root(m_jsonGenerator,root.PeekGlibNode());

	return true;
}

bool SdJsonRpcBuilder::BuildRequest(
		const char* method,
		const IJsonObject* params,
		const SdJsonValue& rpcId)
{
	SdJsonValue jval;
	if (params) {
		jval.SetValueAsObject(params);
	}
	return BuildRequest(method,&jval,rpcId);
}

bool SdJsonRpcBuilder::BuildRequest(
		const char* method,
		const IJsonArray* params,
		const SdJsonValue& rpcId)
{
	SdJsonValue jval;
	if (params) {
		jval.SetValueAsArray(params);
	}
	return BuildRequest(method,&jval,rpcId);
}

void SdJsonRpcBuilder::Reset() {
	if (m_jsonGenerator) {
		g_object_unref(m_jsonGenerator);
		m_jsonGenerator = 0;
	}
	if (m_jsonStream) {
		g_free(m_jsonStream);
		m_jsonStream = 0;
	}
	m_jsonStreamSize = 0;
}

bool SdJsonRpcBuilder::BuildReply(
		const IJsonValue* result,
		const SdJsonValue& rpcId,
		int error,
		const std::string& errorMessage)
{
	SdJsonValue root;
	SdJsonObject rootObj;

	Reset();

	if (0 == (m_jsonGenerator = json_generator_new())) {
		return false;
	}

	rootObj.AddMember("jsonrpc",SdJsonValue("2.0"));
	if (error >= 0) {
		if (result && result->GetType() != SD_JSONVALUE_NULL) {
			rootObj.AddMember("result",result);
		} else {
			rootObj.AddMember("result",SdJsonValue(0));
		}

	} else {
		SdJsonObject errObj;
		errObj.AddMember("code",error);
		errObj.AddMember("message",errorMessage);
		rootObj.AddMember("error",SdJsonValue(errObj));
	}
	rootObj.AddMember("id",rpcId);
	root.SetValueAsObject(&rootObj);

	json_generator_set_root(m_jsonGenerator,root.PeekGlibNode());

	return true;
}

const char* SdJsonRpcBuilder::GetJsonStream()
{
	if (0 == m_jsonStream) {
		m_jsonStream = json_generator_to_data(m_jsonGenerator,
				&m_jsonStreamSize);
	}
	return m_jsonStream;
}

uint32_t SdJsonRpcBuilder::GetJsonStreamSize()
{
	GetJsonStream();
	return (uint32_t)m_jsonStreamSize;
}

uint32_t SdJsonRpcBuilder::GenerateRpcId()
{
	uint32_t id;
	do {
		id = __sync_fetch_and_add((uint32_t*)&s_rpcId,1);
	}
	while(SD_JSONRPC_INVALID_ID == id);
	return id;
}
