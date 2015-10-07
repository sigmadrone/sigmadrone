/*
 *  Sigmadrone
 *  Copyright (c) 2013-2015 The Sigmadrone Developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Martin Stoilov <martin@sigmadrone.org>
 *  Svetoslav Vassilev <svassilev@sigmadrone.org>
 */
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
