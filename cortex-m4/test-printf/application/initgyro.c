/*
 * initgyro.c
 *
 *  Created on: Dec 23, 2014
 *      Author: mstoilov
 */

#include "initgyro.h"
#include "stm32f429i_discovery_gyroscope.h"

void init_gyro()
{
	BSP_GYRO_Init();
}
