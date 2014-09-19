/*
 * droneconfig.cpp
 *
 *  Created on: Aug 18, 2014
 *      Author: svassilev
 */

#include "droneconfig.h"

DroneConfig::DroneConfig() : m_config() {
	Init(SD_DRONE_TYPE_QUADROTOR);
}

void DroneConfig::Init(SdDroneType droneType)
{
	if (SD_DRONE_TYPE_QUADROTOR == droneType) {
		m_config.Accel.DeviceName = "/dev/accel0";
		m_config.Accel.SamplingRate = 200;
		m_config.Accel.Scale = 4;
		m_config.Accel.MaxReading = 32768;
		m_config.Accel.Watermark = 2;
		m_config.Gyro.DeviceName = "/dev/gyro0";
		m_config.Gyro.SamplingRate = 200;
		m_config.Gyro.Scale = 2000;
		m_config.Gyro.MaxReading = 32768;
		m_config.Gyro.NumBiasSamples = 4000;
		m_config.Gyro.Watermark = 2;
		m_config.Mag.DeviceName = "/dev/mag0";
		m_config.Servo.DeviceName = "/dev/pwm0";
		m_config.Servo.ChannelMask = 0xff;
		m_config.Servo.Rate = 390;
		m_config.Servo.BitCount = 0;
		m_config.Quad.Motor[0] = 0;
		m_config.Quad.Motor[1] = 1;
		m_config.Quad.Motor[2] = 2;
		m_config.Quad.Motor[3] = 3;
		m_config.Quad.ImuAngleAxisZ = -135;
		/*
		 * PID coefficients for big props on DJI F450
		 * Kp = 1.0 <-> 1.3
		 * Ki = 1.2, with leak rate dT
		 * Kd = 0.3 <-> 0.45
		 */
		m_config.Quad.Kp = 1.0;
		m_config.Quad.Ki = 1.2;
		m_config.Quad.Kd = 0.35;
		m_config.LogLevel = SD_LOG_LEVEL_VERBOSE;
		m_config.LogRate = 1;

		m_config.Accel.CoordinateMap = Matrix3d(
				-1, 0, 0,
				0, 1, 0,
				0, 0, 1);
		m_config.Mag.CoordinateMap = m_config.Accel.CoordinateMap;
		m_config.Gyro.CoordinateMap = Matrix3d(
				1, 0, 0,
				0, -1, 0,
				0, 0, -1);
	} else {
		// BIG TODO:
	}
}

void DroneConfig::PrintConfig()
{
	printf("Drone configuration:\n");
	printf("Gyro device:    %s\n",m_config.Gyro.DeviceName.c_str());
	printf("Gyro rate:      %d Hz\n",m_config.Gyro.SamplingRate);
	printf("Gyro scale:     %d DPS\n",m_config.Gyro.Scale);
	printf("Gyro watermark: %d\n",m_config.Gyro.Watermark);
	printf("Accel device:   %s\n",m_config.Accel.DeviceName.c_str());
	printf("Accel rate:     %d Hz\n",m_config.Accel.SamplingRate);
	printf("Accel scale:    %d G\n",m_config.Accel.Scale);
	printf("Accel watermrk: %d\n",m_config.Accel.Watermark);
	printf("Compass:        %s\n",m_config.Mag.DeviceName.c_str());
	printf("IMU Angle:      %d\n",m_config.Quad.ImuAngleAxisZ);
	for (int i = 0; (size_t)i < ARRAYSIZE(m_config.Quad.Motor); i++) {
		printf("Quad M%d:        Servo %d\n",i,m_config.Quad.Motor[i]);
	}
	printf("Kp:             %1.8f\n", m_config.Quad.Kp);
	printf("Ki:             %1.8f\n", m_config.Quad.Ki);
	printf("Kd:             %1.8f\n", m_config.Quad.Kd);
}
