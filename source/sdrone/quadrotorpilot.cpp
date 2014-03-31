/* * quadrotorpilot.cpp
 *
 *  Created on: May 27, 2013
 *      Author: svassilev
 */
#include "quadrotorpilot.h"
#include <dlfcn.h>

static const double s_InitialStep = 0.005;

QuadRotorPilot::QuadRotorPilot() :
		m_PidCtl(0,0,0,0),
		m_OmegaPidCtl(0,0,0,0),
		m_Step(s_InitialStep,s_InitialStep,s_InitialStep)
{
	m_MinRev = 0.0;
	m_MaxRev = 1.0;
	m_Counter = 0;
	m_Skip = 0;
	m_ErrorAngle = 0;
	m_Runtime = 0;
	m_RefCnt = 1;
	m_DesiredRev = 0.0;
}

QuadRotorPilot::~QuadRotorPilot()
{
	assert(0 == m_RefCnt);
}

int QuadRotorPilot::AttachToChain(
	void* droneContext,
	SdPluginAttachFunc attachFunc)
{
	static const char* attachAbove[] = {SD_PLUGIN_NAVIGATOR,0};
	int err = attachFunc(
		droneContext,
		this,
		SD_ALTITUDE_GROUP_APP+SD_ALTITUDE_FIRST/2,
		attachAbove,
		0,
		&m_Runtime);
	return err;
}

int QuadRotorPilot::Start(
		const CommandArgs* cmdArgs)
{
	const SdDroneConfig* config = cmdArgs->GetDroneConfig();
	assert(config);
	assert(m_Runtime);
	m_Config = config->Quad;
	m_PidCtl.Reset(m_Config.Kp,m_Config.Ki,m_Config.Kd,30);
	m_OmegaPidCtl.Reset(0,m_Config.Ki/2,m_Config.Kd/4,10);
	m_Counter = 0;
	m_Motors.clear();
	m_PrevQ = QuaternionD(1, 0, 0, 0);
	m_Omega.clear();
	m_AngAccel.clear();
	m_RotZQ = QuaternionD::fromAxisRot(
		Vector3d(0,0,1),DEG2RAD(-m_Config.ImuAngleAxisZ));
	m_GyroFilt.Reset();
	m_Step = Vector3d(s_InitialStep,s_InitialStep,s_InitialStep);
	m_MinRev = cmdArgs->GetMinThrust();
	m_MaxRev = cmdArgs->GetMaxThrust();
	m_DesiredRev = cmdArgs->GetDesiredThrust();

	m_Runtime->SetIoFilters(
			SD_DEVICEID_TO_FLAG(SD_DEVICEID_IMU) |
			SD_DEVICEID_TO_FLAG(SD_DEVICEID_COMMAND),
			SD_IOCODE_TO_FLAG(SD_IOCODE_RECEIVE)|
			SD_IOCODE_TO_FLAG(SD_IOCODE_COMMAND));

	return 0;
}

int QuadRotorPilot::AddRef()
{
	return __sync_fetch_and_add(&m_RefCnt,1);
}

int QuadRotorPilot::Release()
{
	int refCnt = __sync_sub_and_fetch(&m_RefCnt,1);
	if (0 == refCnt) {
		delete this;
	}
	return refCnt;
}

void QuadRotorPilot::Stop(int flags)
{
	/*
	 * Shutdown the motors
	 */
	m_MinRev = m_MaxRev = 0;
	CalcThrustFromErrAxis(Vector3d(0,0,0),0);
	IssueCommandToServo();
	if (!!(flags&FLAG_STOP_AND_DETACH)) {
		m_Runtime->DetachPlugin();
	}
}

const char* QuadRotorPilot::GetName()
{
	return SD_PLUGIN_QUADPILOT;
}

SdDeviceId QuadRotorPilot::GetDeviceId()
{
	return SD_DEVICEID_FILTER;
}

const char* QuadRotorPilot::GetVersion()
{
	return SD_PLUGIN_VERSION;
}

