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
#include <boost/lexical_cast.hpp>
#include "mainwindow.h"

mainwindow::mainwindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
	: Gtk::Window(cobject)
	, timer_number_(0)
	, rpcuri_("/jsonrpc")
	, firmware_rpcuri_("/firmware")
	, rpc_client_(new rpc_client_http("localhost", "18222"))
	, ref_glade_(refGlade)
	, button_quit_(NULL)
	, button_arm_motors_(NULL)
	, button_lock_motors_(NULL)
	, spinbutton_thrust_(NULL)
	, spinbutton_m1_(NULL)
	, spinbutton_m2_(NULL)
	, spinbutton_m3_(NULL)
	, spinbutton_m4_(NULL)
	, spinbutton_acc_period_(NULL)
	, spinbutton_gyro_factor_(NULL)
	, spinbutton_yaw_throttle_factor_(NULL)
	, label_m1_(NULL)
	, label_m2_(NULL)
	, label_m3_(NULL)
	, label_m4_(NULL)
	, label_attitude_w_(NULL)
	, label_attitude_x_(NULL)
	, label_attitude_y_(NULL)
	, label_attitude_z_(NULL)
	, label_target_w_(NULL)
	, label_target_x_(NULL)
	, label_target_y_(NULL)
	, label_target_z_(NULL)
	, label_accelerometer_x_(NULL)
	, label_accelerometer_y_(NULL)
	, label_accelerometer_z_(NULL)
	, label_accelerometer_length_(NULL)
	, label_twist_w_(NULL)
	, label_twist_x_(NULL)
	, label_twist_y_(NULL)
	, label_twist_z_(NULL)
	, spinbutton_xy_kp_(NULL)
	, spinbutton_xy_ki_(NULL)
	, spinbutton_xy_kd_(NULL)
	, spinbutton_z_kp_(NULL)
	, spinbutton_z_ki_(NULL)
	, spinbutton_z_kd_(NULL)
	, label_yaw_(NULL)
	, label_pitch_(NULL)
	, label_roll_(NULL)
	, label_altitude_meters_(NULL)
	, spinbutton_yaw_bias_(NULL)
	, spinbutton_pitch_bias_(NULL)
	, spinbutton_roll_bias_(NULL)
{
	//Get the Glade-instantiated Button, and connect a signal handler:
	ref_glade_->get_widget("button_quit", button_quit_);
	ref_glade_->get_widget("checkbutton_arm_motors", button_arm_motors_);
	ref_glade_->get_widget("checkbutton_lock_motors", button_lock_motors_);
	ref_glade_->get_widget("spinbutton_thrust", spinbutton_thrust_);
	ref_glade_->get_widget("spinbutton_m1", spinbutton_m1_);
	ref_glade_->get_widget("spinbutton_m2", spinbutton_m2_);
	ref_glade_->get_widget("spinbutton_m3", spinbutton_m3_);
	ref_glade_->get_widget("spinbutton_m4", spinbutton_m4_);
	ref_glade_->get_widget("label_m1", label_m1_);
	ref_glade_->get_widget("label_m2", label_m2_);
	ref_glade_->get_widget("label_m3", label_m3_);
	ref_glade_->get_widget("label_m4", label_m4_);
	ref_glade_->get_widget("label_attitude_w", label_attitude_w_);
	ref_glade_->get_widget("label_attitude_x", label_attitude_x_);
	ref_glade_->get_widget("label_attitude_y", label_attitude_y_);
	ref_glade_->get_widget("label_attitude_z", label_attitude_z_);
	ref_glade_->get_widget("label_target_w", label_target_w_);
	ref_glade_->get_widget("label_target_x", label_target_x_);
	ref_glade_->get_widget("label_target_y", label_target_y_);
	ref_glade_->get_widget("label_target_z", label_target_z_);
	ref_glade_->get_widget("label_accelerometer_x", label_accelerometer_x_);
	ref_glade_->get_widget("label_accelerometer_y", label_accelerometer_y_);
	ref_glade_->get_widget("label_accelerometer_z", label_accelerometer_z_);
	ref_glade_->get_widget("label_accelerometer_length", label_accelerometer_length_);
	ref_glade_->get_widget("label_twist_w", label_twist_w_);
	ref_glade_->get_widget("label_twist_x", label_twist_x_);
	ref_glade_->get_widget("label_twist_y", label_twist_y_);
	ref_glade_->get_widget("label_twist_z", label_twist_z_);
	ref_glade_->get_widget("spinbutton_xy_kp", spinbutton_xy_kp_);
	ref_glade_->get_widget("spinbutton_xy_ki", spinbutton_xy_ki_);
	ref_glade_->get_widget("spinbutton_xy_kd", spinbutton_xy_kd_);
	ref_glade_->get_widget("spinbutton_z_kp", spinbutton_z_kp_);
	ref_glade_->get_widget("spinbutton_z_ki", spinbutton_z_ki_);
	ref_glade_->get_widget("spinbutton_z_kd", spinbutton_z_kd_);
	ref_glade_->get_widget("label_yaw", label_yaw_);
	ref_glade_->get_widget("label_pitch", label_pitch_);
	ref_glade_->get_widget("label_roll", label_roll_);
	ref_glade_->get_widget("label_altitude_meters", label_altitude_meters_);
	ref_glade_->get_widget("spinbutton_acc_period", spinbutton_acc_period_);
	ref_glade_->get_widget("spinbutton_gyro_factor", spinbutton_gyro_factor_);
	ref_glade_->get_widget("spinbutton_yaw_throttle_factor", spinbutton_yaw_throttle_factor_);
	ref_glade_->get_widget("spinbutton_yaw_bias", spinbutton_yaw_bias_);
	ref_glade_->get_widget("spinbutton_pitch_bias", spinbutton_pitch_bias_);
	ref_glade_->get_widget("spinbutton_roll_bias", spinbutton_roll_bias_);


	connections_.push_back(button_quit_->signal_clicked().connect(sigc::mem_fun(*this, &mainwindow::on_button_quit)));
	connections_.push_back(spinbutton_acc_period_->signal_value_changed().connect(sigc::mem_fun(*this, &mainwindow::on_change_acc_period)));
	connections_.push_back(spinbutton_gyro_factor_->signal_value_changed().connect(sigc::mem_fun(*this, &mainwindow::on_change_gyro_factor)));
	connections_.push_back(spinbutton_yaw_throttle_factor_->signal_value_changed().connect(sigc::mem_fun(*this, &mainwindow::on_change_yaw_factor)));
	connections_.push_back(button_lock_motors_->signal_clicked().connect(sigc::mem_fun(*this, &mainwindow::on_button_lock_motors)));
	connections_.push_back(button_arm_motors_->signal_clicked().connect(sigc::mem_fun(*this, &mainwindow::on_button_arm_motors)));
	connections_.push_back(spinbutton_yaw_bias_->signal_value_changed().connect(sigc::mem_fun(*this, &mainwindow::on_change_yaw_bias)));
	connections_.push_back(spinbutton_pitch_bias_->signal_value_changed().connect(sigc::mem_fun(*this, &mainwindow::on_change_pitch_bias)));
	connections_.push_back(spinbutton_roll_bias_->signal_value_changed().connect(sigc::mem_fun(*this, &mainwindow::on_change_roll_bias)));
	connections_.push_back(spinbutton_thrust_->signal_value_changed().connect(sigc::mem_fun(*this, &mainwindow::on_change_thrust)));
	connections_.push_back(spinbutton_xy_kp_->signal_value_changed().connect(sigc::mem_fun(*this, &mainwindow::on_change_xy_kp)));
	connections_.push_back(spinbutton_xy_ki_->signal_value_changed().connect(sigc::mem_fun(*this, &mainwindow::on_change_xy_ki)));
	connections_.push_back(spinbutton_xy_kd_->signal_value_changed().connect(sigc::mem_fun(*this, &mainwindow::on_change_xy_kd)));
	connections_.push_back(spinbutton_z_kp_->signal_value_changed().connect(sigc::mem_fun(*this, &mainwindow::on_change_yaw_kp)));
	connections_.push_back(spinbutton_z_ki_->signal_value_changed().connect(sigc::mem_fun(*this, &mainwindow::on_change_yaw_ki)));
	connections_.push_back(spinbutton_z_kd_->signal_value_changed().connect(sigc::mem_fun(*this, &mainwindow::on_change_yaw_kd)));
}

