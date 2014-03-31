/*
 * jsonrpcdispatch.cpp
 *
 *  Created on: Mar 24, 2014
 *      Author: svassilev
 */
#include "jsonrpcdispatch.h"

SdJsonRpcDispatcher::SdJsonRpcDispatcher(
		IRpcTransport::TransportType transportType)
{
	m_transport = IRpcTransport::Create(transportType);
}

SdJsonRpcDispatcher::~SdJsonRpcDispatcher()
{
	if (m_transport) {
		delete m_transport;
	}
}

bool SdJsonRpcDispatcher::StartServingRequests(
	const std::string& localAddress,
	int port)
{
	if (!m_transport) {
		return false;
	}
	return m_transport->StartServer(this, localAddress, port);
}

void SdJsonRpcDispatcher::DeleteRequestCallback(
	const char* methodName)
{
	m_callbacks.erase(methodName);
}

void SdJsonRpcDispatcher::StopServingRequests() {
	if (m_transport) {
		m_transport->StopServer();
	}
}

void SdJsonRpcDispatcher::AddRequestCallback(
		const char* methodName,
		SdJsonRpcRequestCallbackT callback,
		void* context)
{
	CallbackEntry entry = {callback,context};
	m_callbacks[methodName] = entry;
}

bool SdJsonRpcDispatcher::SendJsonRequest(
		const SdJsonRpcRequest& request,
		const std::string& remoteHost,
		int remotePort,
		SdJsonRpcReply* reply)
{
	bool ret = false;
	if (m_transport) {
		SdJsonRpcBuilder bldr;
		std::string serializedRequest;
		std::string serializedReply;
		bldr.BuildRequest(request.MethodName,request.Params,request.Id);
		serializedRequest.assign(bldr.GetJsonStream(),bldr.GetJsonStreamSize());
		if (m_transport->SendData(remoteHost,remotePort,serializedRequest,
				serializedReply)) {
			SdJsonRpcParser parser;
			parser.ParseBuffer(serializedReply.c_str(),
					serializedReply.length(),0);
			if (parser.IsValidRpcSchema() && parser.IsReply()) {
				if (reply) {
					reply->ErrorCode = parser.GetErrorCode();
					if (parser.IsErrorReply()) {
						reply->ErrorMessage = parser.GetErrorMessage();
					} else {
						reply->Results = parser.GetResult();
					}
				}
				ret = true;
			}
		}
	}
	return ret;
}

int SdJsonRpcDispatcher::ReceiveData(
		const std::string& data,
		size_t* sizeUsed)
{
}
