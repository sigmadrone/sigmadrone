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
#include "propeller.h"


int main(int argc, char *argv[])
{
	std::vector<Propeller> propellers;
	Vector3f thrust_dir(0, 0, -1);
	propellers.push_back(Propeller(Vector3f(-1, -1, -1), thrust_dir, Propeller::CW));
	propellers.push_back(Propeller(Vector3f(-1,  1,  1), thrust_dir, Propeller::CCW));
	propellers.push_back(Propeller(Vector3f( 1,  1, -1), thrust_dir, Propeller::CW));
	propellers.push_back(Propeller(Vector3f( 1, -1,  1), thrust_dir, Propeller::CCW));

	for (const auto& p : propellers)
		std::cout << p.torque_dir().transpose() << std::endl;
	return 0;
}
