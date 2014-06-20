#include <string>
#include <iostream>
#include "libhttp/base64.hpp"


int main(int argc, char *argv[])
{
	if (argc < 2)
		return 1;

	std::string encstr = http::base64::encode(argv[1]);
	std::cout << encstr << " (size = " << encstr.size() << ")"<< std::endl;
	std::string decstr = http::base64::decode(encstr);
	std::cout << decstr << " (size = " << decstr.size() << ")"<< std::endl;

	return 0;
}
