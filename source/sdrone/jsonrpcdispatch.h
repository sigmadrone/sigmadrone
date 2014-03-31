/*
 * jsonrpcdispatch.h
 *
 *  Created on: Mar 24, 2014
 *      Author: svassilev
 */

#ifndef JSONRPCDISPATCH_H_
#define JSONRPCDISPATCH_H_

#include "jsonrpctransport.h"

struct SdJsonRpcRequest
{
	std::string MethodName;
	const IJsonObject* Params;
	uint64_t Id; // may not be initialized
};

struct SdJsonRpcReply
{
	SdJsonValue Results;
	uint64_t Id;
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
	SdJsonRpcDispatcher(IRpcTransport::TransportType transportType);
	~SdJsonRpcDispatcher();
	bool StartServingRequests(
			const std::string& localAddress,
			int port);
	void StopServingRequests();
	void AddRequestCallback(
			const char* methodName,
			SdJsonRpcRequestCallbackT,
			void* context);
	void DeleteRequestCallback(const char* methodName);
	bool SendJsonRequest(
			const SdJsonRpcRequest& request,
			const std::string& remoteHost,
			int remotePort,
			SdJsonRpcReply* reply);
	int ReceiveData(
			const std::string& request,
			std::string& reply);
private:
	struct CallbackEntry
	{
		SdJsonRpcRequestCallbackT Callback;
		void* Context;
	};

	typedef std::map<std::string, CallbackEntry> CallbackMap;
	typedef CallbackMap::iterator CallbackMapIt;
	IRpcTransport* m_transport;
	CallbackMap m_callbacks;
};

#endif /* JSONRPCDISPATCH_H_ */
