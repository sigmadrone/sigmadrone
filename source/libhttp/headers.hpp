#ifndef HTTP_HEADERS_HPP
#define HTTP_HEADERS_HPP

#include <string>
#include <cstdlib>
#include <map>

namespace http {

/*
 * This should probably go into separate header file
 */
//struct remote_connection_info
//{
//	std::string address_;
//	std::string port_;
//	remote_connection_info(const std::string& address, const std::string& port) : address_(address), port_(port) { }
//};


class headers : public std::map<std::string, std::string>
{
public:
	std::string header(const std::string& name) const
	{
		std::string ret;
		headers::const_iterator it = find(name);
		if (it != end())
			ret = it->second;
		return ret;
	}

	void header(const std::string& name, const std::string& value)
	{
		std::pair<headers::iterator,bool> ret;
		ret = insert(headers::value_type(name, value));
		if (ret.second == false) {
			headers::iterator it = ret.first;
			it->second = value;
		}
	}

	size_t content_length()
	{
		std::string content_length = header("Content-Length");
		if (!content_length.empty())
			return std::strtoul(content_length.c_str(), NULL, 10);
		return 0UL;
	}
};

} // namespace http

#endif // HTTP_HEADER_HPP
