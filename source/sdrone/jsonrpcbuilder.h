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
			IN IJsonObjectReader*);
	bool SetValueAsArray(
			IN IJsonArrayReader*);
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
	IJsonValueReader* RefValue();

protected:
	~JsonValueBuilder() {}
};


class JsonRpcBuilder {
public:
	JsonRpcBuilder();
	~JsonRpcBuilder();
	void BeginRequest(const char* method, uint32_t id);
	void BeginReply(uint32_t id);
	void Finalize();
	void Reset();
	int AddParams(IJsonValueReader*);
	const char* GetSchema() const;
	size_t GetSchemaLen() consts;

private:
	JsonGenerator* m_jsonGenerator;
	IJsonValueReader* m_root;
};

#endif /* JSONRPCBUILDER_H_ */