mainwindow::~mainwindow()
{
	for (size_t i = 0; i < connections_.size(); i++) {
		connections_[i].disconnect();
	}
}

void mainwindow::on_button_quit()
{
	hide(); //hide() will cause main::run() to end.
}

void mainwindow::on_button_lock_motors()
{
	button_arm_motors_->set_sensitive(!button_lock_motors_->get_active());
}

void mainwindow::on_button_arm_motors()
{
}


void mainwindow::on_change_thrust()
{
}


void mainwindow::on_change_xy_kp()
{
	try {
		rpc_client_->call(firmware_rpcuri_, "kp", spinbutton_xy_kp_->get_value());
	} catch (std::exception& e) {
		std::cout << "on_change_xy_kp exception: " << e.what() << std::endl;
	}
}

void mainwindow::on_change_xy_ki()
{
	try {
		rpc_client_->call(firmware_rpcuri_, "ki", spinbutton_xy_ki_->get_value());
	} catch (std::exception& e) {
		std::cout << "on_change_xy_ki exception: " << e.what() << std::endl;
	}
}

void mainwindow::on_change_xy_kd()
{
	try {
		rpc_client_->call(firmware_rpcuri_, "kd", spinbutton_xy_kd_->get_value());
	} catch (std::exception& e) {
		std::cout << "on_change_xy_kd exception: " << e.what() << std::endl;
	}
}

