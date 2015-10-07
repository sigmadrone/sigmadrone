/*
 *  Sigmadrone
 *  Copyright (c) 2013-2015 The Sigmadrone Developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Martin Stoilov <martin@sigmadrone.org>
 *  Svetoslav Vassilev <svassilev@sigmadrone.org>
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
	const SdJsonValue& RootNode() const { return m_rootNode; }

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
	SdJsonValue GetRpcCallId() const;
	bool IsValidRpcSchema() const;
	bool IsRequest() const;
	bool IsReply() const;
	bool IsErrorReply() const;
	std::string GetRpcMethod() const;
	SdJsonValue GetRpcParams() const;
	const IJsonValue* GetResult() const;
	const IJsonObject* GetError() const;
	int GetErrorCode() const;
	std::string GetErrorMessage() const;
};
#endif /* JSONRPCPARSER_H_ */
