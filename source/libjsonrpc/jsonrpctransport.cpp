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
#include "libhttp/http_server.hpp"
#include "libhttp/http_client.hpp"

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
			http::server::connection& connection,
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
		const std::string& _localAddress,
		int port)
{
	char portAsStr[6]={0};
	std::string localAddress=_localAddress;
	assert(dataSink);
	StopServer();
	m_dataSink = dataSink;
	snprintf(portAsStr,ARRAYSIZE(portAsStr),"%d",port);
	if (_localAddress=="localhost"||_localAddress=="127.0.0.1") {
		localAddress = "0.0.0.0";
	}
	printf("Starting RPC server on %s:%s\n",localAddress.c_str(),portAsStr);
	m_rpc_server.reset(new http::server::http_server(m_io_service_rpc,
			localAddress, portAsStr));
	m_rpc_server->add_uri_handler("/jsonrpc",
			boost::bind(&SdJsonRpcTransport::JsonRequestHandler, this, _1, _2, _3));

	m_io_service_rpc.run();
	return true;
}

void SdJsonRpcTransport::StopServer() {
	if (m_rpc_server) {
		m_rpc_server->stop();
	}
}

bool SdJsonRpcTransport::SendData(
		const std::string& server,
		int port,
		const std::string& data,
		std::string& receivedData)
{
	char portAsStr[6]={0};
	boost::system::error_code ec;

	snprintf(portAsStr,ARRAYSIZE(portAsStr),"%d",port);
	http::client::http_client httpcl(server, portAsStr, 5000);
	if (0 == ec.value()) {
		http::client::response rep;
		http::headers headers;
		headers.header("Content-Type","application/json-rpc");
		httpcl.request(rep,"POST","/jsonrpc",data,headers,ec);
		if (0 == ec.value()) {
			receivedData = rep.content;
		} else {
			printf("Failed to send request, err %d %s\n",
					ec.value(),ec.message().c_str());
		}
	} else {
		printf("Failed to connect to %s:%d, err %d %s\n",
				server.c_str(),port,
				ec.value(),ec.message().c_str());
	}
	return !ec.value();
}

void SdJsonRpcTransport::JsonRequestHandler(
	http::server::connection& connection,
	const http::server::request& req,
	http::server::reply& rep)
{
	rep.reset();
	//if (req.headers.header("Content-Type") == "application/json-rpc" &&
	if (req.method == "POST" || req.method == "GET") {
		int err = m_dataSink->ReceiveData(req.content,rep.content);
		switch (err) {
		case 0:
			rep.status = http::server::reply::ok;
			break;
		case EINVAL:
			rep.status = http::server::reply::bad_request;
			break;
		case ENOMEM:
		default:
			rep.status = http::server::reply::internal_server_error;
			break;
		}
	} else {
		rep.status = http::server::reply::not_implemented;
	}
	rep.headers.header("Content-Type", "application/json-rpc");
}
