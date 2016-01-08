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
	Matrix4f M4{{2,  3,  1,  5},
				{6, 13,  5, 19},
				{2, 19, 10, 23},
				{4, 10, 11, 31}};

	Matrix4f N4{{4, 10, 11, 31},
				{1,  3,  1,  3},
				{2, 19, 9, 23},
				{7, 10,  5, 19}};

	Matrix4f P4;

	Matrix3f M3 (2,  3,  1,
				 6, 13,  5,
				 2, 19, 10
				 );


	Matrix4f A (2,  3,  1,  5,
			    6, 13,  5, 19,
			    2, 19, 10, 23,
			    4, 10, 11, 31);

	Matrix4f B ( 2,  0,  2, 0.6,
				 3,  3,  4,  -2,
				 5,  5,  4,   2,
				-1, -2,3.4,  -1);

	P4 = M4;
	P4 *= N4;
	std::cout << "P4: \n" << P4 << std::endl;
	P4 = M4 * N4;
	std::cout << "P4: \n" << P4 << std::endl;

	Vector3f C(3, 7, 8);
	Matrix3f D(  1,  2,  0,
				 3,  4,  4,
				 5,  6,  3);

	D[0][2] = 3;
	std::cout << "D[1][1]: " << D[1][1] << std::endl;

	std::cout << "A :\n" << A << std::endl;
	std::cout << "B :\n" << B << std::endl;
	std::cout << "A * B is:\n" << A * B << std::endl;

	std::cout << "D is:\n" << D << std::endl;
	std::cout << "C is:\n" << C << std::endl;
	std::cout << "D * C is:\n" << D * C << std::endl;
	std::cout << "D.inverse() is:\n" << D.inverse() << std::endl;
	std::cout << "D * D.inverse() is:\n" << D * D.inverse() << std::endl;

	std::cout << "D.transpose() is:\n" << D.transpose() << std::endl;
	std::cout << "C.length() is: \n" << C.length() << std::endl << std::endl;
	std::cout << "C.length() is: \n" << C.length() << std::endl << std::endl;
	std::cout << "C.normalize() is: \n" << C.normalize() << std::endl;
	std::cout << "C.normalize().transpose() is: \n" << C.normalize().transpose() << std::endl;
	std::cout << "C x C.normalize() is: \n" << Vector3f::cross(C, C.normalize()) << endl;
	cout << "C dot C.normalize() is: \n" << Vector3f::dot(C, C.normalize()) << endl;

	Matrix3f I3 = Matrix3f::identity();

	std::cout << "Identity: \n" << MatrixMN<float, 4, 4>::identity() << std::endl;

	Vector4f V4 {1, 2, 3, 4}, U4;
	Vector1f V1 {1};

	U4 = V4;
	std::cout << "M4: \n" << M4 << std::endl;
	std::cout << "V4: \n" << V4 << std::endl;
	std::cout << "U4: \n" << U4 << std::endl;
	std::cout << "V4.length(): " << V4.length() << std::endl << std::endl;
	std::cout << "V4.x: " << V4.x() << std::endl;
	std::cout << "V4.y: " << V4.y() << std::endl;
	std::cout << "V4.z: " << V4.z() << std::endl;
	std::cout << "V4.w: " << V4.w() << std::endl;
	std::cout << "V4.at(3): " << V4.at(3) << std::endl;

	std::cout << "V1: \n" << V1 << std::endl;
	std::cout << "M3: \n" << M3 << std::endl;
	std::cout << "Identity I3: \n" << I3 << std::endl;

	I3.clear();
	std::cout << "I3.clear(): \n" << I3 << std::endl;

	std::cout << "M3.row(1): \n" << M3.row(1) << std::endl;
	std::cout << "M3.column(1)\n" << M3.column(1) << std::endl;

	M3.set_row(0, M3.row(1));
	std::cout << "M3.set_row(0, M3.row(1)):\n" << M3 << std::endl;

	M3.set_column(0, M3.column(1));
	std::cout << "M3.set_column(0, M3.column(1)):\n" << M3 << std::endl;

	M3.swap_rows(1, 2);
	std::cout << "M3.swap_rows(1, 2):\n" << M3 << std::endl;

	std::cout << "M3+=3:\n" << (M3+=3) << std::endl;
	std::cout << "M3-=3:\n" << (M3-=3.0f) << std::endl;
	std::cout << "M3*=3:\n" << (M3*=3) << std::endl;
	std::cout << "M3/=3:\n" << (M3/=3) << std::endl;
	std::cout << "M3=3:\n" << (M3=3) << std::endl;

	std::cout << "M3+=M3:\n" << (M3+=M3) << std::endl;
	std::cout << "M3-=Matrix3f::identity():\n" << (M3-=Matrix3f::identity()) << std::endl;


	std::cout << "V4=5: \n" << (V4 = 5) << std::endl;
	std::cout << "V4 - =Vector4f({1,2,3,4}): \n" << (V4 -= Vector4f(1,2,3,4)) << std::endl;

	std::cout << "V4.transpose(): \n" << V4.transpose() << std::endl;

	std::cout << "D.inverse() is:\n" << D.inverse() << std::endl;

	std::cout << "D.inverse().min(): \n" << D.inverse().min() << std::endl;
	std::cout << "D.inverse().max(): \n" << D.inverse().max() << std::endl;
	std::cout << "D.sum(): \n" << D.sum() << std::endl;

	std::cout << "Vector3f::cross(Vector3f(1,0,0), Vector3f(0,1,0)): \n" << Vector3f::cross(Vector3f(1,0,0), Vector3f(0,1,0)) << std::endl;
	std::cout << "Vector3f(1,0,0).cross(Vector3f(0,1,0)): \n" << Vector3f(1,0,0).cross(Vector3f(0,1,0)) << std::endl;

	std::cout << "Vector3f::dot(Vector3f(1,0,0), Vector3f(0,1,0)): \n" << Vector3f::dot(Vector3f(1,0,0), Vector3f(0,1,0)) << std::endl;
	std::cout << "Vector3f(1,2,3).dot(Vector3f(0,1,0)): \n" << Vector3f(1,2,2).dot(Vector3f(0,1,0)) << std::endl;

	std::cout << "Vector3f(1,2,3)[0]: " << Vector3f(1,2,3)[0] << std::endl;
	(Vector3f(3,3,3) - Vector3f(1,2,3)).normalize();

	Vector3f res = Matrix3f::identity() * (Vector3f(3,3,3) - Vector3f(2,1,0));
	std::cout << "res: \n" << res;

	std::cout << "(Matrix3f::identity() * Vector3f(3,3,3)).normalize(): \n" << (Matrix3f::identity() * Vector3f(3,3,3)).normalize();

	std::cout << "Vector3f(3,4,5) * Vector3f(3,4,5).transpose(): \n" << Vector3f(3,4,5) * Vector3f(3,4,5).transpose();

	std::cout << "Vector3f(3,4,5).transpose() * Vector3f(3,4,5): \n" << Vector3f(3,4,5).transpose() * Vector3f(3,4,5);

	std::cout << "MatrixMN<float, 2, 3>(1,2,3,4,5,6) * MatrixMN<float, 3, 2>(7,8,9,10,11,12):\n" << MatrixMN<float, 2, 3>(1,2,3,4,5,6) * MatrixMN<float, 3, 2>(7,8,9,10,11,12);
	std::cout << "(MatrixMN<float, 2, 3>(1,2,3,4,5,6) += MatrixMN<float, 2, 3>(7,8,9,10,11,12)).min():\n" << (MatrixMN<float, 2, 3>(1,2,3,4,5,6) += MatrixMN<float, 2, 3>(7,8,9,10,11,12)).min() << std::endl;
	std::cout << "(MatrixMN<float, 3, 3>(1,2,3,4,5,6,7,8,9) *= MatrixMN<float, 3, 3>(7,8,9,10,11,12,13,14,15)).min():\n" << (MatrixMN<float, 3, 3>(1,2,3,4,5,6,7,8,9) *= MatrixMN<float, 3, 3>(7,8,9,10,11,12,13,14,15)).min() << std::endl;

	std::cout << "(MatrixMN<float, 3, 1>(1,2,3) *= 7).min():\n" << (MatrixMN<float, 3, 1>(1,2,3) *= 7).min() << std::endl;

	return 0;
}
