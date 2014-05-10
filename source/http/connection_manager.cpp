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

} // namespace server
} // namespace http
