/*
 * dronestate.h
 *
 *  Created on: Jul 18, 2015
 *      Author: svassilev
 */

#ifndef DRONESTATE_H_
#define DRONESTATE_H_

#include "units.h"
#include "matrix.h"

struct DroneState {
	/*
	 * Sensors - input attributes
	 */
	Vector3f gyro_raw_;
	Vector3f accel_raw_;
	Vector3f mag_raw_;
	Vector3f gyro_;
	Vector3f accel_;
	Vector3f mag_;
	/*more to add here*/

	/*
	 * Time it took to read sensors
	 */
	TimeSpan dt_;

	/*
	 * Calculated state attributes
	 */
	QuaternionF attitude_;
	Vector4f motors_;
};


#endif /* DRONESTATE_H_ */
