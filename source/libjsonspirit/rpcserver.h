#ifndef _RPCSERVER_H_
#define _RPCSERVER_H_

#include <string>
#include <map>
#include <stdexcept>
#include <iostream>
#include "jsontypes.h"

#ifndef ARRAYSIZE
#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))
#endif

enum rpc_error_code
{
    // Standard JSON-RPC 2.0 errors
    RPC_INVALID_REQUEST  = -32600,
    RPC_METHOD_NOT_FOUND = -32601,
    RPC_INVALID_PARAMS   = -32602,
    RPC_INTERNAL_ERROR   = -32603,
    RPC_PARSE_ERROR      = -32700,

    // General application defined errors
    RPC_MISC_ERROR                  = -1,  // std::exception thrown in command handling
    RPC_FORBIDDEN_BY_SAFE_MODE      = -2,  // Server is in safe mode, and command is not allowed in safe mode
    RPC_TYPE_ERROR                  = -3,  // Unexpected type was passed as parameter
    RPC_INVALID_ADDRESS_OR_KEY      = -5,  // Invalid address or key
    RPC_OUT_OF_MEMORY               = -7,  // Ran out of memory during operation
    RPC_INVALID_PARAMETER           = -8,  // Invalid, missing or duplicate parameter
    RPC_DATABASE_ERROR              = -20, // Database error
    RPC_DESERIALIZATION_ERROR       = -22, // Error parsing or validating structure in raw format
    RPC_ENDPOINT_ERROR              = -23, // The request is coming form the same endpoint

    // P2P client errors
    RPC_CLIENT_NOT_CONNECTED        = -9,  // Bitcoin is not connected
    RPC_CLIENT_IN_INITIAL_DOWNLOAD  = -10, // Still downloading initial blocks
    RPC_CLIENT_NODE_ALREADY_ADDED   = -23, // Node is already added
    RPC_CLIENT_NODE_NOT_ADDED       = -24, // Node has not been added before

    // Wallet errors
    RPC_WALLET_ERROR                = -4,  // Unspecified problem with wallet (key not found etc.)
    RPC_WALLET_INSUFFICIENT_FUNDS   = -6,  // Not enough funds in wallet or account
    RPC_WALLET_INVALID_ACCOUNT_NAME = -11, // Invalid account name
    RPC_WALLET_KEYPOOL_RAN_OUT      = -12, // Keypool ran out, call keypoolrefill first
    RPC_WALLET_UNLOCK_NEEDED        = -13, // Enter the wallet passphrase with walletpassphrase first
    RPC_WALLET_PASSPHRASE_INCORRECT = -14, // The wallet passphrase entered was incorrect
    RPC_WALLET_WRONG_ENC_STATE      = -15, // Command given in wrong wallet encryption state (encrypting an encrypted wallet etc.)
    RPC_WALLET_ENCRYPTION_FAILED    = -16, // Failed to encrypt the wallet
    RPC_WALLET_ALREADY_UNLOCKED     = -17, // Wallet is already unlocked
};

struct rpc_server_dummy { };
struct rpc_connection_dummy { };

template<typename T = rpc_server_dummy, typename R = rpc_connection_dummy>
class rpc_server
{
public:
	/*
	 * The rpc types correspond directly to the json value types like obj_type, array_type ...
	 * but they have values 2 pow n, so we can store them in unsigned int like (rpc_str_type|rpc_null_type)
	 * Then we use a vector of these bit masks to specify the parameter types we expect for
	 * the rpc calls.
	 *
	 * get_rpc_type will convert json::Value_type to one of the rpc types.
	 *
	 * create_json_spec will convert the rpc_type bit mask to an integer or array of integers
	 * correspoinding to json Value_type(s)
	 *
	 * rpc_types will convert json spec to a std::vector of rpc_type_ bitmasks.
	 *
	 */
	static const unsigned int rpc_obj_type = 1;
	static const unsigned int rpc_array_type = 2;
	static const unsigned int rpc_str_type = 4;
	static const unsigned int rpc_bool_type = 8;
	static const unsigned int rpc_int_type = 16;
	static const unsigned int rpc_real_type = 32;
	static const unsigned int rpc_null_type = 64;
	enum rpc_exec_mode {
		execute = 0, 	// normal execution
		spec,			// produce machine readable parameter specification
		helpspec, 		// produce a human readable parameter specification
		help, 			// produce a help message
	};
	typedef json::value (T::*rpc_method_type)(R connection, json::array& params, rpc_exec_mode mode);
	typedef std::map<std::string, rpc_method_type> method_map_type;

	rpc_server() {};
	~rpc_server() {};

	static unsigned int get_rpc_type(json::Value_type value_type)
	{
		static const unsigned int rpc_types[] = {rpc_obj_type, rpc_array_type, rpc_str_type, rpc_bool_type, rpc_int_type, rpc_real_type, rpc_null_type};
		return rpc_types[value_type];
	}

