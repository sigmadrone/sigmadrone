/*
 * rcvalueconverter.h
 *
 *  Created on: Jul 5, 2015
 *      Author: svassilev
 */

#ifndef RCVALUECONVERTER_H_
#define RCVALUECONVERTER_H_

#include "rcchannelmapper.h"
#include "matrix.h"
#include "rcreceiver.h"

struct Throttle {
	static const float MIN_VALUE;// = 0.0;
	static const float MAX_VALUE;// = 1.0;
	Throttle(float throttle = 0.0) : throttle_(throttle) {}
	float get(float min = MIN_VALUE, float max = MAX_VALUE) {
		return (throttle_ < min ? min : (throttle_ > max ? max : throttle_));
	}
	float get_unbound() { return throttle_; }
private:
	float throttle_;
};

class RcValueConverter {
public:
	RcValueConverter(const RcChannelMapper& mapper, const RcReceiver& receiver);
	QuaternionD target_quaternion();
	Throttle base_throttle();
private:
	QuaternionD quaternion_;
	Throttle throttle_;
};

#endif /* RCVALUECONVERTER_H_ */
