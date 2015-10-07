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
#include <string>
#include "libhttp/http_client.hpp"
#include "libhttp/base64.hpp"

/*
 * Example:
 * ./test_httpclient www.google.com /
 */
int main(int argc, char *argv[])
{
	size_t col;
	boost::system::error_code ec;
	std::string server, url, address, port = "80";

	if (argc <= 2) {
		std::cout << "Usage: " << std::endl;
		std::cout << "    " << std::string(argv[0]) << " <server:port> <url>" << std::endl;
		return 1;
	}
	address = server = argv[1];
	url = argv[2];
	if ((col = server.find(':')) != std::string::npos) {
		address = server.substr(0, col);
		port = server.substr(col + 1);
	}

	http::client::http_client client(address, port, 5000);
	if (ec) {
		std::cout << "Error Code: " << ec.value() << ", " << ec.message() << std::endl;
		return 1;
	}
	http::client::response response;
	client.request(response, "GET", url, ec);
	if (ec) {
		std::cout << "Error Code: " << ec.value() << ", " << ec.message() << std::endl;
		return 1;
	}
	std::cout << response.content << std::endl;
	return 0;
}
