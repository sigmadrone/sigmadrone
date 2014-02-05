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

class JsonRpcBuilder {
public:
	JsonRpcBuilder();
	~JsonRpcBuilder();
	void BeginRequest(const char* method, uint32_t id);
	void BeginReply(uint32_t id);
	void Finalize();
	void Reset();
	int AddParams(IJsonValueReader*);
	const char* GetSchema();
	size_t GetSchemaLen();

private:
	JsonGenerator* m_jsonGenerator;
	IJsonValueReader* m_root;
};

#endif /* JSONRPCBUILDER_H_ */