extern char **g_Argv;
const char* QuadRotorPilot::GetDlFileName()
{
	return g_Argv[0];
}

int QuadRotorPilot::IoCallback(
	SdIoPacket* ioPacket)
{
	int err = 0;
	if (SD_IOCODE_RECEIVE == ioPacket->IoCode()) {

		/*
		 * Check whether the IO carries all the necessary info for the pilot
		 */
		UpdateState(ioPacket);

		CalcThrustFromErrAxis(m_ErrorAxisPid,m_DesiredRev);
		if (0 == (m_Counter++%(m_Skip+1)))
		{
			//
			// Issue commands to the servos. Since the servos are controlled by
			// different device, we must issue new SdIoPacket "down" to the servo
			// device. We do not want to stop the current packet as there may
			// be another plugin on top that performs different functions, aka
			// logging, etc.
			//
			err = IssueCommandToServo();
		}
		/*
		 *  Set the motor values in the IO structures so it can be used by the
		 *  rest of the chain
		 */
		ioPacket->SetAttribute(SDIO_ATTR_MOTORS,SdIoData(&m_Motors));

	} else if (SD_IOCODE_COMMAND == ioPacket->IoCode()) {
		if (ioPacket->GetIoData(true).dataType == SdIoData::TYPE_COMMAND_ARGS) {
			const CommandArgs* args = ioPacket->GetIoData(true).asCommandArgs;
			SetMinRev(args->GetMinThrust());
			SetMaxRev(args->GetMaxThrust());
			m_DesiredRev = fmin(args->GetMaxThrust(),args->GetDesiredThrust());
			m_DesiredRev = fmax(args->GetMinThrust(),m_DesiredRev);
		} else {
			assert(false);
			err = EINVAL;
		}

	} else {
		assert(false);
	}
	return err;
}

int QuadRotorPilot::IssueCommandToServo()
{
	//
	// Since the servos are controlled by different device, we must issue new
	// SdIoPacket "down" to the servo device.
	//
	int err = ENOMEM;
	SdServoIoData servoData;
	SdIoPacket* ioPacket = m_Runtime->AllocIoPacket(
			SD_IOCODE_SEND,
			SD_DEVICEID_SERVO,
			SD_PLUGIN_SERVO_PCA9685);
	if (0 != ioPacket) {
		servoData.numChannels = 4;
		for (size_t i = 0; i < ARRAYSIZE(m_Config.Motor); i++) {
			servoData.channels[i] = m_Config.Motor[i];
			servoData.value[i] = m_Motors.at(i,0);
		}
		ioPacket->SetIoData(SdIoData(&servoData),true);
		err = m_Runtime->DispatchIo(ioPacket,SD_FLAG_DISPATCH_DOWN);
		m_Runtime->FreeIoPacket(ioPacket);
	} else {
		m_Runtime->Log(SD_LOG_LEVEL_ERROR,
				"Quadrotor pilot failed to alloc iopacket!\n");
	}
	return err;
}

template <typename T> __inline int sign(T val)
{
	return (T(0) < val) - (val < T(0));
}


