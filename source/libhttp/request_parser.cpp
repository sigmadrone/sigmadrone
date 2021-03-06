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
#include "request_parser.hpp"
#include "request.hpp"

namespace http {
namespace server {

request_parser::request_parser()
	: state_(method_start)
{
}

void request_parser::reset()
{
	state_ = method_start;
	last_header_name_.clear();
	last_header_value_.clear();
}

boost::tribool request_parser::consume(request& req, char input)
{
	switch (state_) {
	case method_start:
		if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
			return false;
		} else {
			state_ = method;
			req.method.push_back(input);
			return boost::indeterminate;
		}
	case method:
		if (input == ' ') {
			state_ = uri;
			return boost::indeterminate;
		} else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
			return false;
		} else {
			req.method.push_back(input);
			return boost::indeterminate;
		}
	case uri:
		if (input == ' ') {
			state_ = http_version_h;
			return boost::indeterminate;
		} else if (is_ctl(input)) {
			return false;
		} else {
			req.uri.push_back(input);
			return boost::indeterminate;
		}
	case http_version_h:
		if (input == 'H') {
			state_ = http_version_t_1;
			return boost::indeterminate;
		} else {
			return false;
		}
	case http_version_t_1:
		if (input == 'T') {
			state_ = http_version_t_2;
			return boost::indeterminate;
		} else {
			return false;
		}
	case http_version_t_2:
		if (input == 'T') {
			state_ = http_version_p;
			return boost::indeterminate;
		} else {
			return false;
		}
	case http_version_p:
		if (input == 'P') {
			state_ = http_version_slash;
			return boost::indeterminate;
		} else {
			return false;
		}
	case http_version_slash:
		if (input == '/') {
			req.http_version_major = 0;
			req.http_version_minor = 0;
			state_ = http_version_major_start;
			return boost::indeterminate;
		} else {
			return false;
		}
	case http_version_major_start:
		if (is_digit(input)) {
			req.http_version_major = req.http_version_major * 10 + input - '0';
			state_ = http_version_major;
			return boost::indeterminate;
		} else {
			return false;
		}
	case http_version_major:
		if (input == '.') {
			state_ = http_version_minor_start;
			return boost::indeterminate;
		} else if (is_digit(input)) {
			req.http_version_major = req.http_version_major * 10 + input - '0';
			return boost::indeterminate;
		} else {
			return false;
		}
	case http_version_minor_start:
		if (is_digit(input)) {
			req.http_version_minor = req.http_version_minor * 10 + input - '0';
			state_ = http_version_minor;
			return boost::indeterminate;
		} else {
			return false;
		}
	case http_version_minor:
		if (input == '\r') {
			state_ = expecting_newline_1;
			return boost::indeterminate;
		} else if (is_digit(input)) {
			req.http_version_minor = req.http_version_minor * 10 + input - '0';
			return boost::indeterminate;
		} else {
			return false;
		}
	case expecting_newline_1:
		if (input == '\n') {
			state_ = header_line_start;
			return boost::indeterminate;
		} else {
			return false;
		}
	case header_line_start:
		if (input == '\r') {
			state_ = expecting_newline_3;
			return boost::indeterminate;
		} else if (!req.headers.empty() && (input == ' ' || input == '\t')) {
			state_ = header_lws;
			return boost::indeterminate;
		} else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
			return false;
		} else {
			last_header_name_.clear();
			last_header_value_.clear();
			last_header_name_.push_back(input);
			state_ = header_name;
			return boost::indeterminate;
		}
	case header_lws:
		if (input == '\r') {
			state_ = expecting_newline_2;
			return boost::indeterminate;
		} else if (input == ' ' || input == '\t') {
			return boost::indeterminate;
		} else if (is_ctl(input)) {
			return false;
		} else {
			state_ = header_value;
			last_header_value_.push_back(input);
			return boost::indeterminate;
		}
	case header_name:
		if (input == ':') {
			state_ = space_before_header_value;
			return boost::indeterminate;
		} else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
			return false;
		} else {
			last_header_name_.push_back(input);
			return boost::indeterminate;
		}
	case space_before_header_value:
		if (input == ' ') {
			state_ = header_value;
			return boost::indeterminate;
		} else {
			return false;
		}
	case header_value:
		if (input == '\r') {
			state_ = expecting_newline_2;
			req.headers.insert_header(last_header_name_, last_header_value_);
			return boost::indeterminate;
		} else if (is_ctl(input)) {
			return false;
		} else {
			last_header_value_.push_back(input);
			return boost::indeterminate;
		}
	case expecting_newline_2:
		if (input == '\n') {
			state_ = header_line_start;
			return boost::indeterminate;
		} else {
			return false;
		}
	case expecting_newline_3:
		if (input == '\n') {
			state_ = done;
			return true;
		}
		return false;
	default:
		return false;
	}
}

bool request_parser::is_char(int c)
{
	return c >= 0 && c <= 127;
}

bool request_parser::is_ctl(int c)
{
	return (c >= 0 && c <= 31) || (c == 127);
}

bool request_parser::is_tspecial(int c)
{
	switch (c)
	{
	case '(': case ')': case '<': case '>': case '@':
	case ',': case ';': case ':': case '\\': case '"':
	case '/': case '[': case ']': case '?': case '=':
	case '{': case '}': case ' ': case '\t':
		return true;
	default:
		return false;
	}
}

bool request_parser::is_digit(int c)
{
  return c >= '0' && c <= '9';
}

} // namespace server
} // namespace http
