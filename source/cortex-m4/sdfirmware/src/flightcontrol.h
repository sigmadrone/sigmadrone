/*
 * flightcontrol.h
 *
 *  Created on: Jul 18, 2015
 *      Author: svassilev
 */

#ifndef FLIGHTCONTROL_H_
#define FLIGHTCONTROL_H_

#include "hwtimer.h"
#include "digitalout.h"
#include "servocontroller.h"
#include "rcreceiver.h"
#include "rcvalueconverter.h"

static const float RC_VALUE_SCALE_FACTOR = 1.0;

class FlightControl
{
public:
	FlightControl();
	void start_receiver();
	void stop_receiver();
	QuaternionF target_q() const;
	Throttle base_throttle() const;
	void set_throttle(const std::vector<Throttle>& thrVec);
	void update_throttle();

	/*
	 * Starts/stops servo PWM output, based on previously received command
	 * Must not be called from interrupt context - it calls malloc/free!
	 */
	void process_servo_start_stop_command();

	/*
	 * Override command coming from the RC
	 */
	void motor_power_on_off(bool power_on);

	inline RcReceiver& rc_receiver() { return rc_receiver_; }
	inline ServoController& servo() { return servo_ctrl_; }

private:
	void rc_callback() {
		rc_values_.update();
	}

private:
	RcReceiver rc_receiver_;
	RcChannelMapper ch_mapper_;
	RcValueConverter rc_values_;
	ServoController servo_ctrl_;
	DigitalOut motor_power_;
};


#endif /* FLIGHTCONTROL_H_ */
