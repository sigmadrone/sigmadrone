#ifndef CLIENTAPP_H
#define CLIENTAPP_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "libhttp/http_client.hpp"
#include "json.h"
#include "cmdargs.h"

class client_app : private boost::noncopyable
{
public:
	client_app(const cmd_args& args);
	~client_app();
	int run(int argc, char *argv[]);
	std::vector<std::string> parse_command_line(int argc, char *argv[]);
	std::string create_rpc_request(const std::string& method, const std::vector<std::string>& params, const std::vector<unsigned int>& params_types);
	std::string create_rpc_specrequest(const std::string& method);
	static bool is_client(int argc, char *argv[]);

protected:
	const cmd_args& args_;
	boost::scoped_ptr<http::client::http_client> rpc_client_;
};


#endif // APPLICATION_CLIENT_H
