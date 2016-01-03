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
#include <stdio.h>
#include <stdlib.h>
#include "d3math.h"

using namespace std;


int main(int argc, char *argv[])
{
	Matrix4f M4 {{2,  3,  1,  5},
				{6, 13,  5, 19},
				{2, 19, 10, 23},
				{4, 10, 11, 31}};

	Matrix3f M3 {2,  3,  1,
				 6, 13,  5,
				 2, 19, 10,
				 };

	Matrix3f I3 = Matrix3f::identity();

//	Vector4f V4 = {{1,},  {2,},  {3,},  {4,},};
	Vector4f V4 {1, 2, 3, 4};
	Vector1f V1 {1};

	std::cout << M4 << std::endl;
	std::cout << V4 << std::endl;
	std::cout << V1 << std::endl;
	std::cout << M3 << std::endl;
	std::cout << I3 << std::endl;

	I3.clear();
	std::cout << I3 << std::endl;

	return 0;
}
