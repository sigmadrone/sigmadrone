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

#ifndef FLIGHTCONTROL_H_
#define FLIGHTCONTROL_H_

#include <memory>
#include <stdint.h>
#include "digitalout.h"
#include "servocontroller.h"
#include "rcreceiver.h"
#include "rcvalueconverter.h"
#include "ipilot.h"
#include "alarm.h"
#include "altitudetracker.h"
#include "altitudecontrol.h"
#include "ipilot.h"

static const float RC_VALUE_SCALE_FACTOR = 1.0;

static const Throttle EMERGENCY_THROTTLE(0.45f);

class FlightControl
{
public:
	FlightControl();
	void start_receiver();
	void stop_receiver();
	QuaternionF target_q() const;
	Throttle base_throttle() const;
	void set_throttle(const std::vector<float>& thrVec);
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
	inline IPilot& pilot() { return *pilot_; }
	inline AltitudeTracker& altitude_tracker() { return altitude_track_; }

private:
	void rc_callback() {
		rc_values_.update();
	}
	void record_alarm(const Alarm& alarm);
	void clear_alarm();
	void clear_alarm_if(Alarm::AlarmType alarmToClear);

private:
	RcReceiver rc_receiver_;
	RcChannelMapper ch_mapper_;
	RcValueConverter rc_values_;
	ServoController servo_ctrl_;
	DigitalOut motor_power_;
	std::unique_ptr<IPilot> pilot_;
	Alarm alarm_;
	Alarm most_critical_alarm_;
	AltitudeTracker altitude_track_;
	AltitudeControl altitude_control_;
};


#endif /* FLIGHTCONTROL_H_ */
