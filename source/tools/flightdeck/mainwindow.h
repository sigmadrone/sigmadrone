#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>
#include <vector>
#include "rpcclienthttp.h"
#include "jsonserialization.h"


class mainwindow: public Gtk::Window
{
public:
	mainwindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
	virtual ~mainwindow();
	void set_rpc_connection(const std::string& rpcserver, const std::string& port, size_t updaterate);

	sigc::connection timer_conn_;
protected:
	//Signal handlers:
	void on_button_quit();
	void on_button_arm_motors();
	void on_button_lock_motors();
	void on_change_thrust();
	void on_change_correction_thrust();
	bool on_rpc_update();
	void rpc_update_attitude();
	void rpc_update_target();
	void rpc_update_twist();
	void rpc_update_motors();
	void rpc_update_coefficients();
	void rpc_update_correction_thurst();
	void rpc_update_accelerometer();
	void rpc_update_thrust();
	void rpc_update_armed();
	void rpc_update_controls();
	void rpc_update_xy_pid();
	void rpc_update_yaw_pid();
	void on_change_xy_kp();
	void on_change_xy_ki();
	void on_change_xy_kd();
	void on_change_yaw_kp();
	void on_change_yaw_ki();
	void on_change_yaw_kd();
	void on_change_acc_period();
	void on_change_gyro_factor();
	void on_change_yaw_factor();

	std::string double_to_str(double val, size_t precision = 3);

	rexjson::value drone_state_;
	int timer_number_;
	std::string rpcuri_;
	std::string firmware_rpcuri_;
	boost::scoped_ptr<rpc_client_http> rpc_client_;
	Glib::RefPtr<Gtk::Builder> ref_glade_;
	Gtk::Button* button_quit_;
	Gtk::CheckButton* button_arm_motors_;
	Gtk::CheckButton* button_lock_motors_;
	Gtk::SpinButton* spinbutton_thrust_;
	Gtk::SpinButton* spinbutton_m1_;
	Gtk::SpinButton* spinbutton_m2_;
	Gtk::SpinButton* spinbutton_m3_;
	Gtk::SpinButton* spinbutton_m4_;
	Gtk::SpinButton* spinbutton_acc_period_;
	Gtk::SpinButton* spinbutton_gyro_factor_;
	Gtk::SpinButton* spinbutton_yaw_throttle_factor_;
	Gtk::Label *label_m1_;
	Gtk::Label *label_m2_;
	Gtk::Label *label_m3_;
	Gtk::Label *label_m4_;
	Gtk::Label *label_attitude_w_;
	Gtk::Label *label_attitude_x_;
	Gtk::Label *label_attitude_y_;
	Gtk::Label *label_attitude_z_;
	Gtk::Label *label_target_w_;
	Gtk::Label *label_target_x_;
	Gtk::Label *label_target_y_;
	Gtk::Label *label_target_z_;
	Gtk::Label *label_accelerometer_x_;
	Gtk::Label *label_accelerometer_y_;
	Gtk::Label *label_accelerometer_z_;
	Gtk::Label *label_accelerometer_length_;
	Gtk::Label *label_twist_w_;
	Gtk::Label *label_twist_x_;
	Gtk::Label *label_twist_y_;
	Gtk::Label *label_twist_z_;
	Gtk::SpinButton* spinbutton_xy_kp_;
	Gtk::SpinButton* spinbutton_xy_ki_;
	Gtk::SpinButton* spinbutton_xy_kd_;
	Gtk::SpinButton* spinbutton_z_kp_;
	Gtk::SpinButton* spinbutton_z_ki_;
	Gtk::SpinButton* spinbutton_z_kd_;
	Gtk::Label *label_yaw_;
	Gtk::Label *label_pitch_;
	Gtk::Label *label_roll_;

	std::vector<sigc::connection> connections_;
};

#endif //MAINWINDOW_H
