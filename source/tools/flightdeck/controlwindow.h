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
	void on_checkbutton_snap();
	void on_checkbutton_numeric();
	void on_spinbutton_digits_changed();
	void on_button_close();

	enum enumValueFormats {
		VALUE_FORMAT_INT, VALUE_FORMAT_FLOAT
	};
	void on_button_getvalue(enumValueFormats display);

	//Child widgets:
	Gtk::Frame frame_earth_g_, frame_thrust_;
	Gtk::Box hbox_earth_g_, hbox_thrust_, hbox_buttons_;
	Gtk::Box vbox_main_, vbox_g_, vbox_x_, vbox_y_, vbox_z_;
	Gtk::Box m_VBox_Accelerated, m_VBox_Value, m_VBox_Digits;
	Gtk::Label label_x_, label_y_, label_z_, m_Label_Value, m_Label_Digits,
			m_Label_ShowValue;
	Glib::RefPtr<Gtk::Adjustment> adjustment_x_, adjustment_y_, adjustment_z_;
	Glib::RefPtr<Gtk::Adjustment> m_adjustment_value, m_adjustment_digits;
	Gtk::SpinButton spinbutton_x_, spinbutton_y_, spinbutton_z_;
	Gtk::SpinButton m_SpinButton_Value, m_SpinButton_Digits;
	Gtk::CheckButton m_CheckButton_Snap, m_CheckButton_Numeric;
	Gtk::Button m_Button_Int, m_Button_Float, m_Button_Close;
};

#endif
