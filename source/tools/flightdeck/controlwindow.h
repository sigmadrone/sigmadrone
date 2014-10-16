#ifndef _CONTROLWINDOW_H_
#define _CONTROLWINDOW_H_

#include <gtkmm.h>


class controlwindow : public Gtk::Window
{
public:
	controlwindow();
	virtual ~controlwindow();

protected:
	//Signal handlers:
	void on_lock_motors();
	void on_arm_motors();
	void on_spinbutton_digits_changed();
	void on_button_close();

	enum enumValueFormats {
		VALUE_FORMAT_INT, VALUE_FORMAT_FLOAT
	};
	void on_button_getvalue(enumValueFormats display);

	//Child widgets:
	Gtk::Frame frame_earth_g_, frame_motors_;
	Gtk::Box hbox_earth_g_, hbox_motors_, hbox_buttons_;
	Gtk::Box vbox_main_, vbox_g_, vbox_x_, vbox_y_, vbox_z_;
	Gtk::Box vbox_motors_, vbox_thrust_, m_VBox_Digits;
	Gtk::Grid grid_m4_;
	Gtk::Label label_m1_, label_m2_, label_m3_, label_m4_;
	Gtk::Label label_x_, label_y_, label_z_, label_thrust_, m_Label_Digits,
			m_Label_ShowValue;
	Glib::RefPtr<Gtk::Adjustment> adjustment_x_, adjustment_y_, adjustment_z_;
	Glib::RefPtr<Gtk::Adjustment> m_adjustment_thrust_;
	Gtk::SpinButton spinbutton_x_, spinbutton_y_, spinbutton_z_;
	Gtk::SpinButton spinbutton_thrust_;
	Gtk::CheckButton checkbutton_lock_, checkbutton_arm_;
	Gtk::Button m_Button_Close;
};

#endif
