#include "rpcclient.h"


rpc_client::rpc_client(std::string address, std::string port, size_t timeout)
	: http::client::http_client(address, port, timeout)
	, serial_(0)
	, failures_(0)
{
}

rpc_client::~rpc_client()
{
}

size_t rpc_client::get_failure_count()
{
	return failures_;
}

json::value rpc_client::call(const std::string& url, const std::string& method) throw(std::exception)
{
	json::array params;
	return call(url, method, params);
}

json::value rpc_client::call(const std::string& url, const std::string& method, const json::array& params) throw(std::exception)
{
	json::value rpc_response;
	json::object rpc_request;
	json::object rpc_error;
	http::client::response response;
	http::headers headers;

	headers.insert_header("Connection", "keep-alive");
	rpc_request["jsonrpc"] = "1.0";
	rpc_request["id"] = (int)++serial_;
	rpc_request["method"] = json::value(method);
	rpc_request["params"] = params;
	++failures_;
	request(response, "POST", url, json::write(rpc_request), headers);
	failures_ = 0;
	json::read(response.content, rpc_response);
	if (rpc_response.get_obj()["error"].type() == json::obj_type)
		throw std::runtime_error(rpc_response.get_obj()["error"].get_obj()["message"].get_str());
	return rpc_response.get_obj()["result"];
}

json::value rpc_client::call(const std::string& url, const std::string& method, const json::value& val1) throw(std::exception)
{
	json::array params;
	params.push_back(val1);
	return call(url, method, params);
}

json::value rpc_client::call(const std::string& url, const std::string& method, const json::value& val1, const json::value& val2) throw(std::exception)
{
	json::array params;
	params.push_back(val1);
	params.push_back(val2);
	return call(url, method, params);
}

json::value rpc_client::call(const std::string& url, const std::string& method, const json::value& val1, const json::value& val2, const json::value& val3) throw(std::exception)
{
	json::array params;
	params.push_back(val1);
	params.push_back(val2);
	params.push_back(val3);
	return call(url, method, params);
}


