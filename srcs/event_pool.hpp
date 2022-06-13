#ifndef EVENT_POOL_HPP
#define EVENT_POOL_HPP

#include <algorithm>
#include <map>
#include <sys/select.h>
#include <unistd.h>

#include "debug.hpp"
#include "event.hpp"
#include "result.hpp"

typedef struct {
	int nfds;
	fd_set read_set;
	// int nb_read_set;
	// fd_set write_set;
} SelectFds;

class EventPool
{
  private:
	std::map<int, Event> pool_;
	std::vector<int> ready_fds_;

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

	// push_back ready_fds in vector
	Result<void> MonitorFds(std::vector<int> &ready)
	{
		typedef std::map<int, Event>::iterator iterator;

		SelectFds set;
		FD_ZERO(&set.read_set);
		for (iterator it = pool_.begin(); it != pool_.end(); ++it) {
			int fd = (*it).first;
			FD_SET(fd, &set.read_set);
		}
		int max_fd = pool_.rbegin()->first;
		set.nfds = max_fd + 1;

		if (select(set.nfds, &set.read_set, NULL, NULL, NULL) <= 0) {
			return Result<void >(Error(errno));
		}

		for (iterator it = pool_.begin(); it != pool_.end(); it++) {
			int fd = (*it).first;
			if (FD_ISSET(fd, &set.read_set)) {
				ready.push_back(fd);
			}
			// if (FD_ISSET(fd, &set->write_set)) {
			// 	event.Run();
			// }
		}

		return Result<void>();
	}

	void TriggerEvents(std::vector<int> &ready)
	{
		typedef std::vector<int>::iterator iterator;

		for (iterator it = ready.begin(); it != ready.end(); it++) {
			Event event = pool_[*it];
			Event res = event.Run();
			UpdateEvent(res);
		}
	}
};

#endif
