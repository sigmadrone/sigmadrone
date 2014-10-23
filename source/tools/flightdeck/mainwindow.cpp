#include <boost/lexical_cast.hpp>
#include "mainwindow.h"

mainwindow::mainwindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
	: Gtk::Window(cobject)
	, timer_number_(0)
	, rpcuri_("/jsonrpc")
	, rpc_client_(new rpc_client("localhost", "18222"))
	, ref_glade_(refGlade)
	, button_quit_(NULL)
	, button_arm_motors_(NULL)
	, button_lock_motors_(NULL)
	, spinbutton_thrust_(NULL)
	, spinbutton_g_x_(NULL)
	, spinbutton_g_y_(NULL)
	, spinbutton_g_z_(NULL)
	, spinbutton_m1_(NULL)
	, spinbutton_m2_(NULL)
	, spinbutton_m3_(NULL)
	, spinbutton_m4_(NULL)
	, button_lock_g_(NULL)
	, label_m1_(NULL)
	, label_m2_(NULL)
	, label_m3_(NULL)
	, label_m4_(NULL)
	, label_attitude_w_(NULL)
	, label_attitude_x_(NULL)
	, label_attitude_y_(NULL)
	, label_attitude_z_(NULL)
	, label_accelerometer_x_(NULL)
	, label_accelerometer_y_(NULL)
	, label_accelerometer_z_(NULL)
{
	//Get the Glade-instantiated Button, and connect a signal handler:
	ref_glade_->get_widget("button_quit", button_quit_);
	ref_glade_->get_widget("checkbutton_arm_motors", button_arm_motors_);
	ref_glade_->get_widget("checkbutton_lock_motors", button_lock_motors_);
	ref_glade_->get_widget("spinbutton_thrust", spinbutton_thrust_);
	ref_glade_->get_widget("spinbutton_g_x", spinbutton_g_x_);
	ref_glade_->get_widget("spinbutton_g_y", spinbutton_g_y_);
	ref_glade_->get_widget("spinbutton_g_z", spinbutton_g_z_);
	ref_glade_->get_widget("spinbutton_m1", spinbutton_m1_);
	ref_glade_->get_widget("spinbutton_m2", spinbutton_m2_);
	ref_glade_->get_widget("spinbutton_m3", spinbutton_m3_);
	ref_glade_->get_widget("spinbutton_m4", spinbutton_m4_);
	ref_glade_->get_widget("checkbutton_lock_g", button_lock_g_);
	ref_glade_->get_widget("label_m1", label_m1_);
	ref_glade_->get_widget("label_m2", label_m2_);
	ref_glade_->get_widget("label_m3", label_m3_);
	ref_glade_->get_widget("label_m4", label_m4_);
	ref_glade_->get_widget("label_attitude_w", label_attitude_w_);
	ref_glade_->get_widget("label_attitude_x", label_attitude_x_);
	ref_glade_->get_widget("label_attitude_y", label_attitude_y_);
	ref_glade_->get_widget("label_attitude_z", label_attitude_z_);
	ref_glade_->get_widget("label_accelerometer_x", label_accelerometer_x_);
	ref_glade_->get_widget("label_accelerometer_y", label_accelerometer_y_);
	ref_glade_->get_widget("label_accelerometer_z", label_accelerometer_z_);

	button_quit_->signal_clicked().connect(sigc::mem_fun(*this, &mainwindow::on_button_quit));
	button_lock_motors_->signal_clicked().connect(sigc::mem_fun(*this, &mainwindow::on_button_lock_motors));
	button_arm_motors_->signal_clicked().connect(sigc::mem_fun(*this, &mainwindow::on_button_arm_motors));
	button_lock_g_->signal_clicked().connect(sigc::mem_fun(*this, &mainwindow::on_button_lock_g));
	spinbutton_g_x_->signal_value_changed().connect(sigc::mem_fun(*this, &mainwindow::on_change_g));
	spinbutton_g_y_->signal_value_changed().connect(sigc::mem_fun(*this, &mainwindow::on_change_g));
	spinbutton_g_z_->signal_value_changed().connect(sigc::mem_fun(*this, &mainwindow::on_change_g));
	spinbutton_thrust_->signal_value_changed().connect(sigc::mem_fun(*this, &mainwindow::on_change_thrust));
}

mainwindow::~mainwindow()
{
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
	try {
		spinbutton_thrust_->set_value(0.0);
		on_change_thrust();
		if (button_arm_motors_->get_active()) {
			rpc_client_->call(rpcuri_, "sd_run");
		} else {
			rpc_client_->call(rpcuri_, "sd_reset");
		}
		rpc_update_armed();
	} catch (std::exception& e) {
		std::cout << "on_change_g exception: " << e.what() << std::endl;
	}
}


void mainwindow::on_button_lock_g()
{
	spinbutton_g_x_->set_sensitive(!button_lock_g_->get_active());
	spinbutton_g_y_->set_sensitive(!button_lock_g_->get_active());
	spinbutton_g_z_->set_sensitive(!button_lock_g_->get_active());
}