void mainwindow::on_change_yaw_kp()
{
	try {
		rpc_client_->call(firmware_rpcuri_, "yaw_kp", spinbutton_z_kp_->get_value());
	} catch (std::exception& e) {
		std::cout << "on_change_yaw_kp exception: " << e.what() << std::endl;
	}
}

void mainwindow::on_change_yaw_ki()
{
	try {
		rpc_client_->call(firmware_rpcuri_, "yaw_ki", spinbutton_z_ki_->get_value());
	} catch (std::exception& e) {
		std::cout << "on_change_yaw_ki exception: " << e.what() << std::endl;
	}
}

void mainwindow::on_change_yaw_kd()
{
	try {
		rpc_client_->call(firmware_rpcuri_, "yaw_kd", spinbutton_z_kd_->get_value());
	} catch (std::exception& e) {
		std::cout << "on_change_yaw_kd exception: " << e.what() << std::endl;
	}
}

void mainwindow::on_change_acc_period()
{
	try {
		rpc_client_->call(firmware_rpcuri_, "sd_set_accelerometer_correction_period", spinbutton_acc_period_->get_value());
	} catch (std::exception& e) {
		std::cout << "on_change_acc_period exception: " << e.what() << std::endl;
	}
}

void mainwindow::on_change_gyro_factor()
{
	try {
		rpc_client_->call(firmware_rpcuri_, "sd_set_gyro_factor", spinbutton_gyro_factor_->get_value());
	} catch (std::exception& e) {
		std::cout << "on_change_gyro_factor exception: " << e.what() << std::endl;
	}
}

void mainwindow::on_change_yaw_factor()
{
	try {
		rpc_client_->call(firmware_rpcuri_, "sd_set_yaw_throttle_factor", spinbutton_yaw_throttle_factor_->get_value());
	} catch (std::exception& e) {
		std::cout << "on_change_yaw_factor exception: " << e.what() << std::endl;
	}
}

void mainwindow::on_change_yaw_bias()
{
	try {
		rpc_client_->call(firmware_rpcuri_, "yaw_bias", spinbutton_yaw_bias_->get_value());
	} catch (std::exception& e) {
		std::cout << "on_change_yaw_bias exception: " << e.what() << std::endl;
	}
}

