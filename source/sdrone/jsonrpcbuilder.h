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

class JsonRpcBuilder {
public:
	JsonRpcBuilder();
	~JsonRpcBuilder();
	bool BuildRequest(
			const char* method,
			IJsonValueReader* params = 0,
			uint32_t rpcId = SD_INVALID_RPC_ID);
	bool BuildReply(
			IJsonValueReader* result,
			uint32_t id,
			bool error);
	void Reset();
	const char* GetJsonStream();
	size_t GetJsonStreamSize();

	static uint32_t GenerateRpcId();
private:
	static uint32_t s_rpcId;
	JsonGenerator* m_jsonGenerator;
	char* m_jsonStream;
	size_t m_jsonStreamSize;
};

#endif /* JSONRPCBUILDER_H_ */
