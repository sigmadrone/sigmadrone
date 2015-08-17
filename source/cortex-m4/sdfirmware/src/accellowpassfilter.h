/*
 * accellowpassfilter.h
 *
 *  Created on: Aug 15, 2015
 *      Author: svassilev
 */

#ifndef ACCELLOWPASSFILTER_H_
#define ACCELLOWPASSFILTER_H_

#include "firfilt.h"

struct AccelLowPassFilter {
	AccelLowPassFilter() : filter_() {
		static const float oneHzCutoff_18[] = {
				0.0086, 0.0120, 0.0216, 0.0362, 0.0537, 0.0719, 0.0883, 0.1006, 0.1072, 0.1072,
				0.1006, 0.0883, 0.0719, 0.0537, 0.0362, 0.0216, 0.0120, 0.0086
		};
		filter_.init_coeff(oneHzCutoff_18);
	}
	~AccelLowPassFilter() {}
	Vector3f do_filter(const Vector3f accel) {
		return filter_.do_filter(accel);
	}
private:
	FirFilter<float, 18, 3> filter_;
};

#endif /* ACCELLOWPASSFILTER_H_ */
