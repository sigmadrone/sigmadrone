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
#include "response_parser.hpp"
#include "response.hpp"

namespace http {
namespace client {

response_parser::response_parser()
	: state_(http_version_h)
{
}

void response_parser::reset()
{
	state_ = http_version_h;
	last_header_name_.clear();
	last_header_value_.clear();
}

boost::tribool response_parser::consume(response& resp, char input)
{
	switch (state_) {
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
			resp.http_version_major = 0;
			resp.http_version_minor = 0;
			state_ = http_version_major_start;
			return boost::indeterminate;
		} else {
			return false;
		}
	case http_version_major_start:
		if (is_digit(input)) {
			resp.http_version_major = resp.http_version_major * 10 + input - '0';
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
			resp.http_version_major = resp.http_version_major * 10 + input - '0';
			return boost::indeterminate;
		} else {
			return false;
		}
	case http_version_minor_start:
		if (is_digit(input)) {
			resp.http_version_minor = resp.http_version_minor * 10 + input - '0';
			state_ = http_version_minor;
			return boost::indeterminate;
		} else {
			return false;
		}
	case http_version_minor:
		if (input == ' ') {
			state_ = status_code_start;
			return boost::indeterminate;
		} else if (is_digit(input)) {
			resp.http_version_minor = resp.http_version_minor * 10 + input - '0';
			return boost::indeterminate;
		} else {
			return false;
		}

	case status_code_start:
		if (is_digit1to9(input)) {
			resp.status = resp.status * 10 + input - '0';
			state_ = status_code;
			return boost::indeterminate;
		} else {
			return false;
		}
	case status_code:
		if (input == ' ') {
			state_ = status_text;
			return boost::indeterminate;
		} else if (is_digit(input)) {
			resp.status = resp.status * 10 + input - '0';
			return boost::indeterminate;
		} else {
			return false;
		}
	case status_text:
		if (input == '\r') {
			state_ = expecting_newline_1;
			return boost::indeterminate;
		} else if (is_ctl(input)) {
			return false;
		} else {
			resp.status_text.push_back(input);
			return boost::indeterminate;
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
		} else if (!resp.headers.empty() && (input == ' ' || input == '\t')) {
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
			resp.headers.insert_header(last_header_name_, last_header_value_);
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

bool response_parser::is_char(int c)
{
	return c >= 0 && c <= 127;
}

bool response_parser::is_ctl(int c)
{
	return (c >= 0 && c <= 31) || (c == 127);
}

bool response_parser::is_tspecial(int c)
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

bool response_parser::is_digit(int c)
{
	return c >= '0' && c <= '9';
}

bool response_parser::is_digit1to9(int c)
{
	return c >= '1' && c <= '9';
}


} // namespace server
} // namespace http
