/*
 * SdJsonRpcTransport.cpp
 *
 *  Created on: Mar 25, 2014
 *      Author: svassilev
 */

#include "jsonrpctransport.h"
#include "jsonrpcparser.h"
#include <stdlib.h>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "http/http_server.hpp"

class SdJsonRpcTransport : public IRpcTransport{
public:
	SdJsonRpcTransport();
	virtual ~SdJsonRpcTransport();
	bool StartServer(
			IRpcReceiveDataSink* dataSink,
			const std::string& localAddress,
			int port);
	void StopServer();
	bool SendData(
			const std::string& remoteHost,
			int port,
			const std::string& dataIn,
			std::string& dataOut);
	void JsonRequestHandler(
			const http::server::request& req,
			http::server::reply& rep);
private:
	IRpcReceiveDataSink* m_dataSink;
	boost::asio::io_service m_io_service_rpc;
	boost::scoped_ptr<http::server::http_server> m_rpc_server;
};


SdJsonRpcTransport::SdJsonRpcTransport() : m_dataSink(0),
		m_io_service_rpc(), m_rpc_server()
{
}

SdJsonRpcTransport::~SdJsonRpcTransport() {
	if (m_rpc_server) {
		m_rpc_server->stop();
	}
}

IRpcTransport* IRpcTransport::Create(IRpcTransport::TransportType tr)
{
	if (tr == TRANSPORT_HTTP) {
		return new SdJsonRpcTransport();
	}
	return 0;
}

bool SdJsonRpcTransport::StartServer(
		IRpcReceiveDataSink* dataSink,
		const std::string& localAddress,
		int port)
{
	char portAsStr[6]={0};
	assert(dataSink);
	StopServer();
	m_dataSink = dataSink;
	snprintf((char*)portAsStr,ARRAYSIZE(portAsStr),"%d",port);
	m_rpc_server.reset(new http::server::http_server(m_io_service_rpc,
			localAddress, portAsStr));
	m_rpc_server->add_uri_handler("/jsonrpc",
			boost::bind(&SdJsonRpcTransport::JsonRequestHandler, this, _1, _2));

	m_io_service_rpc.run();
	return true;
}

void SdJsonRpcTransport::StopServer() {
	if (m_rpc_server) {
		m_rpc_server->stop();
	}
}

bool SdJsonRpcTransport::SendData(
		const std::string& remoteHost,
		int port,
		const std::string& dataIn,
		std::string& dataOut)
{
	return false;
}

void SdJsonRpcTransport::JsonRequestHandler(
	const http::server::request& req,
	http::server::reply& rep)
{
	SdJsonRpcParser rpcParser;
	rpcParser.ParseBuffer(req.content.c_str(),req.content.length(),0);
}