int QuadRotorPilot::UpdateState(
	SdIoPacket* ioPacket)
{
	QuaternionD attitudeQ = ioPacket->Attitude();
	const QuaternionD targetQ = ioPacket->TargetAttitude();
	//const QuaternionD targetQ = QuaternionD(0.9999,-0.04,0.02,0);
	int retVal = 0;
	Vector3d currentOmega;

	attitudeQ.z = 0;
	attitudeQ = attitudeQ.normalize();

	//
	// Here we assume that m1 and m3 lie on the X axis, while m2 and m4
	// lie on Y axis. Positive/CCW rotation around X axis is achieved by
	// revving up m4 and revving down m2. Positive/CCW rotation around
	// the Y axis is achieved by revving down m1 and revving up m3.
	// Positive/CCW rot around the Z axis is achieved by revving up
	// m2 and m4 and revving down m1 and m3, thus m1/m3 must rotate CCW and
	// m2/m4 rotate clockwise Overall thrust up (alongside) Z is achieved
	// by rev-ing proportionally all 4 motors.
	//
	QuaternionD errQ = targetQ * (~attitudeQ);

#if 0
	errQ = errQ.normalize();
#endif

	double angleDeg = RAD2DEG(errQ.angle());
	if (fabs(angleDeg) > 180) {
		Vector3d axis = errQ.axis() * -1.0f;
		double rad = DEG2RAD((angleDeg > 0) ? 360-angleDeg : -360+angleDeg);
		errQ = QuaternionD::fromAxisRot(axis,rad);
#if 0
		errQ = errQ.normalize();
#endif
		angleDeg = RAD2DEG(errQ.angle());
	}

	angleDeg = fmin(angleDeg,90);

	Vector3d errAxis;
	if (angleDeg > 5) {
		errAxis = errQ.axis().normalize() * sin(DEG2RAD(angleDeg));
	} else {
		errAxis = errQ.axis().normalize() * DEG2RAD(angleDeg);
	}

	errAxis.at(2,0) = 0; // clear Z axis, we will take care of that later
	errAxis = m_RotZQ.rotate(errAxis);

	//
	// Feed the error into the pid controller
	//
	m_ErrorP = m_PidCtl.GetP(errAxis);
	m_ErrorI = m_PidCtl.GetI(
			errAxis,
			ioPacket->DeltaTime(),
			ioPacket->DeltaTime(),
			Vector3d(0.5,0.5,0.5));
#if 1
	m_ErrorD = m_PidCtl.GetD(
			errAxis,
			ioPacket->DeltaTime());
	m_ErrorAxisPid  = m_ErrorP+m_ErrorI+m_ErrorD;
#else
	/*
	 * Differential error scheme based on the differential of the plant
	 * output as opposed the error.
	 */
	QuaternionD qdiff = attitudeQ * ~m_PrevQ;
	double diffAngle = qdiff.angle();
	if (RAD2DEG(diffAngle) > 5) {
		diffAngle = sin(diffAngle);
	}
	Vector3d diffErrAxis = qdiff.axis().normalize() * diffAngle;
	diffErrAxis = m_RotZQ.rotate(diffErrAxis);
	m_ErrorD = m_PidCtl.GetD(
				diffErrAxis,
				ioPacket->DeltaTime());
	m_ErrorAxisPid  = m_ErrorP+m_ErrorI-m_ErrorD;
#endif

	currentOmega = QuaternionD::angularVelocity(attitudeQ,m_PrevQ,
			ioPacket->DeltaTime()) * 180 / M_PI;
	currentOmega = m_RotZQ.rotate(currentOmega);
	currentOmega = m_GyroFilt.DoFilter(currentOmega);
	Vector3d angAccel = (currentOmega-m_Omega)/ioPacket->DeltaTime();

	Vector3d omegaErrPid;
#if 0
	/*
	 * The difference between desired omega and actual omega can only be
	 * used for torque correction
	 */
	Vector3d desiredOmega = CalcDesiredOmega3d(m_ErrorAxisPid);
	Vector3d omegaErr = (desiredOmega - currentOmega) / (2*RAD2DEG(2*M_PI));

	omegaErrPid  = omegaErrPid+m_OmegaPidCtl.GetP(omegaErr);
	omegaErrPid = omegaErrPid + m_OmegaPidCtl.GetI(
			omegaErr,
			ioPacket->DeltaTime(),
			0.0,
			Vector3d(0.2,0.2,0.2));
	omegaErrPid = omegaErrPid + m_OmegaPidCtl.GetD(
			omegaErrPid,ioPacket->DeltaTime());

	m_ErrorAxisPid = m_ErrorAxisPid + omegaErrPid;
#endif

	m_ErrorAxis = errAxis;

	m_ErrorAngle = angleDeg;
	m_PrevQ = attitudeQ;
	m_AngAccel = angAccel;
	m_Omega = currentOmega;

	ioPacket->SetAttribute(SDIO_ATTR_ERR_PID,SdIoData(&m_ErrorAxisPid));
	ioPacket->SetAttribute(SDIO_ATTR_ERR_P,SdIoData(&m_ErrorP));
	ioPacket->SetAttribute(SDIO_ATTR_ERR_I,SdIoData(&m_ErrorI));
	ioPacket->SetAttribute(SDIO_ATTR_ERR_D,SdIoData(&m_ErrorD));
	ioPacket->SetAttribute(SDIO_ATTR_ERR_ANGLE,SdIoData(angleDeg));
	ioPacket->SetAttribute(SDIO_ATTR_ERR_OMEGA,SdIoData(&omegaErrPid));

	return retVal;
}

