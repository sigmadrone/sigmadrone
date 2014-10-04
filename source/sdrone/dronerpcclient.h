/*
 * dronerpcclient.h
 *
 *  Created on: Aug 17, 2014
 *      Author: svassilev
 */

#ifndef DRONERPCCLIENT_H_
#define DRONERPCCLIENT_H_

#include "commandargs.h"
#include "jsonrpcdispatch.h"
#include <boost/scoped_ptr.hpp>

class SdJsonValue;
class DroneRpcClient {
public:
	DroneRpcClient();
	~DroneRpcClient();
	int /*error*/ ExecuteCommand(const CommandLineArgs& cmdArgs);
private:
	bool FillRequestParamsFromJsonSpec(
			const CommandLineArgs& cmdArgs,
			SdJsonValue& rpcParams);
	bool FillRequestParamsFromJsonStream(
			const CommandLineArgs& cmdArgs,
			SdJsonValue& rpcParams);

	bool /*error*/ MakeRpcCall(
			const std::string& serverAddress,
			int serverPort,
			const SdJsonRpcRequest& req,
			SdJsonRpcReply& rep);
private:
	boost::scoped_ptr<IRpcTransport> m_rpc_transport;
};

#endif /* DRONERPCCLIENT_H_ */
