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
	Matrix4f A (2,  3,  1,  5,
			    6, 13,  5, 19,
			    2, 19, 10, 23,
			    4, 10, 11, 31);

	Matrix3f remap(	0, -1, 0,
					1, 0, 0,
					0, 0, 1);

	MatrixMN<float, 4, 4> B( 2,  0,  2, 0.6,
				 	 	 	 3,  3,  4,  -2,
							 5,  5,  4,   2,
							-1, -2,3.4,  -1);
	Vector3f C(3, 7, 8);
	Matrix3f D(  1,  2,  0,
				 3,  4,  4,
				 5,  6,  3);

	cout << "A is:\n" << A << endl;
	cout << "B is:\n" << B << endl;
	cout << "A * B is:\n" << A * B << endl;
	cout << "A * 2 is:\n" << A * 2.0f << endl;
	cout << "A + 2 is:\n" << A + 2.0f << endl;
	cout << "A - 2 is:\n" << A - 2.0f << endl;
	cout << "A / 2 is:\n" << A / 2.0f << endl;
	cout << "D is:\n" << D << endl;
	cout << "C is:\n" << C << endl;
	cout << "D * C is:\n" << D * C << endl;
	cout << "D.inverse() is:\n" << D.inverse() << endl;
	cout << "D * D.inverse() is:\n" << D * D.inverse() << endl;
	cout << "D.transpose() is:\n" << D.transpose() << endl;
	cout << "C.length() is: \n" << C.length() << endl << endl;
	cout << "C.normalize() is: \n" << C.normalize() << endl;
	cout << "C.normalize().transpose() is: \n" << C.normalize().transpose() << endl;
	cout << "C x C.normalize() is: \n" << Vector3f::cross(C, C.normalize()) << endl;
	cout << "C dot C.normalize() is: \n" << Vector3f::dot(C, C.normalize()) << endl;
	cout << "Re-mapped C is:\n" << remap * C<< endl;
	return 0;
}
