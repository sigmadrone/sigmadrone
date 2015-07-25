#include <iostream>
#include "librexjsonrpc/rpcclientuart.h"

int main(int argc, const char *argv[])
{
	rpc_client_uart rpc("/dev/ttymxc0", B921600, 100000);

	rexjson::value ret = rpc.call("sd_get_attitude");

	std::cout << ret.write(false) << std::endl;

	return 0;
}
