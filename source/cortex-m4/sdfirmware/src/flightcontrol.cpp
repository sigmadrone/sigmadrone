/*
 *  Sigmadrone
 *  Copyright (c) 2013-2015 The Sigmadrone Developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Martin Stoilov <martin@sigmadrone.org>
 *  Svetoslav Vassilev <svassilev@sigmadrone.org>
 */

#include "flightcontrol.h"
#include <stdio.h>
#include "colibripwm.h"
#include "tripilot.h"

FlightControl::FlightControl() :
        rc_receiver_(colibri::PWM_RX_CONSTS,
				FunctionPointer(this, &FlightControl::rc_callback)),
		ch_mapper_({RC_CHANNEL_THROTTLE, RC_CHANNEL_RUDDER, RC_CHANNEL_ELEVATOR, RC_CHANNEL_AILERON, RC_CHANNEL_ARM_MOTOR}),
		rc_values_(ch_mapper_,
				rc_receiver_,
				TimeSpan::from_microseconds(1100),
				TimeSpan::from_microseconds(1910)),
		servo_ctrl_({colibri::PWM_TX_1_4, colibri::PWM_TX_5_8}, Frequency::from_hertz(350)),
		motor_power_(MOTOR_POWER_CTRL_PIN),
		pilot_(new TriPilot()),
		altitude_track_()
{
}

void FlightControl::start_receiver()
{
	rc_receiver_.start();
}

void FlightControl::stop_receiver()
{
	rc_receiver_.stop();
}

QuaternionF FlightControl::target_q() const
{
	return alarm_.is_none() ? rc_values_.target_quaternion() : QuaternionF(1,0,0,0);
}

Throttle FlightControl::base_throttle() const
{
	if (alarm_.is_none() || rc_values_.base_throttle() < EMERGENCY_THROTTLE) {
		return rc_values_.base_throttle();
	}
	return EMERGENCY_THROTTLE;
}

void FlightControl::set_throttle(const std::vector<float>& thrVec)
{
	PwmPulse pulse(TimeSpan::from_microseconds(1050), TimeSpan::from_microseconds(2000));
	for (size_t i = 0; i < thrVec.size(); ++i) {
		servo_ctrl_.set_pwm_pulse(i, pulse.to_timespan(
				(base_throttle().get() >= 0.075) ? Throttle(thrVec[i]).get() : 0.0));
	}
}

void FlightControl::send_throttle_to_motors()
{
	set_throttle(pilot_->motors());
}

/*
 * Must not be called from interrupt context - it calls malloc/free!
 */
void FlightControl::process_servo_start_stop_command()
{
	if (rc_values_.motors_armed()) {
		servo_ctrl_.start();
		motor_power_.write(1);
	} else {
		motor_power_.write(0);
		servo_ctrl_.stop();
	}
}

void FlightControl::motor_power_on_off(bool power_on)
{
	motor_power_.write(power_on ? 1 : 0);
}

void FlightControl::update_state(DroneState& state)
{
	if (state.pilot_type_ != pilot_->get_pilot_type()) {
		if (PILOT_TYPE_PID_NEW == state.pilot_type_) {
			pilot_.reset(new TriPilot());
		}
	}

	if (rc_values_.base_throttle().get() < 0.1) {
		rc_values_.reset_twist_quaternion(state.attitude_);
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
	state.gear_ = rc_values_.get_gear();
	state.gear_alive_ = rc_values_.gear_alive_;
	state.base_throttle_ = base_throttle().get();
	state.motors_armed_ = rc_values_.motors_armed();

	position_control_.update_state(state);

	altitude_control_.update_state(state);
	state.base_throttle_ = altitude_control_.get_throttle().get();

	pilot().update_state(state);
}

void FlightControl::safety_check(DroneState& drone_state)
{
	if (altitude_track_.is_flight_ceiling_hit()) {
		std::string alarmMessage("Alt: ");
		char tmpBuf[6] = {0};
		sprintf(tmpBuf,"%5.1f",drone_state.altitude_.meters());
		alarmMessage += std::string(tmpBuf) + std::string(" / Ceiling: ");
		sprintf(tmpBuf,"%5.1f", altitude_track_.flight_ceiling_absolute().meters());
		alarmMessage += std::string(tmpBuf) + std::string(" m");
		record_alarm(Alarm(Alarm::ALARM_CEILING_HIT,alarmMessage));
	} else {
		clear_alarm_if(Alarm::ALARM_CEILING_HIT);
	}

	/*
	 * Are all the RC channels live?
	 */
	uint32_t i;
	for (i = 0; i < rc_receiver_.channel_count(); ++i) {
		if (!rc_receiver_.channel(i)->is_live()) {
			record_alarm(Alarm(Alarm::ALARM_RC_CHANNEL_DEAD,
					std::string(RcChannelToString(ch_mapper_.channel_name(i)))));
			break;
		}
	}

	if (i == rc_receiver_.channel_count()) {
		clear_alarm_if(Alarm::ALARM_RC_CHANNEL_DEAD);
	}

	/*
	 * More checks to be added
	 */

	drone_state.alarm_ = alarm_;
	drone_state.most_critical_alarm_ = most_critical_alarm_;
}

void FlightControl::clear_alarm()
{
	alarm_ = Alarm();
}

void FlightControl::clear_alarm_if(Alarm::AlarmType alarmToClear)
{
	if (alarm_.type() == alarmToClear) {
		clear_alarm();
	}
}

void FlightControl::record_alarm(const Alarm& alarm) {
	if (alarm_.type() <= alarm.type()) {
		alarm_ = alarm;
	}
	if (most_critical_alarm_.type() < alarm.type()) {
		most_critical_alarm_ = alarm;
	}
}
