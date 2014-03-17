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
