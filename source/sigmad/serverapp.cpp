#include <cstring>
#include <iostream>
#include <syslog.h>
#include <boost/algorithm/string.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include "libhttp/base64.hpp"
#include "serverapp.h"
#include "daemon.h"

server_app::server_app(const cmd_args& args)
	: logger("server_app: ")
	, logfile_(new logfile("sigmad.log", 1024*1024*10, logfile::none))
	, ctrl_thread_(*this)
	, io_service_()
	, signals_(io_service_)
	, args_(args)
{
	set_log_file(logfile_);
	// Register to handle the signals that indicate when the server should exit.
	// It is safe to register for the same signal multiple times in a program,
	// provided all registration for the specified signal is made through Asio.
	signals_.add(SIGINT);
	signals_.add(SIGTERM);
	signals_.add(SIGHUP);
#if defined(SIGQUIT)
	signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
	signals_.async_wait(boost::bind(&server_app::signal_handler_terminate, this));
}

server_app::~server_app()
{

}

void server_app::signal_handler_terminate()
{
	user_rpcserver_->stop(100);
	io_service_.stop();
	log_info_message("Received terminate signal.");
}

void server_app::stop()
{
	/*
	 * This will cause the signal_handler_terminate
	 * to be called.
	 */
	signals_.cancel();
}


void server_app::init_user_rpcserver()
{
	user_rpcserver_.reset(new user_rpcserver(
			*this,
			io_service_,
			args_.get_value("rpcconnect", "0.0.0.0"),
			args_.get_value("rpcport", "18222")));
	user_rpcserver_->set_log_file(logfile_);
	log_info_message("Starting user rpc server.");

	/*
	 * Use the global log file to log the RPC server messages.
	 */
	user_rpcserver_->set_log_file(logfile_);
}

void server_app::init_servo_controller()
{
	if (args_.get_value("servo-ctrl") == "pca9685") {
		servoctrl_.reset(new pca9685controller(4));
	} else {
		servoctrl_.reset(new servocontroller(4));
	}
	servoctrl_->motor(0) = servomotor(1.0, 1.30, 1.1, 2.2);
	servoctrl_->motor(1) = servomotor(1.0, 1.30, 1.1, 2.2);
	servoctrl_->motor(2) = servomotor(1.0, 1.30, 1.1, 2.2);
	servoctrl_->motor(3) = servomotor(1.0, 1.30, 1.1, 2.2);
	servoctrl_->setrate(300);
	servoctrl_->update();
}

void server_app::init_sensors_sampler()
{
	if (args_.get_value("disable-sensors").empty()) {
		ssampler_.reset(new sampler(
			args_.get_value("gyr-device", args_.get_value("gyr-disable").empty() ? "/dev/gyro0" : ""),
			args_.get_value("acc-device", args_.get_value("acc-disable").empty() ? "/dev/accel0" : ""),
			args_.get_value("mag-device", args_.get_value("mag-disable").empty() ? "/dev/mag0" : ""),
			args_.get_value("bar-device", args_.get_value("bar-disable").empty() ?"/sys/bus/i2c/devices/4-0077/pressure0_input" : "")));
		if (args_.get_value("gyr-disable").empty())
			ssampler_->gyr_.set_rate(atoi(args_.get_value("gyr-rate", "760").c_str()));
		if (args_.get_value("acc-disable").empty())
			ssampler_->acc_.set_rate(atoi(args_.get_value("acc-rate", "400").c_str()));
		if (args_.get_value("mag-disable").empty())
			ssampler_->mag_.set_rate(atoi(args_.get_value("mag-rate", "220").c_str()));
		if (args_.get_value("gyr-disable").empty())
			ssampler_->gyr_.set_full_scale(atoi(args_.get_value("gyr-scale", "2000").c_str()));
		if (args_.get_value("acc-disable").empty())
			ssampler_->acc_.set_full_scale(atoi(args_.get_value("acc-scale", "4").c_str()));
		if (args_.get_value("mag-disable").empty())
			ssampler_->mag_.set_full_scale(atoi(args_.get_value("mag-scale", "1300").c_str()));
		if (args_.get_value("gyr-disable").empty())
			ssampler_->gyr_.set_fifo_threshold(atoi(args_.get_value("gyr-fifo", "4").c_str()));
		if (args_.get_value("acc-disable").empty())
			ssampler_->acc_.set_fifo_threshold(atoi(args_.get_value("acc-fifo", "15").c_str()));
		ssampler_->gyr_.set_adjustment(atof(args_.get_value("gyr-adjustment", "1.2").c_str()));
		ssampler_->gyr_.bias_update(atoi(args_.get_value("bias-samples", "2000").c_str()));
	} else {
		ssampler_.reset(new sampler());
	}
}

void server_app::init_attitude_tracker()
{
	attitude_tracker_.reset(new attitudetracker(atof(args_.get_value("acc-period", "3.5").c_str())));
}

int server_app::run(int argc, const char *argv[])
{
	if (args_.get_value("daemon") == "1") {
		// Inform the io_service that we are about to become a daemon. The
		// io_service cleans up any internal resources, such as threads, that may
		// interfere with forking.
		io_service_.notify_fork(boost::asio::io_service::fork_prepare);

		daemon_init();

		// Inform the io_service that we have finished becoming a daemon. The
		// io_service uses this opportunity to create any internal file descriptors
		// that need to be private to the new process.
		io_service_.notify_fork(boost::asio::io_service::fork_child);

		/*
		 * Redirect stdout and stderr to
		 * the log file.
		 */
		dup2(get_log_file()->get_fd(), 1);
		dup2(get_log_file()->get_fd(), 2);
		setlinebuf(stdout);
		setlinebuf(stderr);
	}
	get_log_file()->log_level(args_.get_value("loglevel", "info"));
	log_info_message("Server starting.");
	init_servo_controller();
	init_sensors_sampler();
	init_attitude_tracker();
	init_user_rpcserver();
	boost::thread rpc_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
	rpc_thread.join();
	ctrl_thread_.stop();
	log_info_message("Server stopping.");
	return 0;
}
