/*
 * jsonrpcdispatch.h
 *
 *  Created on: Mar 24, 2014
 *      Author: svassilev
 */

#ifndef JSONRPCDISPATCH_H_
#define JSONRPCDISPATCH_H_

#include "jsonreadwrite.h"
#include "jsonrpctransport.h"

struct SdJsonRpcRequest
{
	SdJsonRpcRequest() {
		Id = SD_JSONRPC_INVALID_ID;
		Params = 0;
	}
	std::string MethodName;
	SdJsonValue Params;
	uint64_t Id; // may not be initialized
};

struct SdJsonRpcReply
{
	SdJsonRpcReply() {
		Id = SD_JSONRPC_INVALID_ID;
		ErrorCode = 0;
	}
	SdJsonValue Results;
	uint64_t Id;
	int ErrorCode;
	std::string ErrorMessage;
};

struct SdJsonRpcMethod {
	SdJsonRpcMethod(const std::string& methodName) {}
	virtual ~SdJsonRpcMethod() {}
	virtual bool Execute(
			const SdJsonRpcRequest*,
			SdJsonRpcReply*);
	const std::string& Name();
	void PrintRequestSchema();
	void PrintReplySchema();
private:
	SdJsonRpcReply reply;
	SdJsonRpcRequest request;
	std::string methodName;
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
