/*
 * jsonrpcparser.h
 *
 *  Created on: Dec 15, 2013
 *      Author: svassilev
 */

#ifndef JSONRPCPARSER_H_
#define JSONRPCPARSER_H_

#include "jsonreadwrite.h"

struct _JsonParser;

class SdJsonParser {
public:
	SdJsonParser();
	virtual ~SdJsonParser();
	bool ParseBuffer(
		const char* buffer,
		uint32_t len,
		uint32_t* usedLen
		);
	bool ParseFile(const char* fileName);
	const IJsonObject* RootObj() const { return m_rootNode.AsObject(); }

	static void PrintJsonObject(const IJsonObject*);
	static void PrintJsonArray(const IJsonArray*);
	static void PrintJsonNode(const IJsonValue*);

private:
	SdJsonValue m_rootNode;
};

class SdJsonRpcParser: public SdJsonParser
{
public:
	SdJsonRpcParser() {}
	virtual ~SdJsonRpcParser() {}
	uint64_t GetRpcCallId() const;
	bool IsValidRpcSchema() const;
	bool IsRequest() const;
	bool IsReply() const;
	bool IsErrorReply() const;
	std::string GetRpcMethod() const;
	const IJsonValue* GetRpcParams() const;
	const IJsonValue* GetResult() const;
	const IJsonObject* GetError() const;
	int GetErrorCode() const;
	std::string GetErrorMessage() const;
};
#endif /* JSONRPCPARSER_H_ */
