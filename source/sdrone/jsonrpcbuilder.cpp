/*
 * jsonrpcbuilder.cpp
 *
 *  Created on: Jan 27, 2014
 *      Author: svassilev
 */

#include "jsonrpcbuilder.h"

JsonRpcBuilder::JsonRpcBuilder() {
}

JsonRpcBuilder::~JsonRpcBuilder() {
}

void JsonRpcBuilder::BeginRequest(const char* method, uint32_t id) {
}

void JsonRpcBuilder::BeginReply(uint32_t id) {
}

void JsonRpcBuilder::Finalize() {
}

void JsonRpcBuilder::Reset() {
}

int JsonRpcBuilder::AddParams(IJsonValueReader*) {
	return 0;
}

const char* JsonRpcBuilder::GetSchema() {
	return 0;
}

size_t JsonRpcBuilder::GetSchemaLen() {
	return 0;
}
