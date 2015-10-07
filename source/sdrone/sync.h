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
#ifndef _SYNC_H_
#define _SYNC_H_

#include <boost/thread/mutex.hpp>

namespace sdrone {

template<typename PARENT>
class basic_mutex: public PARENT
{
public:
	void lock()
	{
		PARENT::lock();
	}

	void unlock()
	{
		PARENT::unlock();
	}

	bool try_lock()
	{
		return PARENT::try_lock();
	}
};


template<typename MUTEX>
class basic_lock
{
public:
	basic_lock(MUTEX& m) : m_(m)
	{
		m_.lock();
	}

	~basic_lock()
	{
		m_.unlock();
	}
private:
	MUTEX& m_;
};

typedef basic_mutex<boost::mutex> mutex;
typedef basic_lock<mutex> lock;


}





#endif
