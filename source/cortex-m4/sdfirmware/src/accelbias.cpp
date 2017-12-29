/*
 * AccelBias.cpp
 *
 *  Created on: Dec 28, 2017
 *      Author: mstoilov
 */

#include "accelbias.h"
#include <iostream>

#define THRESHOLD_ANGLE_RAD (5 * M_PI / 6)

AccelBias::AccelBias()
	: initialQ_(QuaternionF::identity)
{

}

AccelBias::~AccelBias()
{
}

void AccelBias::reset(const QuaternionF& att, const Vector3f& acc)
{
	initialQ_ = att;
	acc_ = acc;
}

Vector3f AccelBias::detect(const QuaternionF& att, const Vector3f& acc)
{
	Vector3f bias;
	QuaternionF dQ = initialQ_.conjugate() * att;

	if (dQ.angle() > THRESHOLD_ANGLE_RAD) {
		bias = (acc + acc_) / 2.0;
	}
	bias[2] = 0;
	return bias;
}

