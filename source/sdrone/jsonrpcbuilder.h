/*
 * jsonrpcparser.h
 *
 *  Created on: Dec 15, 2013
 *      Author: svassilev
 */

#ifndef JSONRPCBUILDER_H_
#define JSONRPCBUILDER_H_

#include "commoninc.h"
#include "jsonreadwrite.h"

static const uint32_t SD_INVALID_RPC_ID = 0;

struct _JsonGenerator;

class SdJsonRpcBuilder {
public:
	SdJsonRpcBuilder();
	~SdJsonRpcBuilder();
	bool BuildRequest(
			const char* method,
			const IJsonValue* params = 0,
			uint32_t rpcId = SD_INVALID_RPC_ID);
	bool BuildRequest(
			const char* method,
			IJsonObject* params = 0,
			uint32_t rpcId = SD_INVALID_RPC_ID);
	bool BuildRequest(
			const char* method,
			IJsonArray* params = 0,
			uint32_t rpcId = SD_INVALID_RPC_ID);
	bool BuildReply(
			IJsonValue* result,
			uint32_t id,
			bool error);
	void Reset();
	const char* GetJsonStream();
	size_t GetJsonStreamSize();

	static uint32_t GenerateRpcId();
private:
	static uint32_t s_rpcId;
	_JsonGenerator* m_jsonGenerator;
	char* m_jsonStream;
	size_t m_jsonStreamSize;
};

#endif /* JSONRPCBUILDER_H_ */
