#ifndef EVENT_POOL_HPP
#define EVENT_POOL_HPP

#include <algorithm>
#include <cerrno>
#include <map>
#include <sys/select.h>
#include <unistd.h>

#include "debug.hpp"
#include "event.hpp"
#include "iselector.hpp"
#include "result.hpp"

class EventPool
{
  private:
	std::map<int, Event> pool_;

  public:
	EventPool()
		: pool_(){};
	~EventPool(){};
	void UpdateEvent(Event e)
	{
		switch (e.state_) {
		case Event::RUNNING:
			pool_[e.fd_] = e;
			log(e.fd_, "event add");
			break;
		case Event::STOPPED:
			pool_.erase(e.fd_);
			close(e.fd_);
			log(e.fd_, "event stop");
			break;
		}
	}

	Result<void> MonitorFds(ISelector *selector)
	{
		selector->Import(pool_.begin(), pool_.end()); //[TODO] allocate失敗
		return selector->Run();
	}

	void TriggerEvents(ISelector *selector)
	{
		typedef std::vector<int>::const_iterator iterator;

		const std::vector<int> &ready = selector->Export();
		for (iterator it = ready.begin(); it != ready.end(); it++) {
			Event event = pool_[*it];
			Event res = event.Run();
			UpdateEvent(res);
		}
	}
};

#endif
