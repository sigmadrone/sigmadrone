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
