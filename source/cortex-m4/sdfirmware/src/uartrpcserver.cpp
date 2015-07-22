/*
 * uartrpcserver.cpp
 *
 *  Created on: Jul 21, 2015
 *      Author: mkstoilo
 */

#include "uartrpcserver.h"

UartRpcServer::UartRpcServer() {
	// TODO Auto-generated constructor stub

}

UartRpcServer::~UartRpcServer() {
	// TODO Auto-generated destructor stub
}

void UartRpcServer::jsonrpc_request_handler(UART* uart)
{
	rexjson::value jsonreq, result;
	std::string response;
	std::string request = uart->readline();

	if (request.empty())
		return;
	jsonreq.read(request);
	result = call(uart, jsonreq);
	uart->transmit(result.write(false, false, 0, 8) + "\n");
}
