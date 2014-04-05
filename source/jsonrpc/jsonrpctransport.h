/*
 * jsonrpctransport.h
 *
 *  Created on: Mar 25, 2014
 *      Author: svassilev
 */

#ifndef JSONRPCTRANSPORT_H_
#define JSONRPCTRANSPORT_H_

#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <assert.h>
#include <errno.h>
#include <string>

struct SdRpcEndpoint
{
	std::string Host;
	int port;
};

struct IRpcReceiveDataSink
{
	virtual int ReceiveData(
		const std::string& request,
		std::string& reply) = 0;

protected:
	virtual ~IRpcReceiveDataSink() {}
};

struct IRpcTransport
{
	virtual bool StartServer(
			IRpcReceiveDataSink* dataSink,
			const std::string& localAddress,
			int port
			) = 0;
	virtual void StopServer(
			) = 0;
	virtual bool SendData(
			const std::string& remoteHost,
			int port,
			const std::string& dataIn,
			std::string& dataOut
			) = 0;

	virtual ~IRpcTransport() {}

	enum TransportType
	{
		TRANSPORT_TCP,
		TRANSPORT_HTTP
	};

	static IRpcTransport* Create(TransportType type);
};

#endif /* JSONRPCTRANSPORT_H_ */
