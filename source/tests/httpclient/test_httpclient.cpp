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
