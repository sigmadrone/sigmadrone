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

#include "airflowforce.h"

void print_delta_v(const AirflowForce<float>& prop, float Vout, float dF)
{
	std::cout << "(" << prop.nvelocity_delta(-dF, Vout) << ", " << prop.nvelocity_delta(dF, Vout) << ")" << std::endl;
}

int main()
{
	AirflowForce<float> prop24(0.24, 1.77);
	std::cout << "It works" << std::endl;

//	print_delta_v(prop24, 0.2, 0.035);

	std::cout << prop24.force(2) << ", " << prop24.force(4) << ", " << prop24.force(6) << ", " << prop24.force(8) << ", " << prop24.force(10) << ", " << std::endl;
	std::cout << prop24.nforce(0.2) << ", " << prop24.nforce(0.4) << ", " << prop24.nforce(0.6) << ", " << prop24.nforce(0.8) << ", " << prop24.nforce(1.0) << ", " << std::endl;
	std::cout << prop24.velocity_delta(0.1, 2) << ", " << prop24.velocity_delta(0.1, 4) << ", " << prop24.velocity_delta(0.1, 6) << ", " << prop24.velocity_delta(0.1, 8) << std::endl;
	std::cout << prop24.nvelocity_delta(0.1, .2) << ", " << prop24.nvelocity_delta(0.1, .4) << ", " << prop24.nvelocity_delta(0.1, .6) << ", " << prop24.nvelocity_delta(0.1, .8) << std::endl;
	std::cout << prop24.nvelocity_delta(-0.1, .0) << std::endl;

	return 0;
}
