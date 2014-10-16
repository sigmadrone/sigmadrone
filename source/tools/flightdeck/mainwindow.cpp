#include "mainwindow.h"

mainwindow::mainwindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
	: Gtk::Window(cobject)
	, ref_glade_(refGlade)
	, button_quit_(NULL)
	, button_arm_motors_(NULL)
	, button_lock_motors_(NULL)
	, button_lock_g_(NULL)

{
	//Get the Glade-instantiated Button, and connect a signal handler:
	ref_glade_->get_widget("button_quit", button_quit_);
	ref_glade_->get_widget("checkbutton_arm_motors", button_arm_motors_);
	ref_glade_->get_widget("checkbutton_lock_motors", button_lock_motors_);
	ref_glade_->get_widget("checkbutton_lock_g", button_lock_g_);

	button_quit_->signal_clicked().connect(sigc::mem_fun(*this, &mainwindow::on_button_quit));
	button_lock_motors_->signal_clicked().connect(sigc::mem_fun(*this, &mainwindow::on_button_lock_motors));
	button_lock_g_->signal_clicked().connect(sigc::mem_fun(*this, &mainwindow::on_button_lock_g));
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

void mainwindow::on_button_lock_g()
{
}
