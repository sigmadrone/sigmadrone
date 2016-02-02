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
#include <iomanip>
#include <complex>
#include <cmath>

#include "d3math.h"
#include "firfilt.h"


int main()
{
	MovingAverageFilter<Vector3f, float, 5> avgfilt;

	avgfilt.do_filter(Vector3f(1,1,1));
	avgfilt.do_filter(Vector3f(1,1,1));
	avgfilt.do_filter(Vector3f(1,1,1));
	avgfilt.do_filter(Vector3f(1,1,1));
	avgfilt.do_filter(Vector3f(1,1,1));

	avgfilt.do_filter(Vector3f(1,1,1));
	std::cout << avgfilt.output().transpose();

	avgfilt.do_filter(Vector3f(1,2,2));
	std::cout << avgfilt.output().transpose();

	avgfilt.do_filter(Vector3f(1,2,2));
	std::cout << avgfilt.output().transpose();

	avgfilt.do_filter(Vector3f(1,2,2));
	std::cout << avgfilt.output().transpose();

	avgfilt.do_filter(Vector3f(2,2,2));
	std::cout << avgfilt.output().transpose();

	avgfilt.do_filter(Vector3f(2,2,2));
	std::cout << avgfilt.output().transpose();

	avgfilt.do_filter(Vector3f(2,2,2));
	std::cout << avgfilt.output().transpose();

	avgfilt.do_filter(Vector3f(2,2,2));
	std::cout << avgfilt.output().transpose();

	avgfilt.do_filter(Vector3f(2,2,2));
	std::cout << avgfilt.output().transpose();

	return 0;
}
