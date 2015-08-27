/*
 * flightcontrol.h
 *
 *  Created on: Jul 18, 2015
 *      Author: svassilev
 */

#ifndef FLIGHTCONTROL_H_
#define FLIGHTCONTROL_H_

#include <stdint.h>
#include "digitalout.h"
#include "servocontroller.h"
#include "rcreceiver.h"
#include "rcvalueconverter.h"
#include "pidpilot.h"
#include "alarm.h"
#include "altitudetracker.h"

static const float RC_VALUE_SCALE_FACTOR = 1.0;

static const Throttle EMERGENCY_THROTTLE(0.3f);

class FlightControl
{
public:
	FlightControl();
	void start_receiver();
	void stop_receiver();
	QuaternionF target_q() const;
	Throttle base_throttle() const;
	void set_throttle(const std::vector<Throttle>& thrVec);
	void send_throttle_to_motors();
	void set_flight_ceiling(const Altitude& altitude);

	/*
	 * Starts/stops servo PWM output, based on previously received command
	 * Must not be called from interrupt context - it calls malloc/free!
	 */
	void process_servo_start_stop_command();

	/*
	 * Override command coming from the RC
	 */
	void motor_power_on_off(bool power_on);

	void update_state(DroneState& state);
	void safety_check(DroneState& state);

	inline RcReceiver& rc_receiver() { return rc_receiver_; }
	inline ServoController& servo() { return servo_ctrl_; }
	inline PidPilot& pilot() { return pilot_; }
	inline AltitudeTracker& altitude_tracker() { return altitude_track_; }

private:
	void rc_callback() {
		rc_values_.update();
	}
	void record_alarm(const Alarm& alarm);
	void clear_alarm();

private:
	RcReceiver rc_receiver_;
	RcChannelMapper ch_mapper_;
	RcValueConverter rc_values_;
	ServoController servo_ctrl_;
	DigitalOut motor_power_;
	PidPilot pilot_;
	Alarm alarm_;
	Alarm most_critical_alarm_;
	AltitudeTracker altitude_track_;
};


#endif /* FLIGHTCONTROL_H_ */
