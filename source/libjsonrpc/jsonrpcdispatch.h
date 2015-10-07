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

#ifndef JSONRPCDISPATCH_H_
#define JSONRPCDISPATCH_H_

#include "jsonreadwrite.h"
#include "jsonrpctransport.h"

struct SdJsonRpcRequest
{
	SdJsonRpcRequest() {
	}
	SdJsonRpcRequest(
			const std::string& method,
			const SdJsonValue& params=s_nullJsonValue,
			const SdJsonValue& id= SdJsonValue()) : MethodName(method), Params(params), Id(id) {}
	std::string MethodName;
	SdJsonValue Params;
	SdJsonValue Id; // may not be initialized
};

struct SdJsonRpcReply
{
	SdJsonRpcReply() {
		Id = SD_JSONRPC_INVALID_ID;
		ErrorCode = 0;
	}
	SdJsonValue Results;
	SdJsonValue Id;
	int ErrorCode;
	std::string ErrorMessage;
};

typedef void (*SdJsonRpcRequestCallbackT)(
		void* Context,
		const SdJsonRpcRequest*,
		SdJsonRpcReply*);

class SdJsonRpcDispatcher: public IRpcReceiveDataSink
{
public:
	SdJsonRpcDispatcher(IRpcTransport* transport);
	~SdJsonRpcDispatcher();
	bool StartServingRequests(
			const std::string& localAddress,
			int port);
	void StopServingRequests();
	void AddRequestCallback(
			const char* methodName,
			SdJsonRpcRequestCallbackT,
			void* context,
			const SdJsonValue& paramsSpec = SdJsonValue(),
			const SdJsonValue& resultSpec = SdJsonValue());
	void DeleteRequestCallback(const char* methodName);
	bool SendJsonRequest(
			const SdJsonRpcRequest& request,
			const std::string& remoteHost,
			int remotePort,
			SdJsonRpcReply* reply);
	bool GetParamsSpec(const std::string& methodName, SdJsonValue* spec);
	bool GetResultSpec(const std::string& methodName, SdJsonValue* spec);
	void GetRegisteredMethods(std::vector<std::string>& list);
private:

	int ReceiveData(
			const std::string& request,
			std::string& reply);

	struct CallbackEntry
	{
		SdJsonRpcRequestCallbackT Callback;
		void* Context;
		SdJsonValue ParamsSpec;
		SdJsonValue ResultSpec;
	};

	typedef std::map<std::string, CallbackEntry> CallbackMap;
	typedef CallbackMap::iterator CallbackMapIt;
	IRpcTransport* m_transport;
	CallbackMap m_callbacks;
};

#endif /* JSONRPCDISPATCH_H_ */
