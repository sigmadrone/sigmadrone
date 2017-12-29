/*
 * AccelBias.h
 *
 *  Created on: Dec 28, 2017
 *      Author: mstoilov
 */

#ifndef ACCELBIAS_H_
#define ACCELBIAS_H_

#include "d3math.h"
#include "lowpassfilter.h"

class AccelBias {
public:
	AccelBias();
	~AccelBias();
	void reset(const QuaternionF& att, const Vector3f& acc);
	Vector3f detect(const QuaternionF& att, const Vector3f& acc);

protected:
	QuaternionF initialQ_;
	Vector3f acc_;
};

#endif /* ACCELBIAS_H_ */
