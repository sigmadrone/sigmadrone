#ifndef _IPILOT_H_
#define _IPILOT_H_

#include "commoninc.h"
#include "pidcontroller.h"
#include "firfilt.h"

class ServoDevice;

class IPilot
{
public:
	virtual ~IPilot() {};
	virtual int Reset(const SdDroneConfig*, ServoDevice*) = 0;
	virtual int Update(
			const QuaternionD& currentAttitude,
			const QuaternionD& desiredAttitude,
			const Vector3d& gyroDps,
			double desiredThrust) = 0;

	virtual void SetMinRev(double minRev) = 0;
	virtual void SetMaxRev(double maxRev) = 0;
	virtual const Vector4d* GetMotors() const = 0;
	virtual double GetMinRev() const = 0;
	virtual double GetMaxRev() const = 0;
	virtual const double GetErrorAngle() const = 0;
	virtual const Vector3d& GetErrorAxis() const = 0;
	virtual const QuaternionD& GetErrorQt() const = 0;
	virtual const Vector3d& GetTorqueCorrection() const = 0;
	virtual const Vector3d& GetOmega() const = 0;
	virtual const Vector3d& GetAngularAccel() const  = 0;
};

#endif