void QuadRotorPilot::CalcThrustFromErrAxis(
		const Vector3d& err,
		double _targetThrust)
{
	//
	// Control the motors in pairs 1-3 and 2-4.
	// We use the following equations:
	// 		torgX = l * k * (m4^2 - m2^2);
	// 		torgY = l * k * (m3^2 - m1^2);
	// 		torgZ = b * (-m1^2 + m2^2 - m3^2 + m4^2);
	// 		thrust = k * (m1^2 + m2^2 + m3^2 + m4^2);
	// Since the rotation axis is exactly the same as the desired torque,
	// we need to find w (motor rot velocity) deltas, that satisfy
	// the equations above.
	//
	double targetThrust = fmax(_targetThrust,0.01);
	double deltaX = err.at(0,0)/(4 * targetThrust);
	double deltaY = err.at(1,0)/(4 * targetThrust);
	double deltaZ = err.at(2,0)/(8 * targetThrust);
	double m1 = targetThrust;
	double m2 = targetThrust;
	double m3 = targetThrust;
	double m4 = targetThrust;

	m2 -= deltaX;
	m4 += deltaX;
	m1 -= deltaY;
	m3 += deltaY;

	m1 -= deltaZ;
	m3 -= deltaZ;
	m2 += deltaZ;
	m4 += deltaZ;

	SetAndScaleMotors(m1,m2,m3,m4);
}

void QuadRotorPilot::SetAndScaleMotors(
		double m1,
		double m2,
		double m3,
		double m4)
{
	Vector4d mv(m1,m2,m3,m4);
	double minVal = mv.minValue();
	if (minVal < m_MinRev) {
		mv = mv + (m_MinRev - minVal);
	}
	double maxVal = mv.maxValue();
	if (maxVal > m_MaxRev) {
		mv = mv - (maxVal - m_MaxRev);
	}
	maxVal = mv.maxValue();
	minVal = mv.minValue();
	if (minVal < m_MinRev || maxVal > m_MaxRev) {
		double scale = (m_MaxRev-m_MinRev)/(maxVal-minVal);
		mv = mv - minVal;
		mv = mv * scale;
		mv = mv + m_MinRev;
	}
	m_Motors = mv;
}

const Vector4d* QuadRotorPilot::GetMotors() const
{
	return &m_Motors;
}

void QuadRotorPilot::SetMinRev(double minRev)
{
	m_MinRev = fmax(0,minRev);
}

void QuadRotorPilot::SetMaxRev(double maxRev)
{
	m_MaxRev = fmin(1,maxRev);
}

double QuadRotorPilot::CalcDesiredOmega(double _err)
{
	double err = fabs(fmin(_err*100,100));
	double omega = fmin(pow(err,2)/10,RAD2DEG(2*M_PI));
	return (_err > 0) ? omega : -omega;
}

Vector3d QuadRotorPilot::CalcDesiredOmega3d(
	const Vector3d& err)
{
	Vector3d omega;
	omega.at(0,0) = CalcDesiredOmega(err.at(0,0));
	omega.at(1,0) = CalcDesiredOmega(err.at(1,0));
	omega.at(2,0) = CalcDesiredOmega(err.at(2,0));
	return omega;
}
