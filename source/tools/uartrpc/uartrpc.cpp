#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "librexjsonrpc/rpcclientuart.h"

int main(int argc, const char *argv[])
{
	try {
//		rpc_client_uart rpc("/dev/ttymxc0", B921600, 100000);
		rpc_client_uart rpc("/dev/ttymxc0", B460800, 100000);
		rexjson::value ret = rpc.call("sd_get_attitude");
		std::cout << ret.write(false) << std::endl;
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
