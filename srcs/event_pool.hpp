#ifndef EVENT_POOL_HPP
#define EVENT_POOL_HPP

#include <algorithm>
#include <cerrno>
#include <map>
#include <queue>
#include <sys/select.h>
#include <unistd.h>

#include "debug.hpp"
#include "event.hpp"
#include "iselector.hpp"
#include "result.hpp"

class EventPool
{
  public:
	typedef std::queue<Event> Events;

  private:
	std::map<int, Events> pool_;

  public:
	EventPool()
		: pool_(){};
	~EventPool(){};
	void UpdateEvent(Event e)
	{
		pool_[e.fd_].push(e);
		// switch (e.state_) {
		// case Event::RUNNING:
		// 	pool_[e.fd_] = e;
		// 	log(e.fd_, "event add");
		// 	break;
		// case Event::STOPPED:
		// 	pool_.erase(e.fd_);
		// 	close(e.fd_);
		// 	log(e.fd_, "event stop");
		// 	break;
		// }
	}

	Result<void> MonitorFds(ISelector *selector, std::vector<int> &ready)
	{
		selector->Import(pool_.begin(), pool_.end()); //[TODO] allocate失敗
		Result<void> res = selector->Run();
		if (res.IsErr()) {
			return res;
		}
		selector->Export(ready);
		return Result<void>();
	}

	void TriggerEvents(const std::vector<int> &ready)
	{
		typedef std::vector<int>::const_iterator iterator;

		for (iterator it = ready.begin(); it != ready.end(); ++it) {
			int fd = *it;

			Event event = pool_[fd].front();
			pool_[fd].pop();
			std::vector<Event> res = event.Run();
			for (std::vector<Event>::iterator it = res.begin(); it != res.end(); ++it) {
				UpdateEvent(*it);
			}
			if (pool_[fd].empty()) {
				pool_.erase(fd);
				close(fd);
			}
		}
	}
};

#endif
