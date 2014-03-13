/*
 * jsonrpcbuilder.cpp
 *
 *  Created on: Jan 27, 2014
 *      Author: svassilev
 */

#include "jsonrpcbuilder.h"

uint32_t JsonRpcBuilder::s_rpcId = 0;

JsonRpcBuilder::JsonRpcBuilder() :
		m_jsonGenerator(0),
		m_jsonStream(0),
		m_jsonStreamSize(0)
{
}

JsonRpcBuilder::~JsonRpcBuilder() {
	Reset();
}

bool JsonRpcBuilder::BuildRequest(
	const char* methodName,
	IJsonValueReader* params,
	uint32_t _rpcId)
{
	bool ret = false;
	JsonValueWriter* root = 0;
	JsonObjectWriter* rootObj = 0;
	uint32_t rpcId = _rpcId;

	Reset();

	if (0 == (m_jsonGenerator = json_generator_new()) ||
		0 == (root = new JsonValueWriter()) ||
		0 == (rootObj = new JsonObjectWriter())) {
		goto __return;
	}
	if (SD_INVALID_RPC_ID == rpcId) {
		rpcId = GenerateRpcId();
	}

	rootObj->AddStringMember("jsonrpc","2.0");
	rootObj->AddStringMember("method",methodName);
	if (params) {
		rootObj->AddMember("params",params);
	}
	rootObj->AddIntMember("id",rpcId);
	root->SetValueAsObject(rootObj);

	json_generator_set_root(m_jsonGenerator,root->GetJNode());

	__return:
	if (root) {
		root->Release();
	}
	if (rootObj) {
		rootObj->Release();
	}
	return ret;
}

bool JsonRpcBuilder::BuildReply(
		IJsonValueReader* result,
		uint32_t id,
		bool error)
{
	return false;
}

void JsonRpcBuilder::Reset() {
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

const char* JsonRpcBuilder::GetJsonStream()
{
	if (0 != m_jsonStream) {
		m_jsonStream = json_generator_to_data(m_jsonGenerator,
				&m_jsonStreamSize);
	}
	return m_jsonStream;
}

size_t JsonRpcBuilder::GetJsonStreamSize()
{
	GetJsonStream();
	return m_jsonStreamSize;
}

uint32_t JsonRpcBuilder::GenerateRpcId()
{
	uint32_t id;
	do {
		id = __sync_fetch_and_add((uint32_t*)&s_rpcId,1);
	}
	while(SD_INVALID_RPC_ID == id);
	return id;
}
