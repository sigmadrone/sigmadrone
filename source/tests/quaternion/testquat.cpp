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

#define VMATH_NAMESPACE vmath
//#include "vmath.h"
using namespace std;

void printQ(float X, float Y, float Z)
{
	cout << X << ", "<< Y << ", "<< Z << ": " << Quaternion<float>::fromEulerAngles(X, Y, Z) << endl;
}

void test_quaternion_decompostion_twist_swing()
{
	QuaternionF swing, twist;
	QuaternionF swing_z = QuaternionF::fromAxisRot(Vector3f(1, 0, 0), M_PI/2).normalize();
	QuaternionF twist_z = QuaternionF::fromAxisRot(Vector3f(0, 0, 1), M_PI/4).normalize();
	QuaternionF::decomposeTwistSwing(swing_z * twist_z, Vector3f(0,0,1), swing, twist);

	cout << "swing_z:             " << swing_z << std::endl;
	cout << "twist_z:             " << twist_z << std::endl;
	cout << "swing_z * twist_z:   " << swing_z * twist_z << std::endl;
	cout << "twist:               " << twist.normalize() << std::endl;
	cout << "swing:               " << swing.normalize() << std::endl;
	cout << "swing * twist:       " << swing.normalize() * twist.normalize() << std::endl;
	cout << std::endl;
}


void test_quaternion_decompostion_swing_twist()
{
	QuaternionF swing, twist;
	QuaternionF swing_z = QuaternionF::fromAxisRot(Vector3f(1, 0, 0), M_PI).normalize();
	QuaternionF twist_z = QuaternionF::fromAxisRot(Vector3f(0, 0, 1), M_PI).normalize();
	QuaternionF::decomposeSwingTwist(twist_z * swing_z, Vector3f(0,0,1), swing, twist);

	cout << "swing_z:             " << swing_z << std::endl;
	cout << "twist_z:             " << twist_z << std::endl;
	cout << "twist_z * swing_z:   " << twist_z * swing_z << std::endl;
	cout << "twist:               " << twist.normalize() << std::endl;
	cout << "swing:               " << swing.normalize() << std::endl;
	cout << "twist * swing:       " << twist.normalize() * swing.normalize() << std::endl;
	cout << std::endl;
}


void test_quaternion_transforms()
{
	cout << "Quaternion Transforms from: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/transforms/" << std::endl << std::endl;
	Vector3f N(0,0,1);
	Vector3f Vin(2, 3, 4);
	QuaternionF q = QuaternionF(N);
	QuaternionF Pin = QuaternionF(Vin);
	cout << "q:                                   " << q << std::endl;
	cout << "Pin:                                 " << Pin << std::endl;

	/*
	 * The plane is defined by its normal vector represented as Quaternion 'q'
	 */
	cout << "Parallel component of plane       :  " << (Pin + q * Pin * q) * 1.0f / 2.0f << std::endl;
	cout << "Perpendicular component of plane  :  " << (Pin - q * Pin * q) * 1.0f / 2.0f << std::endl;
	cout << "Reflection in plane               :  " << (q * Pin * q) << std::endl << std::endl;

	cout << "(V) Parallel component of plane       :  " << N.parallel(Vin).transpose();
	cout << "(V) Perpendicular component of plane  :  " << N.perpendicular(Vin).transpose();
	cout << "(V) Reflection in plane               :  " << N.reflection(Vin).transpose();

	cout << std::endl;
	cout << "(V) Projection to vector              :  " << N.projection(Vin).transpose();
	cout << "(V) Perpendicular to vector           :  " << N.perpendicular(Vin).transpose();

	std::cout << std::endl;
}

void test_quaternion_nlerp()
{
	cout << "\nQuaternion nlerp test\n" << std::endl;

	QuaternionF q1 = QuaternionF::identity;
	QuaternionF q2 = QuaternionF::fromAxisRot(Vector3f(0,0,1),M_PI/2);
//	QuaternionF dq = slerp(q1, q2, 1.0);

	cout << "q1:                 " << q1 << std::endl;
	cout << "q2:                 " << q2 << std::endl << std::endl;

	cout << "slerp(q1,q2,0.33):  " << QuaternionF::slerp(q1,q2,0.33) << std::endl;
	cout << "slerp(q1,q2,0.66):  " << QuaternionF::slerp(q1,q2,0.66) << std::endl;
	cout << "slerp(q1,q2,1.0):   " << QuaternionF::slerp(q1,q2,1.0) << std::endl << std::endl;

	cout << "nlerp(q1,q2,0.33):  " << QuaternionF::nlerp(q1,q2,0.33) << std::endl;
	cout << "nlerp(q1,q2,0.66):  " << QuaternionF::nlerp(q1,q2,0.66) << std::endl;
	cout << "nlerp(q1,q2,1.0):   " << QuaternionF::nlerp(q1,q2,1.0) << std::endl << std::endl;

}

