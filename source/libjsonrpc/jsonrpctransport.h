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

#ifndef JSONRPCTRANSPORT_H_
#define JSONRPCTRANSPORT_H_

#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <assert.h>
#include <errno.h>
#include <string>

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
