#include <iostream>
#include "controlwindow.h"


int main(int argc, char *argv[])
{
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.sigmadrone");

  controlwindow window;

  //Shows the window and returns when it is closed.
  return app->run(window);
}