void mainwindow::on_change_thrust()
{
	try {
		rpc_client_->call(rpcuri_, "sd_set_thrust", spinbutton_thrust_->get_value());
		rpc_update_thrust();
	} catch (std::exception& e) {
		std::cout << "on_change_thrust exception: " << e.what() << std::endl;
	}
}


void mainwindow::on_change_g()
{
	try {
		Vector3d G(spinbutton_g_x_->get_value(), spinbutton_g_y_->get_value(), spinbutton_g_z_->get_value());
		rpc_client_->call(rpcuri_, "sd_set_earth_g_vector", matrix_to_json_value(G));
		rpc_update_g();
	} catch (std::exception& e) {
		std::cout << "on_change_g exception (sd_set_earth_g_vector): " << e.what() << std::endl;
	}
}

void mainwindow::on_change_motors()
{
	try {
		Vector4d motors(
				spinbutton_m1_->get_value(),
				spinbutton_m2_->get_value(),
				spinbutton_m3_->get_value(),
				spinbutton_m4_->get_value());
		rpc_client_->call(rpcuri_, "sd_set_correction_thurst", matrix_to_json_value(motors));
		rpc_update_correction_thurst();
	} catch (std::exception& e) {
		std::cout << "on_change_motors (sd_set_correction_thurst) exception: " << e.what() << std::endl;
	}
}

void mainwindow::set_rpc_connection(const std::string& rpcserver, const std::string& rpcport, size_t updaterate)
{
	try {
		timer_conn_.disconnect();
		rpc_client_.reset(new rpc_client(rpcserver, rpcport, 30000, "2.0"));
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
		QuaternionD q;
		json::value val = rpc_client_->call(rpcuri_, "sd_get_attitude");
		q = quaternion_from_json_value<double>(val);
		label_attitude_w_->set_text(double_to_str(q.w));
		label_attitude_x_->set_text(double_to_str(q.x));
		label_attitude_y_->set_text(double_to_str(q.y));
		label_attitude_z_->set_text(double_to_str(q.z));
	} catch (std::exception& e) {
		std::cout << "rpc_update_attitude(sd_get_attitude) exception: " << e.what() << std::endl;
	}
}

void mainwindow::rpc_update_motors()
{
	try {
		json::value val = rpc_client_->call(rpcuri_, "sd_get_motors");
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
		json::value val = rpc_client_->call(rpcuri_, "sd_get_correction_thrust");
		spinbutton_m1_->set_value(val.get_array().at(0).get_real());
		spinbutton_m2_->set_value(val.get_array().at(1).get_real());
		spinbutton_m3_->set_value(val.get_array().at(2).get_real());
		spinbutton_m4_->set_value(val.get_array().at(3).get_real());
	} catch (std::exception& e) {
		std::cout << "rpc_update_correction_thurst(sd_get_correction_thrust) exception: " << e.what() << std::endl;
	}
}


void mainwindow::rpc_update_g()
{
	try {
		Vector3d G = matrix_from_json_value<double, 3, 1>(rpc_client_->call(rpcuri_, "sd_get_earth_g_vector"));
		spinbutton_g_x_->set_value(G.at(0, 0));
		spinbutton_g_y_->set_value(G.at(1, 0));
		spinbutton_g_z_->set_value(G.at(2, 0));
	} catch (std::exception& e) {
		std::cout << "rpc_update_g(sd_get_earth_g_vector) exception: " << e.what() << std::endl;
	}
}

void mainwindow::rpc_update_accelerometer()
{
	try {
		json::value val = rpc_client_->call(rpcuri_, "sd_get_accelerometer");
		label_accelerometer_x_->set_text(double_to_str(val.get_array().at(0).get_real()));
		label_accelerometer_y_->set_text(double_to_str(val.get_array().at(1).get_real()));
		label_accelerometer_z_->set_text(double_to_str(val.get_array().at(2).get_real()));
	} catch (std::exception& e) {
		std::cout << "rpc_update_accelerometer(sd_get_accelerometer) exception: " << e.what() << std::endl;
	}
}

void mainwindow::rpc_update_thrust()
{
	try {
		spinbutton_thrust_->set_value(rpc_client_->call(rpcuri_, "sd_get_thrust").get_real());
	} catch (std::exception& e) {
		std::cout << "rpc_update_thrust(sd_get_thrust) exception: " << e.what() << std::endl;
	}
}

void mainwindow::rpc_update_armed()
{
	try {
		button_arm_motors_->set_active(rpc_client_->call(rpcuri_, "sd_get_running").get_bool());
	} catch (std::exception& e) {
		std::cout << "rpc_update_armed(sd_get_running) exception: " << e.what() << std::endl;
	}

}

bool mainwindow::on_rpc_update()
{
	try {
		rpc_update_armed();
		rpc_update_attitude();
		rpc_update_accelerometer();
		rpc_update_motors();
		rpc_update_g();
		rpc_update_thrust();
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
