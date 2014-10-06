/*
 * droneconfig.h
 *
 *  Created on: Aug 18, 2014
 *      Author: svassilev
 */

#ifndef DRONECONFIG_H_
#define DRONECONFIG_H_

#include "commoninc.h"

struct DroneConfig
{
	DroneConfig();
	~DroneConfig() {}
	void Init(SdDroneType droneType);
	void PrintConfig();

	SdDroneConfig m_config;
};

#endif /* DRONECONFIG_H_ */
