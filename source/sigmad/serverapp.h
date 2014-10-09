#ifndef SERVERAPP_H_
#define SERVERAPP_H_

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include "libcmdargs/cmdargs.h"
#include "liblogger/logfile.h"
#include "liblogger/logger.h"
#include "libdevice/pca9685controller.h"
#include "libdevice/sampler.h"
#include "libattitude/attitudetracker.h"

#include "userrpcserver.h"
#include "attcontroller.h"


class server_app : private boost::noncopyable, public logger<logfile>
{
public:
	server_app(const cmd_args& args = cmd_args());
	~server_app();
	int run(int argc, const char *argv[]);
	void stop();

public:
	boost::scoped_ptr<user_rpcserver> user_rpcserver_;
	attcontroller ctrl_thread_;
	boost::scoped_ptr<servocontroller> servoctrl_;
	boost::scoped_ptr<sampler> ssampler_;
	boost::scoped_ptr<attitudetracker> attitude_tracker_;

protected:
	void signal_handler_terminate();
	void usage();
	void init_user_rpcserver();
	void init_servo_controller();
	void init_sensors_sampler();
	void init_attitude_tracker();
	void hwctrl_thread_worker();

	boost::shared_ptr<boost::thread> hwctrl_thread_;
	boost::asio::io_service io_service_;
	boost::asio::signal_set signals_;
	const cmd_args& args_;
};

#endif
