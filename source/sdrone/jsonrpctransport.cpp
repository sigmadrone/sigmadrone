/*
 * SdJsonRpcTransport.cpp
 *
 *  Created on: Mar 25, 2014
 *      Author: svassilev
 */

#include "jsonrpctransport.h"

class SdJsonRpcTransport {
public:
	SdJsonRpcTransport();
	virtual ~SdJsonRpcTransport();
};


SdJsonRpcTransport::SdJsonRpcTransport() {
	// TODO Auto-generated constructor stub

}

SdJsonRpcTransport::~SdJsonRpcTransport() {
	// TODO Auto-generated destructor stub
}

IRpcTransport* IRpcTransport::Create(IRpcTransport::TransportType)
{
	return 0;
}
