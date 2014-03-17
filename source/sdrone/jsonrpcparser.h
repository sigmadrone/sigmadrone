/*
 * jsonrpcparser.h
 *
 *  Created on: Dec 15, 2013
 *      Author: svassilev
 */

#ifndef JSONRPCPARSER_H_
#define JSONRPCPARSER_H_

#include "commoninc.h"
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
	const IJsonObject* RootObj() { return m_rootNode.AsObject(); }

	void PrintJsonObject(const IJsonObject*);
	void PrintJsonArray(const IJsonArray*);
	void PrintJsonNode(const IJsonValue*);

private:
	SdJsonValue m_rootNode;
};

bool SdJsonParseDroneConfig(
		const IJsonObject* jsonRpcParams,
		SdDroneConfig*);
bool SdJsonParseImuConfig(
		const IJsonObject* jsonImuObj,
		SdImuDeviceConfig*);
bool SdJsonParseParseThrust(
		const IJsonObject* jsonRpcParams,
		double* thrust,
		double* minThrust,
		double* maxThrust);

class SdJsonRpcParser: public SdJsonParser
{
public:
	SdJsonRpcParser() {}
	virtual ~SdJsonRpcParser() {}
	uint64_t GetRpcCallId();
	bool IsValidRpcSchema();
	std::string GetRpcMethod();
	const IJsonObject* GetRpcParams();
};

struct SdJsonRpcCallback
{
	virtual bool ExecuteRpc(uint64_t callId, IJsonObject* params) = 0;
};

class SdJsonRpcDispatcher
{
};

#endif /* JSONRPCPARSER_H_ */
