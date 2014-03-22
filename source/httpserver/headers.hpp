//
// header.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_HEADER_HPP
#define HTTP_HEADER_HPP

#include <string>
#include <cstdlib>
#include <map>

namespace http {
namespace server {


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

	std::string operator[](const std::string& name) const
	{
		return header(name);
	}

};

} // namespace server
} // namespace http

#endif // HTTP_HEADER_HPP
