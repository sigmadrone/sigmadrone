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
#ifndef _CMDARGS_H_
#define _CMDARGS_H_

#include <vector>
#include <map>
#include <string>
#include <exception>
using namespace std;

enum cmd_arg_enum {
	CMD_ARG_BOOL = 0,
	CMD_ARG_STRING,
	CMD_ARG_MULTISTRING,
};

struct cmd_arg_spec
{
	std::string name;
	std::string alias;
	std::string help;
	cmd_arg_enum type;
};


class cmd_args
{
public:
	cmd_args(const std::string &argprefix = "--",
			const std::string &aliasprefix = "-",
			unsigned int helpwidth = 30,
			unsigned int aliaswidth = 5);
	~cmd_args();
	void add_specs(cmd_arg_spec *specs, unsigned int count);
	void parse_command_line(int argc, const char *argv[]) throw (std::exception);
	std::string get_value(const std::string& name, const std::string& defval = "") const;
	std::vector<std::string> get_values(const std::string &name) const;
	std::string get_help_message() const;

public:
	std::map<std::string, std::vector<std::string> > args_;

private:
	std::vector<cmd_arg_spec> specs_;
	std::string argprefix_;
	std::string aliasprefix_;
	unsigned int helpwidth_;
	unsigned int aliaswidth_;
};


#endif

