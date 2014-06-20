#include "connection_manager.hpp"
#include <algorithm>
#include <boost/bind.hpp>
#include "http_server.hpp"

namespace http {
namespace server {

connection_manager::connection_manager(http_server& server)
	: server_(server)
{

}

void connection_manager::start(connection_ptr c)
{
	connections_.insert(c);
	c->start();
}

void connection_manager::stop(connection_ptr c)
{
	connections_.erase(c);
	c->stop();
}

void connection_manager::stop_all()
{
	std::for_each(connections_.begin(), connections_.end(), boost::bind(&connection::stop, _1));
	connections_.clear();
}

void connection_manager::handle_pending_requests()
{
	std::for_each(connections_.begin(), connections_.end(), boost::bind(&connection::handle_pending_request, _1));
}

size_t connection_manager::get_remote_connections(std::vector<std::string>& connections)
{
	for (std::set<connection_ptr>::const_iterator it = connections_.begin(); it != connections_.end(); it++) {
		connections.push_back((*it)->get_remote_address());
	}
	return connections.size();
}


} // namespace server
} // namespace http
