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
			const SdJsonValue* params = 0,
			const SdJsonValue& rpcId = SdJsonValue());
	bool BuildRequest(
			const char* method,
			const SdJsonObject* params = 0,
			const SdJsonValue& rpcId = SdJsonValue());
	bool BuildRequest(
			const char* method,
			const SdJsonArray* params = 0,
			const SdJsonValue& rpcId = SdJsonValue());
	bool BuildReply(
			const SdJsonValue* result,
			const SdJsonValue& rpcId,
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
