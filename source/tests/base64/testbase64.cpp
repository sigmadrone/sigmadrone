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
