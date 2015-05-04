#include "rpcclient.h"


rpc_client::rpc_client(std::string address, std::string port, size_t timeout, const std::string& jsonrpc_version)
	: http::client::http_client(address, port, timeout)
	, jsonrpc_version_(jsonrpc_version)
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

rexjson::value rpc_client::call(const std::string& url, const std::string& method, const rexjson::array& params, http::headers headers) throw(std::exception)
{
	rexjson::value rpc_response;
	rexjson::object rpc_request;
	rexjson::object rpc_error;
	http::client::response response;

	if (headers.find_header("Connection").empty())
		headers.insert_header("Connection", "keep-alive");
	if (headers.find_header("content-type").empty())
		headers.insert_header("content-type", "application/json-rpc");
	rpc_request["jsonrpc"] = jsonrpc_version_;
	rpc_request["id"] = (int)++serial_;
	rpc_request["method"] = rexjson::value(method);
	rpc_request["params"] = params;
	++failures_;
	request(response, "POST", url, rexjson::write(rpc_request), headers);
	failures_ = 0;
	rpc_response.read(response.content);
	if (rpc_response.get_obj()["error"].type() == rexjson::obj_type)
		throw std::runtime_error(rpc_response.get_obj()["error"].get_obj()["message"].get_str());
	return rpc_response.get_obj()["result"];
}

rexjson::value rpc_client::call(const std::string& url, const std::string& method, http::headers headers) throw(std::exception)
{
	rexjson::array params;
	return call(url, method, params, headers);
}

rexjson::value rpc_client::call(const std::string& url, const std::string& method, const rexjson::value& val1, http::headers headers) throw(std::exception)
{
	rexjson::array params;
	params.push_back(val1);
	return call(url, method, params);
}

rexjson::value rpc_client::call(const std::string& url, const std::string& method, const rexjson::value& val1, const rexjson::value& val2, http::headers headers) throw(std::exception)
{
	rexjson::array params;
	params.push_back(val1);
	params.push_back(val2);
	return call(url, method, params);
}

rexjson::value rpc_client::call(
		const std::string& url,
		const std::string& method,
		const rexjson::value& val1,
		const rexjson::value& val2,
		const rexjson::value& val3,
		http::headers headers) throw(std::exception)
{
	rexjson::array params;
	params.push_back(val1);
	params.push_back(val2);
	params.push_back(val3);
	return call(url, method, params);
}