	static json::value create_json_spec(unsigned int *arr, size_t n)
	{
		json::array params;

		for (size_t i = 0; i < n; i++) {
			json::array param;
			if (arr[i] & rpc_obj_type)
				param.push_back(json::obj_type);
			if (arr[i] & rpc_array_type)
				param.push_back(json::array_type);
			if (arr[i] & rpc_str_type)
				param.push_back(json::str_type);
			if (arr[i] & rpc_bool_type)
				param.push_back(json::bool_type);
			if (arr[i] & rpc_int_type)
				param.push_back(json::int_type);
			if (arr[i] & rpc_real_type)
				param.push_back(json::real_type);
			if (arr[i] & rpc_null_type)
				param.push_back(json::null_type);
			params.push_back((param.size() > 1) ? param : param[0]);
		}
		return params;
	}

	static std::vector<unsigned int> rpc_types(const json::array& spec)
	{
		std::vector<unsigned int> ret;

		for (size_t i = 0; i < spec.size(); i++) {
			if (spec[i].type() == json::array_type) {
				unsigned int rpc_types = 0;
				for (size_t j = 0; j < spec[i].get_array().size(); j++) {
					rpc_types |= get_rpc_type((json::Value_type)spec[i].get_array()[j].get_int());
				}
				ret.push_back(rpc_types);
			} else {
				ret.push_back(get_rpc_type((json::Value_type)spec[i].get_int()));
			}
		}
		return ret;
	}

	static std::vector<unsigned int> rpc_types(const std::string strspec)
	{
		json::value jsonspec;

		json::read(strspec, jsonspec);
		return rpc_types(jsonspec.get_array());
	}

	static json::value create_json_helpspec(unsigned int *arr, size_t n)
	{
		json::value ret = create_json_spec(arr, n);
		convert_types_to_strings(ret);
		return ret;
	}

	static json::object create_rpc_error(rpc_error_code code, const std::string& message)
	{
		json::object error;
		error["code"] = code;
		error["message"] = message;
		return error;
	}

	void add(const std::string& name, rpc_method_type method)
	{
		if (name.empty())
			throw std::runtime_error("rpc_server::add, invalid name parameter");
		if (!method)
			throw std::runtime_error("rpc_server::add, invalid method parameter");
		map_[name] = method;
	}

	json::value call_method_name(R connection, const json::value& methodname, json::array& params, rpc_exec_mode mode = execute)
	{
		if (methodname.type() != json::str_type)
			throw create_rpc_error(RPC_INVALID_REQUEST, "method must be a string");
		typename method_map_type::const_iterator method_entry = map_.find(methodname.get_str());
		if (method_entry == map_.end())
			throw create_rpc_error(RPC_METHOD_NOT_FOUND, "method not found");
		return (static_cast<T*>(this)->*(method_entry->second))(connection, params, mode);
	}

	json::value call(R connection, const json::value& val, rpc_exec_mode mode = execute)
	{
		json::object ret;
		json::value result;
		json::value error;
		json::value id;
		json::array params;

		try {
			if (val.type() != json::obj_type)
				throw create_rpc_error(RPC_PARSE_ERROR, "top-level object parse error");
			json::object::const_iterator params_it = val.get_obj().find("params");
			if (params_it != val.get_obj().end() && params_it->second.type() != json::array_type)
				throw create_rpc_error(RPC_INVALID_REQUEST, "params must be an array");
			if (params_it != val.get_obj().end())
				params = params_it->second.get_array();
			json::object::const_iterator id_it = val.get_obj().find("id");
			if (id_it != val.get_obj().end())
				id = id_it->second;
			json::object::const_iterator method_it = val.get_obj().find("method");
			if (method_it == val.get_obj().end())
				throw create_rpc_error(RPC_INVALID_REQUEST, "missing method");
			result = call_method_name(connection, method_it->second, params, mode);
		} catch (json::object& e) {
			error = e;
		} catch (std::exception& e) {
			error = std::string(e.what());
		}
		ret["result"] = result;
		ret["id"] = id;
		ret["error"] = error;
		return ret;
	}

	json::value call(R connection, const std::string& name, const json::value& id, const json::array& params, rpc_exec_mode mode = execute)
	{
		json::object req;
		req["id"] = id;
		req["method"] = json::value(name);
		req["params"] = json::value(params);
		return call(connection, json::value(req), mode);

	}

protected:
	static void convert_types_to_strings(json::value& val)
	{
		if (val.type() == json::int_type && val.get_int() >= json::obj_type && val.get_int() <= json::null_type) {
			val = std::string(json::Value_type_name[val.get_int()]);
		} else if (val.type() == json::array_type) {
			for (size_t i = 0; i < val.get_array().size(); i++) {
				convert_types_to_strings(val.get_array()[i]);
			}
		}
	}

	static void verify_parameters(json::array& params, unsigned int *types, size_t n)
	{
		params.resize(n);
		for (size_t i = 0; i < n; i++) {
			json::Value_type value_type = params[i].type();
			if ((get_rpc_type(value_type) & types[i]) == 0) {
				throw create_rpc_error(RPC_INVALID_PARAMS, "Invalid parameter: '" + json::write(params[i]) + "'");
			}
		}
	}

protected:
	method_map_type map_;
};

#endif /* _RPCSERVER_H_ */
