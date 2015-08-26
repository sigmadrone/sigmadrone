#include "mainwindow.h"
#include "libcmdargs/cmdargs.h"
#include <iostream>

static cmd_arg_spec g_argspec[] = {
		{"help",			"h",	"Display this help", CMD_ARG_BOOL},
		{"rpcserver",		"",		"RPC server name", CMD_ARG_STRING},
		{"rpcport",			"",		"RPC server port. Default: 18222", CMD_ARG_STRING},
		{"update-rate",		"",		"UI update frequency. Default: 30 Hz", CMD_ARG_STRING},
};


int main(int argc, char **argv)
{
	cmd_args args;
	std::string rpcserver;
	std::string rpcport;

	try {
		args.add_specs(g_argspec, sizeof(g_argspec)/sizeof(*g_argspec));
		args.parse_command_line(argc, (const char**)argv);
		if (!args.get_value("help").empty()) {
			std::cout << argv[0] << " <options>" << std::endl;
			std::cout << args.get_help_message() << std::endl;
			return 0;
		}
		rpcserver = args.get_value("rpcserver");
		rpcport = args.get_value("rpcport", "18222");
	} catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("com.sigmadrone.flightui");
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();

	try {
		refBuilder->add_from_file("/home/mstoilov/workspace-yocto/sigmadrone/source/tools/flightdeck/mainwindow.xml");
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
	mainwindow *mainwin;
	refBuilder->get_widget_derived("mainwindow", mainwin);
	if (mainwin) {
		if (!rpcserver.empty()) {
			mainwin->set_rpc_connection(rpcserver, rpcport, atol(args.get_value("update-rate", "10").c_str()));
		}

		//Start:
		app->run(*mainwin, 0, NULL);
	}
	mainwin->timer_conn_.disconnect();
	delete mainwin;
	return 0;
}
