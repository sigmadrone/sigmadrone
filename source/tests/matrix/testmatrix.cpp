#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

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
	cout << "A * 2 is:\n" << A * 2 << endl;
	cout << "A + 2 is:\n" << A + 2 << endl;
	cout << "A - 2 is:\n" << A - 2 << endl;
	cout << "A / 2 is:\n" << A / 2 << endl;
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
