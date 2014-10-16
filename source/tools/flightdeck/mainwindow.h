#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>


class mainwindow: public Gtk::Window
{
public:
	mainwindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
	virtual ~mainwindow();

protected:
	//Signal handlers:
	void on_button_quit();
	void on_button_lock_motors();
	void on_button_lock_g();

	Glib::RefPtr<Gtk::Builder> ref_glade_;
	Gtk::Button* button_quit_;
	Gtk::CheckButton* button_arm_motors_;
	Gtk::CheckButton* button_lock_motors_;
	Gtk::CheckButton* button_lock_g_;

};

#endif //MAINWINDOW_H
