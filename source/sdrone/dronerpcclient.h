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
