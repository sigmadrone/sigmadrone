/*
 * jsonrpcparser.h
 *
 *  Created on: Dec 15, 2013
 *      Author: svassilev
 */

#ifndef JSONRPCBUILDER_H_
#define JSONRPCBUILDER_H_

#include "commoninc.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

class JsonValueBuilder: public IJsonValueBuilder
{
public:
	void AddRef();
	void Release();
	void Reset();
	bool SetValueAsObject(
			IN IParsedJsonObject*);
	bool SetValueAsArray(
			IN IParsedJsonArray*);
	bool SetValueAsDouble(
			IN double value);
	bool SetValueAsInt(
			IN int32_t value);
	bool SetValueAsInt64(
			OUT int64_t value);
	bool SetValueAsBool(
			IN bool value);
	bool SetValueAsString(
			IN const char* value
			);
	IParsedJsonValue* RefValue();

protected:
	~JsonValueBuilder() {}
};


class JsonRpcBuilder {
public:
	JsonRpcBuilder();
	~JsonRpcBuilder();
	void BeginRequest(const char* method, uint32_t id);
	void BeginReply(uint32_t id);
	void Reset();
	int AddParams(IParsedJsonValue*);
	const char*

private:
	JsonGenerator* m_jsonGenerator;
	IParsedJsonValue* m_root;
};

#endif /* JSONRPCBUILDER_H_ */
