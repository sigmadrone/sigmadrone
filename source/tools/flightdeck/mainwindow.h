#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>
#include "libjsonspirit/rpcclient.h"
#include "libjsonspirit/jsonserialization.h"


class mainwindow: public Gtk::Window
{
public:
	mainwindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
	virtual ~mainwindow();
	void set_rpc_connection(const std::string& rpcserver, const std::string& port, size_t updaterate);

protected:
	//Signal handlers:
	void on_button_quit();
	void on_button_arm_motors();
	void on_button_lock_motors();
	void on_button_lock_g();
	void on_change_thrust();
	void on_change_g();
	void on_change_correction_thrust();
	bool on_rpc_update();
	void rpc_update_attitude();
	void rpc_update_motors();
	void rpc_update_g();
	void rpc_update_correction_thurst();
	void rpc_update_accelerometer();
	void rpc_update_thrust();
	void rpc_update_armed();
	std::string double_to_str(double val, size_t precision = 3);

	sigc::connection timer_conn_;
	int timer_number_;
	std::string rpcuri_;
	boost::scoped_ptr<rpc_client> rpc_client_;
	Glib::RefPtr<Gtk::Builder> ref_glade_;
	Gtk::Button* button_quit_;
	Gtk::CheckButton* button_arm_motors_;
	Gtk::CheckButton* button_lock_motors_;
	Gtk::SpinButton* spinbutton_thrust_;
	Gtk::SpinButton* spinbutton_g_x_;
	Gtk::SpinButton* spinbutton_g_y_;
	Gtk::SpinButton* spinbutton_g_z_;
	Gtk::SpinButton* spinbutton_m1_;
	Gtk::SpinButton* spinbutton_m2_;
	Gtk::SpinButton* spinbutton_m3_;
	Gtk::SpinButton* spinbutton_m4_;
	Gtk::CheckButton* button_lock_g_;
	Gtk::Label *label_m1_;
	Gtk::Label *label_m2_;
	Gtk::Label *label_m3_;
	Gtk::Label *label_m4_;
	Gtk::Label *label_attitude_w_;
	Gtk::Label *label_attitude_x_;
	Gtk::Label *label_attitude_y_;
	Gtk::Label *label_attitude_z_;
	Gtk::Label *label_accelerometer_x_;
	Gtk::Label *label_accelerometer_y_;
	Gtk::Label *label_accelerometer_z_;

};

#endif //MAINWINDOW_H
