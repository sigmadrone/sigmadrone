#include "controlwindow.h"
#include <iostream>
#include <cstdio>

controlwindow::controlwindow()
	: frame_earth_g_("Earth G")
	, frame_thrust_("Thrust")
	, vbox_main_(Gtk::ORIENTATION_VERTICAL, 5)
	, vbox_g_(Gtk::ORIENTATION_VERTICAL)
	, vbox_x_(Gtk::ORIENTATION_VERTICAL)
	, vbox_y_(Gtk::ORIENTATION_VERTICAL)
	, vbox_z_(Gtk::ORIENTATION_VERTICAL)
	, m_VBox_Accelerated(Gtk::ORIENTATION_VERTICAL)
	, m_VBox_Value(Gtk::ORIENTATION_VERTICAL)
	, m_VBox_Digits(Gtk::ORIENTATION_VERTICAL)
	, label_x_("X: ")
	, label_y_("Y: ")
	, label_z_("Z: ")
	, m_Label_Value("Value: ")
	, m_Label_Digits("Digits: ")
	, adjustment_x_(Gtk::Adjustment::create(0.0, -1.0, 1.0, 0.01, 0.05, 0.0))
	, adjustment_y_(Gtk::Adjustment::create(0.0, -1.0, 1.0, 0.01, 0.05, 0.0))
	, adjustment_z_(Gtk::Adjustment::create(1.0, -1.0, 1.0, 0.01, 0.05, 0.0))
	, m_adjustment_value(Gtk::Adjustment::create(0.0, -10000.0, 10000.0, 0.5, 100.0, 0.0))
	, m_adjustment_digits(Gtk::Adjustment::create(2.0, 1.0, 5.0, 1.0, 1.0, 0.0))
	, spinbutton_x_(adjustment_x_)
	, spinbutton_y_(adjustment_y_)
	, spinbutton_z_(adjustment_z_)
	, m_SpinButton_Value(m_adjustment_value, 1.0, 2)
	, m_SpinButton_Digits(m_adjustment_digits)
	, m_CheckButton_Snap("Snap to 0.5-ticks")
	, m_CheckButton_Numeric("Numeric only input mode")
	, m_Button_Int("Value as Int")
	, m_Button_Float("Value as Float")
	, m_Button_Close("Close")
{
	set_title("FD");
	set_position(Gtk::WIN_POS_CENTER);


	spinbutton_x_.set_digits(2);
	spinbutton_y_.set_digits(2);
	spinbutton_z_.set_digits(2);
	spinbutton_x_.set_numeric();
	spinbutton_y_.set_numeric();
	spinbutton_z_.set_numeric();

	vbox_main_.set_border_width(10);
	add(vbox_main_);

	vbox_main_.pack_start(frame_earth_g_);

	vbox_g_.set_border_width(5);
	frame_earth_g_.add(vbox_g_);

	/* X, Y, Z spinners */
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
	spinbutton_z_.set_size_request(55, -1);
	vbox_z_.pack_start(spinbutton_z_);
	hbox_earth_g_.pack_start(vbox_z_, Gtk::PACK_EXPAND_WIDGET, 5);

	//Accelerated:
	vbox_main_.pack_start(frame_thrust_);
	m_VBox_Accelerated.set_border_width(5);
	frame_thrust_.add(m_VBox_Accelerated);
	m_VBox_Accelerated.pack_start(hbox_thrust_, Gtk::PACK_EXPAND_WIDGET, 5);
	hbox_thrust_.pack_start(m_VBox_Value, Gtk::PACK_EXPAND_WIDGET, 5);
	m_Label_Value.set_alignment(Gtk::ALIGN_START);
	m_VBox_Value.pack_start(m_Label_Value);
	m_SpinButton_Value.set_wrap();
	m_SpinButton_Value.set_size_request(100, -1);
	m_VBox_Value.pack_start(m_SpinButton_Value);
	hbox_thrust_.pack_start(m_VBox_Digits, Gtk::PACK_EXPAND_WIDGET, 5);
	m_Label_Digits.set_alignment(Gtk::ALIGN_START);
	m_VBox_Digits.pack_start(m_Label_Digits);
	m_SpinButton_Digits.set_wrap();
	m_adjustment_digits->signal_value_changed().connect(
			sigc::mem_fun(*this, &controlwindow::on_spinbutton_digits_changed));
	m_VBox_Digits.pack_start(m_SpinButton_Digits);

	//CheckButtons:
	m_VBox_Accelerated.pack_start(m_CheckButton_Snap);
	m_CheckButton_Snap.set_active();
	m_CheckButton_Snap.signal_clicked().connect(
			sigc::mem_fun(*this, &controlwindow::on_checkbutton_snap));

	m_VBox_Accelerated.pack_start(m_CheckButton_Numeric);
	m_CheckButton_Numeric.set_active();
	m_CheckButton_Numeric.signal_clicked().connect(
			sigc::mem_fun(*this, &controlwindow::on_checkbutton_numeric));

	//Buttons:
	m_VBox_Accelerated.pack_start(hbox_buttons_, Gtk::PACK_SHRINK, 5);
	m_Button_Int.signal_clicked().connect(
			sigc::bind(sigc::mem_fun(*this, &controlwindow::on_button_getvalue),
					VALUE_FORMAT_INT));
	hbox_buttons_.pack_start(m_Button_Int, Gtk::PACK_EXPAND_WIDGET, 5);

	m_Button_Float.signal_clicked().connect(
			sigc::bind(sigc::mem_fun(*this, &controlwindow::on_button_getvalue),
					VALUE_FORMAT_FLOAT));
	hbox_buttons_.pack_start(m_Button_Float, Gtk::PACK_EXPAND_WIDGET, 5);

	m_VBox_Accelerated.pack_start(m_Label_ShowValue);
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

void controlwindow::on_checkbutton_snap()
{
  m_SpinButton_Value.set_snap_to_ticks( m_CheckButton_Snap.get_active() );
}

void controlwindow::on_checkbutton_numeric()
{
  m_SpinButton_Value.set_numeric( m_CheckButton_Numeric.get_active() );
}

void controlwindow::on_spinbutton_digits_changed()
{
  m_SpinButton_Value.set_digits( m_SpinButton_Digits.get_value_as_int() );
}

void controlwindow::on_button_getvalue(enumValueFormats display)
{
  gchar buf[32];

  if (display == VALUE_FORMAT_INT)
    sprintf (buf, "%d", m_SpinButton_Value.get_value_as_int());
  else
    sprintf (buf, "%0.*f", m_SpinButton_Value.get_digits(),
            m_SpinButton_Value.get_value());

  m_Label_ShowValue.set_text(buf);
}
