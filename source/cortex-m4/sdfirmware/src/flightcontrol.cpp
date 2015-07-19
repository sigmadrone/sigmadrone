/*
 * flightcontrol.cpp
 *
 *  Created on: Jul 18, 2015
 *      Author: svassilev
 */

#include "flightcontrol.h"
#include "colibripwm.h"

FlightControl::FlightControl() : rc_receiver_(colibri::PWM_RX_CONSTS,
		FunctionPointer(this, &FlightControl::rc_callback)),
		ch_mapper_({RC_CHANNEL_THROTTLE, RC_CHANNEL_RUDDER, RC_CHANNEL_ELEVATOR, RC_CHANNEL_AILERON, RC_CHANNEL_ARM_MOTOR}),
		rc_values_(ch_mapper_, rc_receiver_, RC_VALUE_SCALE_FACTOR) ,
		servo_ctrl_({colibri::PWM_TX_1_4}, Frequency::from_hertz(400)),
		motor_power_(PB_2) {
}

void FlightControl::start_receiver() {
	rc_receiver_.start();
}

void FlightControl::stop_receiver() {
	rc_receiver_.stop();
}

QuaternionF FlightControl::target_q() const {
	return rc_values_.target_quaternion();
}

Throttle FlightControl::base_throttle() const {
	return rc_values_.base_throttle();
}

void FlightControl::set_throttle(const std::vector<Throttle>& thrVec) {
	PwmPulse pulse(TimeSpan::from_milliseconds(1), TimeSpan::from_milliseconds(2));
	for (size_t i = 0; i < thrVec.size(); ++i) {
		servo_ctrl_.set_pwm_pulse(i, pulse.to_timespan(thrVec[i].get()));
	}
}

void FlightControl::update_throttle() {
	set_throttle({pilot_.motors().at(0,0), pilot_.motors().at(1,0),
		pilot_.motors().at(2,0), pilot_.motors().at(3,0)});
}

/*
 * Must not be called from interrupt context - it calls malloc/free!
 */
void FlightControl::process_servo_start_stop_command() {
	if (rc_values_.motors_armed()) {
		servo_ctrl_.start();
		motor_power_.write(1);
	} else {
		motor_power_.write(0);
		servo_ctrl_.stop();
	}
}

void FlightControl::motor_power_on_off(bool power_on) {
	motor_power_.write(power_on ? 1 : 0);
}
