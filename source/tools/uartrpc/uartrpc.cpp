#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "librexjsonrpc/rpcclientuart.h"

int main(int argc, const char *argv[])
{
	std::string strarg;
	try {
		rpc_client_uart rpc("/dev/ttymxc0", B460800, 100000);
		if (argc > 1)
			strarg = argv[1];
		if (strarg == "pressure") {
			std::cout << rpc.call("sd_get_pressure").write(false) << std::endl;
		} else if (strarg == "temperature") {
			std::cout << rpc.call("sd_get_temperature").write(false) << std::endl;
		} else {
			std::cout << rpc.call("sd_get_attitude").write(false) << std::endl;
		}
		usleep(200000);
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
