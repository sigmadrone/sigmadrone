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
		rc_values_(ch_mapper_, rc_receiver_, RC_VALUE_SCALE_FACTOR, TimeSpan::from_microseconds(1100),
				TimeSpan::from_microseconds(1910)),
		servo_ctrl_({colibri::PWM_TX_1_4}, Frequency::from_hertz(400)),
		motor_power_(PB_2), pilot_(), altitude_track_() {
}

void FlightControl::start_receiver() {
	rc_receiver_.start();
}

void FlightControl::stop_receiver() {
	rc_receiver_.stop();
}

QuaternionF FlightControl::target_q() const {
	return alarm_.is_none() ? rc_values_.target_quaternion() : QuaternionF(1,0,0,0);
}

Throttle FlightControl::base_throttle() const {
	if (alarm_.is_none() || rc_values_.base_throttle() < EMERGENCY_THROTTLE) {
		return rc_values_.base_throttle();
	}
	return EMERGENCY_THROTTLE;
}

void FlightControl::set_throttle(const std::vector<Throttle>& thrVec) {
	PwmPulse pulse(TimeSpan::from_milliseconds(1), TimeSpan::from_milliseconds(2));
	for (size_t i = 0; i < thrVec.size(); ++i) {
		servo_ctrl_.set_pwm_pulse(i, pulse.to_timespan(
				(base_throttle().get() >= 0.1) ? thrVec[i].get() : 0.0));
	}
}

void FlightControl::send_throttle_to_motors() {
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

void FlightControl::update_state(DroneState& state)
{
	if (rc_values_.base_throttle().get() < 0.1) {
		rc_values_.reset_yaw_quaternion(state.attitude_);
	}

	if (!rc_values_.motors_armed()) {
		altitude_tracker().reset();
	}

	state.target_ = target_q();

	process_servo_start_stop_command();
	altitude_tracker().update_state(state);
	safety_check(state);

	state.yaw_ = rc_values_.get_yaw();
	state.pitch_ = rc_values_.get_pitch();
	state.roll_ = rc_values_.get_roll();
	state.base_throttle_ = base_throttle().get();

	pilot().update_state(state);
}

void FlightControl::safety_check(DroneState& drone_state) {

	if (!rc_values_.motors_armed()) {
		/*
		 * motors are not armed, we will still perform the rest of the checks
		 * so the alarm will persist if the underlying problem was not fixed
		 */
		clear_alarm();
	}

	if (altitude_track_.is_flight_ceiling_hit()) {
		record_alarm(Alarm(Alarm::ALARM_CEILING_HIT));
	}

	/*
	 * Are all the RC channels live?
	 */
	for (uint32_t i = 0; i < rc_receiver_.channel_count(); ++i) {
		if (!rc_receiver_.channel(i)->is_live()) {
			record_alarm(Alarm(Alarm::ALARM_RC_CHANNEL_DEAD, i));
			break;
		}
	}

	/*
	 * More checks to be added
	 */

	drone_state.alarm_ = alarm_;
	drone_state.most_critical_alarm_ = most_critical_alarm_;
}

void FlightControl::clear_alarm() {
	alarm_ = Alarm();
}

void FlightControl::record_alarm(const Alarm& alarm) {
	if (alarm_.type() < alarm.type()) {
		alarm_ = alarm;
	}
	if (most_critical_alarm_.type() < alarm.type()) {
		most_critical_alarm_ = alarm;
	}
}