void test_delta_quaternion()
{
	Vector3f v(1.0, 0.0, 0.0);
	QuaternionF q1 = QuaternionF::fromAxisRot(Vector3f(0,0,1), M_PI/3);
	QuaternionF q2 = QuaternionF::fromAxisRot(Vector3f(0,0,1), M_PI/2);
	QuaternionF dQ;

	/*
	 * Example using absolute frame
	 * What is the dQ between q1 and q2. So that q2 = dQ * q1;
	 * q2 * (~q1) = dQ * q1 * (~q1);
	 * q2 * (~q1) = dQ
	 */
	std::cout << "q1:                 " << q1 << std::endl;
	std::cout << "q2:                 " << q2 << std::endl;
	dQ = q2 * (~q1);
	std::cout << "dQ:                 " << dQ << std::endl;
	std::cout << "q2 = dQ * q1:       " << dQ * q1 << std::endl;

	/*
	 * Example using frame of reference of rotating object
	 * What is the dQ between q1 and q2. So that q2 = q1 * dQ
	 * q2 = q1 * dQ
	 * (~q1) * q2 = (~q1) * q1 * dQ;
	 * (~q1) * q2 = dQ;
	 */
	std::cout << "q1:                 " << q1 << std::endl;
	std::cout << "q2:                 " << q2 << std::endl;
	dQ = (~q1) * q2;
	std::cout << "dQ:                 " << dQ << std::endl;
	std::cout << "q2 = q1 * dQ:       " << q1 * dQ << std::endl;

	std::cout << "q2 = q1 * dQ(W):    " << q1
			* QuaternionF::fromAngularVelocity(Vector3f(0,0,M_PI/6), 1.0/4.0)
			* QuaternionF::fromAngularVelocity(Vector3f(0,0,M_PI/6), 1.0/2.0)
			* QuaternionF::fromAngularVelocity(Vector3f(0,0,M_PI/6), 1.0/4.0)
			<< std::endl;

}


int main(int argc, char *argv[])
{
	Vector3f Xi(1.00,    0.00,    0.00);
	Vector3f Yi(0.00,    1.00,    0.00);
	Vector3f Zi(0.00,    0.00,    1.00);

	Vector3f X(0.97,   -0.19,   -0.16);
	Vector3f Y(0.08,    0.85,   -0.52);
	Vector3f Z(0.23,    0.49,    0.84);
	Vector3f Gc(-0.16,   -0.52,    0.84);
	Vector3f Gn(-0.02,   -0.01,    1.00);
	Vector3f Gi(-0.02,   -0.01,    1.00);
	Quaternion<float> Q(0.96, -0.26, 0.10, -0.07);

	for (int i = 0; i <= 24; i++) {
		double angle = 2 * M_PI * i /24.0;
		std::cout << "Rotation around Z, angle: " << RAD2DEG(angle) << ", Q: " << QuaternionF::fromAxisRot(Vector3f(0, 0, 1), angle).normalize() << std::endl;
	}
	std::cout << std::endl << std::endl;

	Gn = Gn.normalize();

	cout << "X: " << X.transpose();
	cout << "Y: " << Y.transpose();
	cout << "Z: " << Z.transpose();

	cout << endl;

	cout << "X: " << Q.rotate(Xi).transpose();
	cout << "Y: " << Q.rotate(Yi).transpose();
	cout << "Z: " << Q.rotate(Zi).transpose();

	cout << endl;
	cout << "Gi:               " << Gi.transpose();
	cout << "Gn:               " << Gn.transpose();
	cout << "Gc:               " << Gc.transpose();
	cout << "(~Q).roatate(Gn): " << (~Q).rotate(Gn).transpose();
	cout << "Q.roatate(Gc):    " << Q.rotate(Gc).transpose();

	cout << "Q:                " << Q << endl;
	cout << "Q':               " << Quaternion<float>::fromVectors(Gc, Gn).normalize() << endl;
	cout << "fromVectors(Gc,Gc)': " << Quaternion<float>::fromVectors(Gc, Gc).normalize() << endl;
	cout << "fromVectors(Gc,-Gc)': " << Quaternion<float>::fromVectors(Gc, -Gc).normalize() << endl;

	Quaternion<float> q = Quaternion<float>::fromVectorsPartial(Gc, Gn, 0.25).normalize();
	cout << "q:                " << q*q*q*q << endl;

	q = (q*q*q*q).normalize();

	X = (~q).rotate(X);
	Y = (~q).rotate(Y);
	Z = (~q).rotate(Z);

	cout << "X: " << X.transpose();
	cout << "Y: " << Y.transpose();
	cout << "Z: " << Z.transpose();

	cout << Quaternion<float>(1,2,3,4).ln() << endl;
	cout << Quaternion<float>(1,2,3,4).exp() << endl;

	Quaternion<float> I = Quaternion<float>::fromAxisRot(Vector3f(1.00, 0.00, 0.00), 30);
	Quaternion<float> F = Quaternion<float>::fromAxisRot(Vector3f(0.00, 1.00, 0.00), 210);

	cout << "I:      " << I << endl;
	cout << "F:      " << F << endl;
	cout << "nlerp:  " << Quaternion<float>::nlerp(I, F, 1.0) << endl;



	cout << "2*Ln(F * ~I): " << (F * ~I).ln() * 2.0f << endl;
	cout << "W(t)        : " << Quaternion<float>::angularVelocity(I, F, 1.0).transpose() << endl;


	test_quaternion_transforms();
	test_quaternion_nlerp();
	test_quaternion_decompostion_twist_swing();
	test_quaternion_decompostion_swing_twist();

	std::cout << "Test Delta" << std::endl;
	test_delta_quaternion();

	return 0;
}
