/*
 * missioncontrol.h
 *
 *  Created on: May 10, 2013
 *      Author: svassilev
 */

#ifndef MISSIONCONTROL_H_
#define MISSIONCONTROL_H_

#include "commoninc.h"
#include "matrix.h"

/*
 *
 * Makes sure the mission gets executed correctly. For the most part a mission
 * is envisioned to be a set of checkpoints, which must be visited by the drone.
 */
class MissionControl
{
public:
	MissionControl();
	~MissionControl();
	int Reset(
			const SdDroneConfig*
			);
	const QuaternionD& GetDesiredAttitude();
	double GetDesiredThrust();

	void SetDesiredAttitude(const QuaternionD* att);
	void IncThrust(double increment);
	void ResetThrust(double thrust);

private:
	QuaternionD m_DesiredAttitude;
	double m_Thrust;
	double m_HoverThrust;
};

#endif /* MISSIONCONTROL_H_ */
