#ifndef HTTP_APPLICATION_HPP
#define HTTP_APPLICATION_HPP

#include <string>
#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "httpserver/http_server.hpp"

/// The top-level class of the HTTP server.
class application : private boost::noncopyable
{
public:
	application();
	~application();
	void init(int argc, char *argv[]);
	int run();

protected:
	void signal_handler_terminate();
	void usage();

	void jsonrpc_request_handler(const http::server::request& req, http::server::reply& rep);
	void echo_request_handler(const http::server::request& req, http::server::reply& rep);
	void headers_request_handler(const http::server::request& req, http::server::reply& rep);

	boost::asio::io_service io_service_rpc_;
	boost::asio::signal_set signals_;
	std::string appname_;
	boost::scoped_ptr<http::server::http_server> rpc_server_;
};



#endif
