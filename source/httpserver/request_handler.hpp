//
// request_handler.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include <map>
#include <string>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace http {
namespace server {

struct reply;
struct request;
class server;

struct uri_handler_base
{
	virtual ~uri_handler_base()
	{
	}
	virtual void handler_callback(const request& req, reply& rep) = 0;
};

template<typename handler_callback_type>
class uri_handler_storage : public uri_handler_base
{
public:
	uri_handler_storage(const handler_callback_type& h)
		: uri_handler_base()
		, handler_callback_(h)
	{
	}

	virtual void handler_callback(const request& req, reply& rep)
	{
		handler_callback_(req, rep);
	}

protected:
	handler_callback_type handler_callback_;
};


class request_handler: private boost::noncopyable
{
public:
	typedef std::map< std::string, boost::shared_ptr<uri_handler_base> > uri_handler_map;

	// Construct with a directory containing files to be served.
	request_handler(server& server);

	// Handle a request and produce a reply.
	void handle_request(const request& req, reply& rep, size_t serial);

	// Add unique URI handler
	template<typename handler_type>
	void add_uri_handler(const std::string& uri, handler_type handler_callback)
	{
		boost::shared_ptr<uri_handler_base> stored_ptr(new uri_handler_storage<handler_type>(handler_callback));
		if (uri_handlers_.insert(std::pair<std::string, boost::shared_ptr<uri_handler_base> >(uri, stored_ptr)).second == false) {
			throw(std::runtime_error("URI: \"" + uri + "\" is already registered"));
		}
	}

protected:
	// Perform URL-decoding on a string. Returns false if the encoding was invalid.
	static bool url_decode(const std::string& in, std::string& out);

	void handle_request_internal(const request& req, reply& rep);

protected:
	server& server_;

	// Registered uri handlers
	uri_handler_map uri_handlers_;

};

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HANDLER_HPP
