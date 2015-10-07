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
#include "base64.hpp"
#include <boost/system/system_error.hpp>
#include <sstream>

namespace http {
namespace base64 {


std::string encode(const std::string& s)
{
	static const char basis64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string::const_iterator in = s.begin();
	size_t size = s.size();
	std::string ret;

	ret.reserve((size + 2) / 3 * 4);
	while (size >= 3) {
		ret += basis64[in[0] >> 2];
		ret += basis64[((in[0] << 4) & 0x30) | (in[1] >> 4)];
		ret += basis64[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
		ret += basis64[in[2] & 0x3f];
		in += 3;
		size -= 3;
	}
	if (size == 1) {
		ret += basis64[in[0] >> 2];
		ret += basis64[(in[0] << 4) & 0x30];
		ret += '=';
		ret += '=';
	} else if (size == 2) {
		ret += basis64[in[0] >> 2];
		ret += basis64[((in[0] << 4) & 0x30) | (in[1] >> 4)];
		ret += basis64[(in[1] << 2) & 0x3c];
		ret += '=';
	}
	return ret;
}

std::string decode(const std::string& s, boost::system::error_code& ec)
{
	try {
		return decode(s);
	} catch (boost::system::system_error& e) {
		ec = e.code();
	}
	return std::string("");
}

std::string decode(const std::string& s) throw (std::exception)
{
	char c;
	unsigned int i = 0;
	static const int index64[128] = {
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
			-1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
			-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
			41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1};

	std::string ret;
	ret.resize(s.size() * 3 / 4);
	std::string::iterator out = ret.begin();
	for (std::string::const_iterator in = s.begin(); in < s.end(); in++) {
		if ((i > 1 && *in == '=') || *in == '\n' || *in == '\r')
			continue;
		if ((c = index64[static_cast<int>(*in)]) == (char)-1) {
			throw boost::system::error_code(boost::system::errc::illegal_byte_sequence, boost::system::generic_category());
		}
		switch (i) {
		case 0:
			*out = c << 2;
			break;
		case 1:
			*out++ |= c >> 4;
			*out = c << 4;
			break;
		case 2:
			*out++ |= c >> 2;
			*out = c << 6;
			break;
		case 3:
			*out++ |= c;
			break;
		default:
			break;
		}
		i++;
		i = i % 4;
	}
	ret.resize(out - ret.begin());
	return ret;
}


} // namespace base64
} // namespace http
