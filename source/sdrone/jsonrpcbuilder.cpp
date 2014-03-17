/*
 * jsonrpcbuilder.cpp
 *
 *  Created on: Jan 27, 2014
 *      Author: svassilev
 */

#include "jsonrpcbuilder.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>


uint32_t SdJsonRpcBuilder::s_rpcId = 0;

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
	uint32_t _rpcId)
{
	SdJsonValue root;
	SdJsonObject rootObj;
	uint32_t rpcId = _rpcId;

	Reset();

	if (0 == (m_jsonGenerator = json_generator_new())) {
		return false;
	}
	if (SD_INVALID_RPC_ID == rpcId) {
		rpcId = GenerateRpcId();
	}

	rootObj.AddMember("jsonrpc",SdJsonValue("2.0"));
	rootObj.AddMember("method",SdJsonValue(methodName));
	if (params && params->GetType() != SD_JSONVALUE_NULL) {
		rootObj.AddMember("params",params);
	}
	rootObj.AddMember("id",SdJsonValue(rpcId));
	root.SetValueAsObject(&rootObj);

	json_generator_set_root(m_jsonGenerator,root.PeekGlibNode());

	return true;
}

bool SdJsonRpcBuilder::BuildRequest(
		const char* method,
		IJsonObject* params,
		uint32_t rpcId)
{
	SdJsonValue jval;
	if (params) {
		jval.SetValueAsObject(params);
	}
	return BuildRequest(method,&jval,rpcId);
}

bool SdJsonRpcBuilder::BuildRequest(
		const char* method,
		IJsonArray* params,
		uint32_t rpcId)
{
	SdJsonValue jval;
	if (params) {
		jval.SetValueAsArray(params);
	}
	return BuildRequest(method,&jval,rpcId);
}

bool SdJsonRpcBuilder::BuildReply(
		IJsonValue* result,
		uint32_t id,
		bool error)
{
	return false;
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

const char* SdJsonRpcBuilder::GetJsonStream()
{
	if (0 != m_jsonStream) {
		m_jsonStream = json_generator_to_data(m_jsonGenerator,
				&m_jsonStreamSize);
	}
	return m_jsonStream;
}

size_t SdJsonRpcBuilder::GetJsonStreamSize()
{
	GetJsonStream();
	return m_jsonStreamSize;
}

uint32_t SdJsonRpcBuilder::GenerateRpcId()
{
	uint32_t id;
	do {
		id = __sync_fetch_and_add((uint32_t*)&s_rpcId,1);
	}
	while(SD_INVALID_RPC_ID == id);
	return id;
}
