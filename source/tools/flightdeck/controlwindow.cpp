#include "controlwindow.h"
#include <iostream>
#include <cstdio>

controlwindow::controlwindow()
	: frame_earth_g_(" Direction of Earth G ")
	, frame_motors_(" Motors ")
	, vbox_main_(Gtk::ORIENTATION_VERTICAL, 5)
	, vbox_g_(Gtk::ORIENTATION_VERTICAL)
	, vbox_x_(Gtk::ORIENTATION_VERTICAL)
	, vbox_y_(Gtk::ORIENTATION_VERTICAL)
	, vbox_z_(Gtk::ORIENTATION_VERTICAL)
	, vbox_motors_(Gtk::ORIENTATION_VERTICAL)
	, vbox_thrust_(Gtk::ORIENTATION_VERTICAL)
	, m_VBox_Digits(Gtk::ORIENTATION_VERTICAL)
	, grid_m4_()
	, label_m1_("0.0")
	, label_m2_("0.0")
	, label_m3_("0.0")
	, label_m4_("0.0")
	, label_x_("X: ")
	, label_y_("Y: ")
	, label_z_("Z: ")
	, label_thrust_("Thrust: ")
	, m_Label_Digits("Digits: ")
	, adjustment_x_(Gtk::Adjustment::create(0.0, -1.0, 1.0, 0.01, 0.05, 0.0))
	, adjustment_y_(Gtk::Adjustment::create(0.0, -1.0, 1.0, 0.01, 0.05, 0.0))
	, adjustment_z_(Gtk::Adjustment::create(1.0, -1.0, 1.0, 0.01, 0.05, 0.0))
	, m_adjustment_thrust_(Gtk::Adjustment::create(0.0, 0.0, 1.0, 0.01, 0.05, 0.0))
	, spinbutton_x_(adjustment_x_, 0.01, 2)
	, spinbutton_y_(adjustment_y_, 0.01, 2)
	, spinbutton_z_(adjustment_z_, 0.01, 2)
	, spinbutton_thrust_(m_adjustment_thrust_, 0.01, 2)
	, checkbutton_lock_("Lock")
	, checkbutton_arm_("Arm")
	, m_Button_Close("Close")
{
	set_title("FD");
	set_position(Gtk::WIN_POS_CENTER);

	spinbutton_x_.set_numeric();
	spinbutton_y_.set_numeric();
	spinbutton_z_.set_numeric();
	spinbutton_thrust_.set_numeric();
	spinbutton_x_.set_size_request(120, -1);
	spinbutton_y_.set_size_request(120, -1);
	spinbutton_z_.set_size_request(120, -1);
	spinbutton_thrust_.set_size_request(120, -1);

	vbox_main_.set_border_width(10);
	add(vbox_main_);


	//Motors:

	//CheckButtons:
	vbox_motors_.pack_start(checkbutton_arm_);
	checkbutton_arm_.set_active(false);
	checkbutton_arm_.signal_clicked().connect(
			sigc::mem_fun(*this, &controlwindow::on_arm_motors));

	vbox_motors_.pack_start(checkbutton_lock_);
	checkbutton_lock_.set_active(false);
	checkbutton_lock_.signal_clicked().connect(
			sigc::mem_fun(*this, &controlwindow::on_lock_motors));

	vbox_main_.pack_start(frame_motors_);
	vbox_motors_.set_border_width(5);
	frame_motors_.add(vbox_motors_);
	vbox_motors_.pack_start(hbox_motors_, Gtk::PACK_EXPAND_WIDGET, 5);
	hbox_motors_.pack_start(vbox_thrust_, Gtk::PACK_EXPAND_WIDGET, 5);
	label_thrust_.set_alignment(Gtk::ALIGN_START);
	vbox_thrust_.pack_start(label_thrust_);
	vbox_thrust_.pack_start(spinbutton_thrust_);
	hbox_motors_.pack_start(m_VBox_Digits, Gtk::PACK_EXPAND_WIDGET, 5);
	m_Label_Digits.set_alignment(Gtk::ALIGN_START);
	m_VBox_Digits.pack_start(m_Label_Digits);


	/* Earth G: (X, Y, Z) spinners */
	vbox_main_.pack_start(frame_earth_g_);
	vbox_g_.set_border_width(5);
	frame_earth_g_.add(vbox_g_);
	vbox_g_.pack_start(hbox_earth_g_, Gtk::PACK_EXPAND_WIDGET, 5);
	label_x_.set_alignment(Gtk::ALIGN_START);
	vbox_x_.pack_start(label_x_);
	vbox_x_.pack_start(spinbutton_x_);
	hbox_earth_g_.pack_start(vbox_x_, Gtk::PACK_EXPAND_WIDGET, 5);
	label_y_.set_alignment(Gtk::ALIGN_START);
	vbox_y_.pack_start(label_y_);
	vbox_y_.pack_start(spinbutton_y_);
	hbox_earth_g_.pack_start(vbox_y_, Gtk::PACK_EXPAND_WIDGET, 5);
	label_z_.set_alignment(Gtk::ALIGN_START);
	vbox_z_.pack_start(label_z_);
	vbox_z_.pack_start(spinbutton_z_);
	hbox_earth_g_.pack_start(vbox_z_, Gtk::PACK_EXPAND_WIDGET, 5);


	//Buttons:
	vbox_motors_.pack_start(hbox_buttons_, Gtk::PACK_SHRINK, 5);

	vbox_motors_.pack_start(m_Label_ShowValue);
	m_Label_ShowValue.set_text("0");

	//Close button:
	m_Button_Close.signal_clicked().connect(
			sigc::mem_fun(*this, &controlwindow::on_button_close));
	vbox_main_.pack_start(m_Button_Close, Gtk::PACK_SHRINK);

	show_all_children();
}

controlwindow::~controlwindow()
{
}


void controlwindow::on_button_close()
{
	hide();
}

void controlwindow::on_lock_motors()
{
	checkbutton_arm_.set_sensitive(!checkbutton_lock_.get_active());
}

void controlwindow::on_arm_motors()
{
	if (checkbutton_arm_.get_active()) {
		std::cout << "enabled" << std::endl;
	} else {
		std::cout << "disabled" << std::endl;
	}
}

void controlwindow::on_spinbutton_digits_changed()
{

}

void controlwindow::on_button_getvalue(enumValueFormats display)
{
  gchar buf[32];

  if (display == VALUE_FORMAT_INT)
    sprintf (buf, "%d", spinbutton_thrust_.get_value_as_int());
  else
    sprintf (buf, "%0.*f", spinbutton_thrust_.get_digits(),
            spinbutton_thrust_.get_value());

  m_Label_ShowValue.set_text(buf);
}
