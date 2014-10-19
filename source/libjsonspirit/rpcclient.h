#ifndef _RPCCLIENT_H_
#define _RPCCLIENT_H_

#include <string>
#include <stdexcept>
#include "jsontypes.h"
#include "libhttp/http_client.hpp"

class rpc_client : public http::client::http_client
{
public:
	rpc_client(std::string address, std::string port, size_t timeout = 0, const std::string& jsonrpc_version = "1.0");
	~rpc_client();
	json::value call(const std::string& url,
			const std::string& method,
			const json::array& params,
			http::headers headers = http::headers()) throw(std::exception);

	json::value call(const std::string& url,
			const std::string& method,
			http::headers headers = http::headers()) throw(std::exception);

	json::value call(const std::string& url,
			const std::string& method,
			const json::value& val1,
			http::headers headers = http::headers()) throw(std::exception);

	json::value call(const std::string& url,
			const std::string& method,
			const json::value& val1,
			const json::value& val2,
			http::headers headers = http::headers()) throw(std::exception);

	json::value call(const std::string& url,
			const std::string& method,
			const json::value& val1,
			const json::value& val2,
			const json::value& val3,
			http::headers headers = http::headers()) throw(std::exception);

	size_t get_failure_count();

protected:
	std::string jsonrpc_version_;
	size_t serial_;
	size_t failures_;
};


#endif // _RPCCLIENT_H_
