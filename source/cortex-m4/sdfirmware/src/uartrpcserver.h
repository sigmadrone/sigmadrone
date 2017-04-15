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

#ifndef UARTRPCSERVER_H_
#define UARTRPCSERVER_H_

#include "uart.h"
#include "dronestate.h"
#include "flashmemory.h"
#include "librexjsonrpc/rpcserver.h"

class MagCalibrator;

class UartRpcServer : public rpc_server<UartRpcServer, UART*>
{
public:
	UartRpcServer(DroneState& dronestate, FlashMemory& configdata, MagCalibrator& mag_calibrator);
	virtual ~UartRpcServer();

	void jsonrpc_request_handler(UART* uart);

protected:
	rexjson::value rpc_spec(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_help(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_attitude(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_altitude(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_pressure(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_temperature(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_motors(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_dronestate(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_dronestate_ex(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_configdata(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_set_configdata(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_save_bootconfigdata_to_flash(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_get_datastream(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);

	rexjson::value rpc_accelerometer_adjustment(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_kp(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_kd(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_ki(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_yaw_kp(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_yaw_kd(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_yaw_ki(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_pid_filter_freq(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_gyro_drift_kp(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_gyro_drift_ki(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_gyro_drift_kd(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_gyro_drift_leak_rate(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_altitude_kp(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_altitude_kd(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_altitude_ki(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_altitude_ki_leak(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_altitude_tracker_kp_ki_kd(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_altitude_tracker_kp2(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_altitude_correction_period(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_altitude_filter(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);

	rexjson::value rpc_yaw_bias(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_pitch_bias(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_roll_bias(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);

	rexjson::value rpc_set_accelerometer_correction_period(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_set_gyro_factor(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_set_yaw_throttle_factor(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_flight_ceiling(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_restore_config(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_set_mag_tracking(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_set_acc_tracking(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_set_pilot_type(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_enable_external_gyro(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_external_gyro_align(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_flight_mode(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_calibrate_mag(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);

	rexjson::value rpc_position_kp(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_position_kd(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);
	rexjson::value rpc_position_ki(UART* uart, rexjson::array& params, rpc_exec_mode mode = execute);


protected:
	std::string cached_request_;
	DroneState& dronestate_;
	FlashMemory& configdata_;
	MagCalibrator& mag_calibrator_;
};

#endif /* UARTRPCSERVER_H_ */
