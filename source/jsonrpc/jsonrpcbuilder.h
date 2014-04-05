/*
 * jsonrpcparser.h
 *
 *  Created on: Dec 15, 2013
 *      Author: svassilev
 */

#ifndef JSONRPCBUILDER_H_
#define JSONRPCBUILDER_H_

#include "jsonreadwrite.h"

struct _JsonGenerator;

class SdJsonRpcBuilder {
public:
	SdJsonRpcBuilder();
	~SdJsonRpcBuilder();
	bool BuildRequest(
			const char* method,
			const IJsonValue* params = 0,
			uint32_t rpcId = SD_JSONRPC_INVALID_ID);
	bool BuildRequest(
			const char* method,
			const IJsonObject* params = 0,
			uint32_t rpcId = SD_JSONRPC_INVALID_ID);
	bool BuildRequest(
			const char* method,
			const IJsonArray* params = 0,
			uint32_t rpcId = SD_JSONRPC_INVALID_ID);
	bool BuildReply(
			const IJsonValue* result,
			uint32_t id,
			int error,
			const std::string& errorMessage);
	void Reset();
	const char* GetJsonStream();
	uint32_t GetJsonStreamSize();

	static uint32_t GenerateRpcId();
private:
	static uint32_t s_rpcId;
	_JsonGenerator* m_jsonGenerator;
	char* m_jsonStream;
	size_t m_jsonStreamSize;
};

#endif /* JSONRPCBUILDER_H_ */
