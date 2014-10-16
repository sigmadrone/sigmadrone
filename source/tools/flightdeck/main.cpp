#include "mainwindow.h"
#include <iostream>

int main(int argc, char **argv)
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv,
			"com.sigmadrone.flightui");

	//Load the Glade file and instiate its widgets:
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
	try {
		refBuilder->add_from_file("mainwindow.xml");
	} catch (const Glib::FileError& ex) {
		std::cerr << "FileError: " << ex.what() << std::endl;
		return 1;
	} catch (const Glib::MarkupError& ex) {
		std::cerr << "MarkupError: " << ex.what() << std::endl;
		return 1;
	} catch (const Gtk::BuilderError& ex) {
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		return 1;
	}

	//Get the GtkBuilder-instantiated dialog::
	mainwindow* mainwin = 0;
	refBuilder->get_widget_derived("mainwindow", mainwin);
	if (mainwin) {
		//Start:
		app->run(*mainwin);
	}
	delete mainwin;
	return 0;
}