void mainwindow::on_change_pitch_bias()
{
	try {
		rpc_client_->call(firmware_rpcuri_, "pitch_bias", spinbutton_pitch_bias_->get_value());
	} catch (std::exception& e) {
		std::cout << "on_change_pitch_bias exception: " << e.what() << std::endl;
	}
}

void mainwindow::on_change_roll_bias()
{
	try {
		rpc_client_->call(firmware_rpcuri_, "roll_bias", spinbutton_roll_bias_->get_value());
	} catch (std::exception& e) {
		std::cout << "on_change_roll_bias exception: " << e.what() << std::endl;
	}
}


void mainwindow::set_rpc_connection(const std::string& rpcserver, const std::string& rpcport, size_t updaterate)
{
	try {
		timer_conn_.disconnect();
		rpc_client_.reset(new rpc_client_http(rpcserver, rpcport, 30000, "2.0"));
		on_rpc_update();
		sigc::slot<bool> my_slot = sigc::mem_fun(*this, &mainwindow::on_rpc_update);
		timer_conn_ = Glib::signal_timeout().connect(my_slot, 1000/(updaterate + 1));
	} catch (std::exception& e) {
		std::cout << "set_rpc_connection exception: " << e.what() << std::endl;
	}
}

void mainwindow::rpc_update_attitude()
{
	try {
		QuaternionF q;
		q = quaternion_from_json_value<double>(drone_state_["attitude"]);
		label_attitude_w_->set_text(double_to_str(q.w));
		label_attitude_x_->set_text(double_to_str(q.x));
		label_attitude_y_->set_text(double_to_str(q.y));
		label_attitude_z_->set_text(double_to_str(q.z));
	} catch (std::exception& e) {
		std::cout << "rpc_update_attitude(sd_get_attitude) exception: " << e.what() << std::endl;
	}
}

void mainwindow::rpc_update_altitude()
{
	try {
		label_altitude_meters_->set_text(double_to_str(drone_state_["altitude_meters"].get_real()));
	} catch (std::exception& e) {
		std::cout << "rpc_update_altitude exception: " << e.what() << std::endl;
	}
}


void mainwindow::rpc_update_target()
{
	try {
		QuaternionF q;
		q = quaternion_from_json_value<double>(drone_state_["target"]);
		label_target_w_->set_text(double_to_str(q.w));
		label_target_x_->set_text(double_to_str(q.x));
		label_target_y_->set_text(double_to_str(q.y));
		label_target_z_->set_text(double_to_str(q.z));
	} catch (std::exception& e) {
		std::cout << "rpc_update_target exception: " << e.what() << std::endl;
	}
}

void mainwindow::rpc_update_twist()
{
	try {
		QuaternionF q;
		q = quaternion_from_json_value<double>(drone_state_["twist"]);
		label_twist_w_->set_text(double_to_str(q.w));
		label_twist_x_->set_text(double_to_str(q.x));
		label_twist_y_->set_text(double_to_str(q.y));
		label_twist_z_->set_text(double_to_str(q.z));
	} catch (std::exception& e) {
		std::cout << "rpc_update_twist exception: " << e.what() << std::endl;
	}
}

void mainwindow::rpc_update_motors()
{
	try {
		rexjson::value val = drone_state_["motors"];
		label_m1_->set_text(double_to_str(val.get_array().at(0).get_real()));
		label_m2_->set_text(double_to_str(val.get_array().at(1).get_real()));
		label_m3_->set_text(double_to_str(val.get_array().at(2).get_real()));
		label_m4_->set_text(double_to_str(val.get_array().at(3).get_real()));
	} catch (std::exception& e) {
		std::cout << "rpc_update_motors(sd_get_motors) exception: " << e.what() << std::endl;
	}
}

void mainwindow::rpc_update_correction_thurst()
{
	try {
		rexjson::value val = rpc_client_->call(rpcuri_, "sd_get_correction_thrust");
		spinbutton_m1_->set_value(val.get_array().at(0).get_real());
		spinbutton_m2_->set_value(val.get_array().at(1).get_real());
		spinbutton_m3_->set_value(val.get_array().at(2).get_real());
		spinbutton_m4_->set_value(val.get_array().at(3).get_real());
	} catch (std::exception& e) {
		std::cout << "rpc_update_correction_thurst(sd_get_correction_thrust) exception: " << e.what() << std::endl;
	}
}

