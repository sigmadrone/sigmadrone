#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include "reply.hpp"
#include "request.hpp"
#include "http_server.hpp"

namespace http {
namespace server {

request_handler::request_handler(http_server& server)
	: server_(server)
{
}

void request_handler::handle_request(const request& req, reply& rep, size_t serial)
{
	handle_request_internal(req, rep);
	std::stringstream oss;
	oss << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< serial: " << serial << std::endl;
	oss << req << std::endl;
	oss << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
	oss << rep << std::endl;
	oss << "==================================" << std::endl << std::endl << std::endl;
	server_.log_debug_message("%s", oss.str().c_str());
}

void request_handler::handle_request_internal(const request& req, reply& rep)
{
	// Decode url to path.
	std::string request_path;
	if (!url_decode(req.uri, request_path)) {
		rep = reply::stock_reply(reply::bad_request);
		return;
	}
	// Request path must be absolute and not contain "..".
	if (request_path.empty() || request_path[0] != '/' || request_path.find("..") != std::string::npos) {
		rep = reply::stock_reply(reply::bad_request);
		return;
	}
	uri_handler_map::const_iterator uri_handler_iterator = uri_handlers_.find(request_path);
	if (uri_handler_iterator == uri_handlers_.end()) {
		rep = reply::stock_reply(reply::not_found);
		return;
	}
	uri_handler_iterator->second->handler_callback(req, rep);
	if (rep.headers["Connection"].empty()) {
		if (req.headers.header("Connection") == "keep-alive")
			rep.headers.header("Connection", "keep-alive");
		else
			rep.headers.header("Connection", "close");
	}
	if (rep.headers["Content-Length"].empty())
		rep.headers.header("Content-Length", boost::lexical_cast<std::string>(rep.content.size()));
	if (rep.headers["Content-Type"].empty())
		rep.headers.header("Content-Type", "application/json");

}

bool request_handler::url_decode(const std::string& in, std::string& out)
{
	out.clear();
	out.reserve(in.size());
	for (std::size_t i = 0; i < in.size(); ++i) {
		if (in[i] == '%') {
			if (i + 3 <= in.size()) {
				int value = 0;
				std::istringstream is(in.substr(i + 1, 2));
				if (is >> std::hex >> value) {
					out += static_cast<char>(value);
					i += 2;
				} else {
					return false;
				}
			} else {
				return false;
			}
		} else if (in[i] == '+') {
			out += ' ';
		} else {
			out += in[i];
		}
	}
	return true;
}

} // namespace server
} // namespace http
