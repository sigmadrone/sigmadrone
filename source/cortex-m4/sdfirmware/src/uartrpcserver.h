/*
 * uartrpcserver.h
 *
 *  Created on: Jul 21, 2015
 *      Author: mkstoilo
 */

#ifndef UARTRPCSERVER_H_
#define UARTRPCSERVER_H_

#include "uart.h"
#include "librexjsonrpc/rpcserver.h"

class UartRpcServer
		: public rpc_server<UartRpcServer, UART*>
{
public:
	UartRpcServer();
	virtual ~UartRpcServer();

	rexjson::value rpc_spec(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_help(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);

protected:
	void jsonrpc_request_handler(UART* uart);
};

#endif /* UARTRPCSERVER_H_ */