void mainwindow::rpc_update_xy_pid()
{
	try {
		spinbutton_xy_kp_->set_value(drone_state_["kp"].get_real());
		spinbutton_xy_ki_->set_value(drone_state_["ki"].get_real());
		spinbutton_xy_kd_->set_value(drone_state_["kd"].get_real());
	} catch (std::exception& e) {
		std::cout << "rpc_update_xy_pid exception: " << e.what() << std::endl;
	}
}

void mainwindow::rpc_update_yaw_pid()
{
	try {
		spinbutton_z_kp_->set_value(drone_state_["yaw_kp"].get_real());
		spinbutton_z_ki_->set_value(drone_state_["yaw_ki"].get_real());
		spinbutton_z_kd_->set_value(drone_state_["yaw_kd"].get_real());
	} catch (std::exception& e) {
		std::cout << "rpc_update_xy_pid exception: " << e.what() << std::endl;
	}
}

void mainwindow::rpc_update_coefficients()
{
	try {
		spinbutton_acc_period_->set_value(drone_state_["accelerometer_correction_period"].get_real());
		spinbutton_gyro_factor_->set_value(drone_state_["gyro_factor"].get_real());
		spinbutton_yaw_throttle_factor_->set_value(drone_state_["yaw_throttle_factor"].get_real());
		spinbutton_yaw_bias_->set_value(drone_state_["yaw_bias"].get_real());
		spinbutton_pitch_bias_->set_value(drone_state_["pitch_bias"].get_real());
		spinbutton_roll_bias_->set_value(drone_state_["roll_bias"].get_real());

	} catch (std::exception& e) {
		std::cout << "rpc_update_coefficients exception: " << e.what() << std::endl;
	}

}

void mainwindow::rpc_update_accelerometer()
{
	try {
		rexjson::value val = drone_state_["accel"];
		Vector3f G = matrix_from_json_value<float, 3, 1>(val);
		label_accelerometer_x_->set_text(double_to_str(val.get_array().at(0).get_real()));
		label_accelerometer_y_->set_text(double_to_str(val.get_array().at(1).get_real()));
		label_accelerometer_z_->set_text(double_to_str(val.get_array().at(2).get_real()));
		label_accelerometer_length_->set_text(double_to_str(G.length()));
	} catch (std::exception& e) {
		std::cout << "rpc_update_accelerometer(sd_get_accelerometer) exception: " << e.what() << std::endl;
	}
}

void mainwindow::rpc_update_thrust()
{
}

void mainwindow::rpc_update_armed()
{
}

void mainwindow::rpc_update_controls()
{
	try {
		label_yaw_->set_text(double_to_str(drone_state_["yaw"].get_real()));
		label_pitch_->set_text(double_to_str(drone_state_["pitch"].get_real()));
		label_roll_->set_text(double_to_str(drone_state_["roll"].get_real()));
	} catch (std::exception& e) {
		std::cout << "rpc_update_controls exception: " << e.what() << std::endl;
	}
}

bool mainwindow::on_rpc_update()
{
	try {
		drone_state_ = rpc_client_->call(firmware_rpcuri_, "sd_get_dronestate");
		rpc_update_twist();
		rpc_update_armed();
		rpc_update_attitude();
		rpc_update_target();
		rpc_update_accelerometer();
		rpc_update_motors();
		rpc_update_thrust();
		rpc_update_xy_pid();
		rpc_update_yaw_pid();
		rpc_update_controls();
		rpc_update_coefficients();
		rpc_update_altitude();

	} catch (std::exception& e) {
		std::cout << "on_rpc_update exception: " << e.what() << std::endl;
	}
	return true;
}

std::string mainwindow::double_to_str(double val, size_t precision)
{
	std::stringstream oss;
	oss.setf(std::ios::fixed, std::ios::floatfield);
	oss.precision(precision);
	oss << val;
	return oss.str();
}
